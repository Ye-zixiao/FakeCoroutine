//
// Created by Ye-zixiao on 2021/4/19.
//

#ifndef COROUTINE__COROUTINE_H_
#define COROUTINE__COROUTINE_H_

#include <ucontext.h>
#include <memory>
#include <unordered_map>

namespace fc {

enum State {
  kCOROUTINE_DEAD = 0,
  kCOROUTINE_READY,
  kCOROUTINE_RUNNING,
  kCOROUTINE_SUSPEND
};

class CoroutineSchedule;

using CoroutineFunc = void (*)(CoroutineSchedule *, void *);

/***
 * 协程类，实际上用户并不会使用到它
 */
class Coroutine {
 public:
  Coroutine(CoroutineSchedule *schedule, CoroutineFunc func, void *args);
  ~Coroutine();

 public:
  CoroutineSchedule *schedule_;
  ucontext_t ctx_; // 协程自己的用户上下文
  ptrdiff_t size_; // 当前运行时协程栈大小
  ptrdiff_t cap_;  // 当前协程栈大小，在调用save_stack后两者一致
  char *stack_;    // 协程私有栈空间，用来暂时保存让出后的栈空间数据
  State status_;   // 协程当前状态

  CoroutineFunc func_; // 协程函数
  void *args_;         // 协程函数参数
};

/***
 * 协程调度类，用户通过它来创建、启动/恢复、让出协程
 */
class CoroutineSchedule {
 public:
  using CoroutinePtr = std::shared_ptr<Coroutine>;
  using CoroutineID = int;

  CoroutineSchedule();
  ~CoroutineSchedule();

  CoroutineSchedule(const CoroutineSchedule &) = delete;
  CoroutineSchedule &operator=(CoroutineSchedule &) = delete;

  // 有必要设置移动接口吗？

  CoroutineID co_create(CoroutineFunc func, void *args);
  void co_resume(CoroutineID id);
  void co_yield();

  CoroutineID co_runningID() const;
  State co_status(CoroutineID id);

 public:
  constexpr static int kSTACKSIZE = 1024 * 1024;

 private:
  static void mainFunc(uint32_t low32, uint32_t high32);

 private:
  using CoroutineMap = std::unordered_map<CoroutineID, CoroutinePtr>;

  char stack_[kSTACKSIZE];  // 各个协程公共使用的栈帧空间，不允许超过kSTACKSIZE大小
  CoroutineMap map_;        // 协程容器，由协程调度类记录各个协程对象的指针
  CoroutineID running_;     // 正在执行的协程ID
  ucontext_t main_;         // 主协程的用户上下文，当次协程选择让出后会回到主协程，由它来负责调度到下个次协程
  CoroutineID nco_;         // 下一个协程编号，它不是用来记录次协程数量的！
};

} // namespace fc

#endif //COROUTINE__COROUTINE_H_
