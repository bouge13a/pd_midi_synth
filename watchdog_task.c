///*
// * watchdog_task.c
// *
// *  Created on: Jan 22, 2021
// *      Author: steph
// */
//
//#include "watchdog_task.h"
//
//static watched_task_t watched_tasks[10];
//static uint32_t total_tasks = 0;
//static uint32_t max_runtime_ms = 0;
//
//// The max_runtime_ms variable is multiplied by the factor to get total wd window
//const static uint32_t max_run_time_factor = 4;
//
//void init_watchdog(void) {
//
//} // End init_watchdog
//
//uint32_t add_to_watch(uint32_t expected_time_ms) {
//
//    uint32_t index;
//
//    watched_tasks[total_tasks].expected_time_ms = expected_time_ms;
//    watched_tasks[total_tasks].run_time_ms      = 0;
//    watched_tasks[total_tasks].active = true;
//    watched_tasks[total_tasks].kicked = false;
//
//    total_tasks++;
//
////    if (watched_tasks[total_tasks].expected_time_ms
////            > watched_tasks[total_tasks-1].expected_time_ms) {
////        max_runtime_ms =
////    }
//
//    return (total_tasks - 1);
//} // End add_to_watch(void)
//
//uint32_t unwatch(uint32_t task_index) {
//    watched_tasks[total_tasks].active = false;
//}
//
//uint32_t watch(uint32_t task_index) {
//    watched_tasks[total_tasks].active = true;
//}
//
//
//void pet_the_dog(uint32_t task_index) {
//    watched_tasks[task_index].run_time_ms = 0;
//
//} // End pet_the_dog
//
//// Only the watchdog_task can kick the dog
//static void kick_the_dog(void){
//
//} // End kick_the_dog
//
//void watchdog_task(void* parm) {
//
//    uint32_t index;
//
//    while(1) {
//
//        for (index=0; index<total_tasks; index++) {
//
////            if (watched_tasks[index].active) {
////                if (watched_tasks[index].kicked) {
////                    watched_tasks[index].
////                }
////            }
//        }
//
//        vTaskDelay(1);
//    }
//
//} // End watchdog_task
