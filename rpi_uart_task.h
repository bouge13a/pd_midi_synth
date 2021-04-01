/*
 * uart_task.h
 *
 *  Created on: Apr 1, 2021
 *      Author: steph
 */

#ifndef RPI_UART_TASK_H_
#define RPI_UART_TASK_H_

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

void init_rpi_uart(void);

void rpi_uart_task(void* parm);

#endif /* RPI_UART_TASK_H_ */
