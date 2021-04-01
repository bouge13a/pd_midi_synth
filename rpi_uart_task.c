/*
 * uart_task.c
 *
 *  Created on: Apr 1, 2021
 *      Author: steph
 */

#include <stdbool.h>


#include "rpi_uart_task.h"

#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "utils/uartstdio.h"

static QueueHandle_t uart_tx_q = NULL;

void init_rpi_uart(void) {

    uart_tx_q = xQueueCreate(100, sizeof(uint8_t));

} // End init_rpi_uart

