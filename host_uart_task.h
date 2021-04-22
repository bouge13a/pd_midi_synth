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
    PITCH,
    MODULATION,
    KNOB1,
    KNOB2,
    KNOB3,
    KNOB4,
}uart_msg_type_e;

typedef struct {
    uint8_t  message_type : 4;
    uint8_t  pad_num      : 8;
    uint16_t value        :16;
    uint8_t  channel      : 4;
}uart_msg_t;

typedef union {
    uart_msg_t bitfield;
    char       bytes[4];
}uart_msg_u;

void init_host_uart(uint32_t oage_number);

void host_uart_task(void* parm);

void send_to_host(uart_msg_u message);

void send_to_host_from_isr(uart_msg_u message);

// GUI functions
void uartmsg_drawpage(void);
void uartmsg_drawdata(void);
void uartmsg_drawinput(int character);



#endif /* HOST_UART_TASK_H_ */
