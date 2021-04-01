/*
 * console_uart.h
 *
 *  Created on: Aug 3, 2020
 *      Author: steph
 */

#ifndef CONSOLE_UART_H_
#define CONSOLE_UART_H_

#include "FreeRTOS.h"
#include "queue.h"

void init_console_uart(QueueHandle_t uart_rx_queue);

int get_char(void);

#endif /* CONSOLE_UART_H_ */
