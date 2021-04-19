// Author: Ye-zixiao
// Date: 2020-04-19

#include "Coroutine.h"
#include <iostream>

using namespace std;
using namespace fc;

/***
 * 协程函数
 * @param schedule
 * @param args
 */
void coFunc(CoroutineSchedule *schedule, void *args) {
  int *init = static_cast<int *>(args);
  for (int i = 0; i < 5; ++i) {
	printf("coroutine %d: %d\n", schedule->co_runningID(), *init + i);
	schedule->co_yield();
  }
}

/***
 * 协程测试函数
 * @param schedule
 */
void testFunc(CoroutineSchedule &schedule) {
  int init1 = 0, init2 = 100;

  auto co1 = schedule.co_create(coFunc, &init1);
  auto co2 = schedule.co_create(coFunc, &init2);
  cout << "main coroutine start" << endl;
  while (schedule.co_status(co1) && schedule.co_status(co2)) {
	// 启动或调度到协程1
	schedule.co_resume(co1);
	// 启动或调度到协程2
	schedule.co_resume(co2);
	// 此处回到主协程
  }
  cout << "main coroutine end" << endl;
}

int main() {
  CoroutineSchedule schedule;
  testFunc(schedule);
  return 0;
}