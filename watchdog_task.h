///*
// * watchdog.h
// *
// *  Created on: Jan 22, 2021
// *      Author: steph
// */
//
//#ifndef WATCHDOG_TASK_H_
//#define WATCHDOG_TASK_H_
//
//#include <stdbool.h>
//#include <stdint.h>
//
//#include "FreeRTOS.h"
//#include "task.h"
//
//typedef struct {
//    uint32_t expected_time_ms;
//    uint32_t run_time_ms;
//    bool active;
//    bool kicked;
//} watched_task_t;
//
//void init_watchdog(void);
//
//uint32_t add_to_watch(uint32_t expected_time_ms);
//uint32_t unwatch(uint32_t task_index);
//uint32_t watch(uint32_t task_index);
//
//void pet_the_dog(uint32_t task_index);
//
//void watchdog_task(void* parm);
//
//
//
//#endif /* WATCHDOG_TASK_H_ */
