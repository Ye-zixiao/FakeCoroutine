// Author: Ye-zixiao
// Date: 2020-04-19

#include "Coroutine.h"
#include <iostream>
#include <queue>

using namespace std;
using namespace fc;

constexpr int kLOOPTIMES = 100;

/***
 * 生产者协程
 * @param schedule
 * @param args
 */
void coProducer(CoroutineSchedule *schedule, void *args) {
  auto *unBlockingQueue = static_cast<queue<int> *>(args);
  int start = 100;

  for (int i = 0; i < kLOOPTIMES; ++i) {
	unBlockingQueue->push(i + start);
	cout << "producer push " << i + start << endl;
	schedule->co_yield();
  }
}

/***
 * 消费者协程
 * @param schedule
 * @param args
 */
void coConsumer(CoroutineSchedule *schedule, void *args) {
  auto unBlockingQueue = static_cast<queue<int> *>(args);

  for (int i = 0; i < kLOOPTIMES; ++i) {
	auto thing = unBlockingQueue->front();
	unBlockingQueue->pop();
	cout << "consumer pop " << thing << endl;
	schedule->co_yield();
  }
}

/***
 * 协程测试函数
 * @param schedule
 * @param unBlockingQueue
 */
void testFunc1(CoroutineSchedule &schedule, queue<int> &unBlockingQueue) {
  // 创建协程对象，并返回协程对象ID
  auto producer = schedule.co_create(coProducer, &unBlockingQueue);
  auto consumer = schedule.co_create(coConsumer, &unBlockingQueue);

  cout << "main coroutine start" << endl;
  while (schedule.co_status(producer) && schedule.co_status(consumer)) {
    // 启动/调度到生产者协程
	schedule.co_resume(producer);
	// 启动/调度到消费者协程
	schedule.co_resume(consumer);
	// 恢复到主协程
  }
  cout << "main coroutine end" << endl;
}

int main() {
  CoroutineSchedule schedule;
  queue<int> unBlockingQueue;
  testFunc1(schedule, unBlockingQueue);
  return 0;
}