/*
 * taskmanager_page.c
 *
 *  Created on: Aug 8, 2020
 *      Author: steph
 */

#include <stdint.h>
#include <stdbool.h>
#include <uartstdio.h>

#include "task_manager_page.h"
#include "text_controls.h"

#include "FreeRTOS.h"
#include "task.h"

static char write_buffer[400];

void taskmanager_drawpage(void) {

    uint32_t idx;

    vTaskList(write_buffer);
    cursor_pos(5, 0);

    for(idx=0; idx<400; idx++) {
        UARTprintf("%c", write_buffer[idx]);
    }

} // End taskmanager_drawpage

void taskmanager_drawdata(void) {

} // End taskmanager_drawdata

void taskmanager_drawinput(int character) {

} // End taskmanager_drawinput
