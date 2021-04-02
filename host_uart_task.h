/*
 * uart_task.h
 *
 *  Created on: Apr 1, 2021
 *      Author: steph
 */

#ifndef HOST_UART_TASK_H_
#define HOST_UART_TASK_H_

#include <stdint.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

typedef enum {
    NOTE_ON,
    NOTE_OFF,
    OVERDRIVE,
}uart_msg_type_e;

typedef struct {
    uint8_t  message_type : 4;
    uint8_t  pad_num      : 4;
    uint16_t value        :16;
    uint8_t  reserved     : 8;
}uart_msg_t;

typedef union {
    uart_msg_t bitfield;
    char       bytes[4];
}uart_msg_u;

void init_host_uart(void);

void host_uart_task(void* parm);

void send_to_host(uart_msg_u message);

void send_to_host_from_isr(uart_msg_u message);


#endif /* HOST_UART_TASK_H_ */