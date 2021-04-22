/*
 * homepage.c
 *
 *  Created on: Aug 4, 2020
 *      Author: steph
 */
#include <stdbool.h>
#include <stdint.h>
#include <uartstdio.h>
#include "FreeRTOS.h"

#include "homepage.h"
#include "text_controls.h"
#include "driverlib/sysctl.h"


void homepage_drawpage(void) {

    cursor_pos(5, 0);
    UARTprintf("Processor:   TM4C123GH6PM\r\n");
    UARTprintf("Core:        ARM Cortex-M4F\r\n");
    UARTprintf("Performance: 80 MHz\r\n");
    UARTprintf("SRAM:        32 KB\r\n");
    UARTprintf("Flash:       256 KB\r\n\n");

    UARTprintf("Clock        %d MHz\r\n", 16);
    UARTprintf("Heap Size:   %d Bytes\r\n", configTOTAL_HEAP_SIZE);
    UARTprintf("Heap Used:   %d Bytes\r\n", configTOTAL_HEAP_SIZE - xPortGetFreeHeapSize());
}

void homepage_drawdata(void) {

}

void homepage_drawinput(int character) {

}
