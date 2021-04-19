//
// Created by Ye-zixiao on 2021/4/19.
//

#include "Coroutine.h"

#include <cstring>
#include <cassert>
//#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif

using namespace fc;

Coroutine::Coroutine(CoroutineSchedule *schedule, CoroutineFunc func, void *args)
	: schedule_(schedule),
	  ctx_{},
	  size_(0),
	  cap_(0),
	  stack_(nullptr),
	  status_(kCOROUTINE_READY),
	  func_(func),
	  args_(args) {}

Coroutine::~Coroutine() {
  if (stack_) delete[] stack_;
#ifdef DEBUG
  std::cout << "~Coroutine()" << std::endl;
#endif
}

CoroutineSchedule::CoroutineSchedule() :
	stack_{},
	map_(),
	running_(-1),
	main_{},
	nco_(0) {}

CoroutineSchedule::~CoroutineSchedule() {
  map_.clear(); // 清理所有的协程对象
}

/***
 * 创建一个新的协程对象，但返回给用户的是它的ID
 * @param func
 * @param args
 * @return
 */
CoroutineSchedule::CoroutineID CoroutineSchedule::co_create(CoroutineFunc func, void *args) {
  CoroutinePtr coroutine_ptr(
	  std::make_shared<Coroutine>(this, func, args));
  // 协程ID只会一直正向生长，其实在实际执行的时候前面的有些ID已经不再使用了，
  // 那么怎么重复使用它们？还是做个环回操作，当ID增长到kMAXID之后重新回到0
  // 以while找一个不再使用的协程ID？fix it！
  CoroutineID currID = nco_++;
  map_[currID] = coroutine_ptr;
  return currID;
}

/***
 * 启动/恢复某一个指定的协程。若该协程失效，则不调度
 * @param id
 */
void CoroutineSchedule::co_resume(CoroutineID id) {
  if (map_.find(id) == map_.end())
	return;

  CoroutinePtr cp = map_[id];
  State state = cp->status_;
  switch (state) {
	case kCOROUTINE_READY: {
	  // 将一个新创建协程对象中的将要切换到用户上下文进行初始化。
	  // cp->ctx_.uc_stack指定下面的swapcontext()在切换后会使用调度器中的共享栈，
	  // ctx_.uc_link指定在协程函数执行完之后会返回到主协程中（this->main_记录了主
	  // 协程的用户上下文）
	  getcontext(&cp->ctx_);
	  cp->ctx_.uc_stack.ss_sp = this->stack_;
	  cp->ctx_.uc_stack.ss_size = kSTACKSIZE;
	  cp->ctx_.uc_link = &this->main_;
	  this->running_ = id;
	  cp->status_ = kCOROUTINE_RUNNING;

	  /***
	   * 由于makecontext()函数后面的函数变参列表只能为下面的mainFunc传递int类型的
	   * 可变参，所以我们将64位的虚拟地址拆成两个32位的数，在调用mainFunc()的时候重新
	   * 拼接即可。
	   * 其中次协程在启动后会去执行mainFunc，而mainFunc会自动去执行用户传递的协程函数，
	   * 在执行协程函数的时候，用户可以反复让出yield()CPU的控制权给别的协程。当这个协程
	   * 函数执行完毕之后，mainFunc就会自动从调度器中的协程容器中删除这个协程对象，此时
	   * 协程ID也就相应的失效了，用户不可能再恢复这个协程，即使调用了resume()也没什么用
	   */
	  auto ptr = (uintptr_t)this;
	  makecontext(&cp->ctx_, (void (*)())mainFunc,
				  2, (uint32_t)ptr, (uint32_t)(ptr >> 32));
	  // 保存主协程的用户上下文到this->main_中，换入次协程cp的用户上下文，转而执行次协程的函数
	  swapcontext(&this->main_, &cp->ctx_);
	  break;
	}
	case kCOROUTINE_SUSPEND: {
	  memcpy(this->stack_ + kSTACKSIZE - cp->size_, cp->stack_, cp->size_);
	  this->running_ = id;
	  cp->status_ = kCOROUTINE_RUNNING;
	  swapcontext(&this->main_, &cp->ctx_);
	  break;
	}
	default: {
	  assert(false);
	}
  }
#ifdef DEBUG
  std::cout << "coroutine shared_ptr use_count(): "
			<< cp.use_count() << std::endl;
#endif
}

static void save_stack(CoroutineSchedule::CoroutinePtr cp, char *top) {
  // 这个guard哨兵变量比较巧妙，因为它分配的位置必然是共享协程栈的
  // 栈顶，所以我们可以很容易的确定当前协程在这个共享协程中的使用量。
  // 如此我们就可以保存到协程对象自己管理的私有协程栈缓冲区中，如果
  // 协程对象中管理的缓冲区不够，则重新分配。
  char guard = 0;
  assert(top - &guard <= CoroutineSchedule::kSTACKSIZE);
  if (cp->cap_ < top - &guard) {
	delete[] cp->stack_;
	cp->cap_ = top - &guard;
	cp->stack_ = new char[cp->cap_];
  }
  cp->size_ = top - &guard;
  memcpy(cp->stack_, &guard, cp->size_);
}

void CoroutineSchedule::co_yield() {
  CoroutineID id = running_;
  assert(id >= 0);

  CoroutinePtr cp = map_[id];
  save_stack(cp, stack_ + kSTACKSIZE);
  cp->status_ = kCOROUTINE_SUSPEND;
  running_ = -1;
  // 让出CPU控制权，换到主协程，由协程调度器来负责调度到下一个协程
  swapcontext(&cp->ctx_, &main_);
}

CoroutineSchedule::CoroutineID CoroutineSchedule::co_runningID() const {
  return running_;
}

State CoroutineSchedule::co_status(CoroutineID id) {
  bool valid = map_.find(id) != map_.end();
  return valid ? map_[id]->status_ : kCOROUTINE_DEAD;
}

/***
 * 每一个次协程都会调用该函数，并在其中调用用户定义的协程函数。执行的过程中
 * 可能发生多次让出与恢复，当用户的协程函数执行完毕后，该函数会自动析构协程对象
 * @param low32
 * @param high32
 */
void CoroutineSchedule::mainFunc(uint32_t low32, uint32_t high32) {
  uintptr_t ptr = (uintptr_t)low32 | ((uintptr_t)high32 << 32);
  auto *schedule = reinterpret_cast<CoroutineSchedule *>(ptr);

  CoroutineID id = schedule->running_;
  auto cp = schedule->map_[id];
  cp->func_(schedule, cp->args_);
  schedule->map_.erase(id);
  schedule->running_ = -1;
  // 到这个位置协程对象的共享指针为2，其中一个很显然就是当前函数中的共享指针cp，
  // 另一个共享指针位于co_resume()成员函数中！所以这个函数结束后可以安全的
  // 切回主协程。当离开这个函数后，引用计数为1，当主协程离开co_resume()函数
  // 后引用计数变为0，此时才自动析构协程对象。
}