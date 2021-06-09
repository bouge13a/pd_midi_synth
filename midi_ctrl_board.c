/*
 * midi_ctrl_board.c
 *
 *  Created on: Apr 11, 2021
 *      Author: steph
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdbool.h>

#include "midi_ctrl_board.h"

#include "console_task.h"
#include "homepage.h"
#include "console_uart.h"
#include "control_page.h"
#include "ADC_task.h"
#include "I2C_task.h"
#include "logger.h"
#include "task_manager_page.h"
#include "GPIs.h"
#include "GPOs.h"
#include "usb_midi_task.h"
#include "drum_pad_functions.h"
#include "midi_channel_select.h"
#include "ads1x15_task.h"
#include "midi_buttons_task.h"

static QueueHandle_t console_uart_rx_q = NULL;

void init_midi_ctrl_board(void){
    /////////////////////////////////////////////////////////
    //           Create Queues and Semaphores
    /////////////////////////////////////////////////////////

    console_uart_rx_q = xQueueCreate(10, sizeof(uint8_t));


    /////////////////////////////////////////////////////////
    //                    Initialization
    /////////////////////////////////////////////////////////

    init_i2c();

    init_gpis();

    init_gpos();

    init_adc();

    init_console_uart(console_uart_rx_q);

    init_console(console_uart_rx_q);

    init_logger();

    init_midi_channel_select();

    init_ads1x15();

    init_midi_buttons();

    /////////////////////////////////////////////////////////
    //                  Add pages to Console
    /////////////////////////////////////////////////////////

    add_page("Homepage1",
             homepage_drawpage,
             homepage_drawdata,
             homepage_drawinput,
             portMAX_DELAY,
             false);

    add_page("Controls",
             control_drawpage,
             control_drawdata,
             control_drawinput,
             250,
             false);

    add_page("Logger",
             logger_drawpage,
             logger_drawdata,
             logger_drawinput,
             1000,
             false);

    add_page("Task Manager",
             taskmanager_drawpage,
             taskmanager_drawdata,
             taskmanager_drawinput,
             portMAX_DELAY,
             false);

    init_usb_midi(add_page("USB MIDI",
                           usb_midi_drawpage,
                           usb_midi_drawdata,
                           usb_midi_drawinput,
                            portMAX_DELAY,
                            true));

    init_drumpad(add_page("Slope",
                              slope_drawpage,
                              slope_drawdata,
                              slope_drawinput,
                              portMAX_DELAY,
                              true));

    add_page("ads1x15",
             ads1x15_drawpage,
             ads1x15_drawdata,
             ads1x15_drawinput,
             500,
             false);

    ///////////////////////////////////////////////////////
    //                Create Tasks
    ///////////////////////////////////////////////////////

    xTaskCreate(console_task,               /* Function that implements the task. */
                "Console",                  /* Text name for the task. */
                220,                        /* Stack size in words, not bytes. */
                NULL,                       /* Parameter passed into the task. */
                3,                          /* Priority at which the task is created. */
                NULL );                     /* Used to pass out the created task's handle. */

    xTaskCreate(midi_adc_task,               /* Function that implements the task. */
                "ADC",                  /* Text name for the task. */
                400,                        /* Stack size in words, not bytes. */
                NULL,                       /* Parameter passed into the task. */
                3,                          /* Priority at which the task is created. */
                NULL );                     /* Used to pass out the created task's handle. */

    xTaskCreate(i2c_task,               /* Function that implements the task. */
                "I2C1",                  /* Text name for the task. */
                configMINIMAL_STACK_SIZE,                        /* Stack size in words, not bytes. */
                NULL,                       /* Parameter passed into the task. */
                3,                          /* Priority at which the task is created. */
                NULL );                     /* Used to pass out the created task's handle. */


    xTaskCreate(usb_midi_task,               /* Function that implements the task. */
                "USB MIDI TX",                  /* Text name for the task. */
                configMINIMAL_STACK_SIZE,                        /* Stack size in words, not bytes. */
                NULL,                       /* Parameter passed into the task. */
                3,                          /* Priority at which the task is created. */
                NULL );                     /* Used to pass out the created task's handle. */

    xTaskCreate(usb_midi_rx_task,               /* Function that implements the task. */
                "USB MIDI RX",                  /* Text name for the task. */
                configMINIMAL_STACK_SIZE,                        /* Stack size in words, not bytes. */
                NULL,                       /* Parameter passed into the task. */
                3,                          /* Priority at which the task is created. */
                NULL );                     /* Used to pass out the created task's handle. */

    xTaskCreate(midi_channel_select_task,               /* Function that implements the task. */
                "Chan Buttons",                  /* Text name for the task. */
                configMINIMAL_STACK_SIZE,                        /* Stack size in words, not bytes. */
                NULL,                       /* Parameter passed into the task. */
                3,                          /* Priority at which the task is created. */
                NULL );                     /* Used to pass out the created task's handle. */


    xTaskCreate(ads1x15_midi_task,               /* Function that implements the task. */
                "ADS1X15",                  /* Text name for the task. */
                configMINIMAL_STACK_SIZE,                        /* Stack size in words, not bytes. */
                NULL,                       /* Parameter passed into the task. */
                3,                          /* Priority at which the task is created. */
                NULL );                     /* Used to pass out the created task's handle. */

    xTaskCreate(midi_buttons_task,               /* Function that implements the task. */
                "MIDI buttons",                  /* Text name for the task. */
                configMINIMAL_STACK_SIZE,                        /* Stack size in words, not bytes. */
                NULL,                       /* Parameter passed into the task. */
                3,                          /* Priority at which the task is created. */
                NULL );                     /* Used to pass out the created task's handle. */


}
