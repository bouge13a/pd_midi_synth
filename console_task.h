/*
 * console_task.h
 *
 *  Created on: Aug 3, 2020
 *      Author: steph
 */

#ifndef CONSOLE_TASK_H_
#define CONSOLE_TASK_H_

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include <stdbool.h>


typedef enum {
    UP_KEY = 21,
    DOWN_KEY,
    RIGHT_KEY,
    LEFT_KEY,
    ESCAPE_KEY = 27,
}arrow_keys;

void init_console(QueueHandle_t uart_rx_queue);

void add_page(const char* name,
              void (*pfnDrawPage)(void),
              void (*pfnDrawData)(void),
              void (*pfnDrawInput)(int),
              uint32_t refresh_rate,
              bool write_to_page);

void console_task(void* parm);

#endif /* CONSOLE_TASK_H_ */
