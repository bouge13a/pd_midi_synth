/*
 * looper_board.c
 *
 *  Created on: Apr 11, 2021
 *      Author: steph
 */


#include <usb_midi_task.h>
#include "looper_board.h"

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
#include "looper_buttons_task.h"
#include "looper_effects.h"
#include "looper_volume_functions.h"
#include "ads1x15_task.h"

static QueueHandle_t console_uart_rx_q_looper = NULL;

void init_looper_board(void) {

    /////////////////////////////////////////////////////////
    //           Create Queues and Semaphores
    /////////////////////////////////////////////////////////

    console_uart_rx_q_looper = xQueueCreate(10, sizeof(uint8_t));


    /////////////////////////////////////////////////////////
    //                    Initialization
    /////////////////////////////////////////////////////////


    init_i2c();

    init_gpis();

    init_gpos();

    init_adc();

    init_console_uart(console_uart_rx_q_looper);

    init_console(console_uart_rx_q_looper);

    init_logger();

    init_looper_buttons();

    init_looper_effects();

    init_looper_volumes();

    init_ads1x15();


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

    add_page("ADS1015",
             ads1x15_drawpage,
             ads1x15_drawdata,
             ads1x15_drawinput,
             250,
             false);

    init_usb_midi(add_page("MIDI msgs",
                           usb_midi_drawpage,
                           usb_midi_drawdata,
                           usb_midi_drawinput,
                            portMAX_DELAY,
                            true));

    ///////////////////////////////////////////////////////
    //                Create Tasks
    ///////////////////////////////////////////////////////

    xTaskCreate(console_task,               /* Function that implements the task. */
                "Console",                  /* Text name for the task. */
                220,                        /* Stack size in words, not bytes. */
                NULL,                       /* Parameter passed into the task. */
                3,                          /* Priority at which the task is created. */
                NULL );                     /* Used to pass out the created task's handle. */

//    xTaskCreate(looper_adc_task,               /* Function that implements the task. */
//                "ADC",                  /* Text name for the task. */
//                400,                        /* Stack size in words, not bytes. */
//                NULL,                       /* Parameter passed into the task. */
//                3,                          /* Priority at which the task is created. */
//                NULL );                     /* Used to pass out the created task's handle. */

    xTaskCreate(i2c_task,               /* Function that implements the task. */
                "I2C1",                  /* Text name for the task. */
                configMINIMAL_STACK_SIZE,                        /* Stack size in words, not bytes. */
                NULL,                       /* Parameter passed into the task. */
                3,                          /* Priority at which the task is created. */
                NULL );                     /* Used to pass out the created task's handle. */

    xTaskCreate(looper_buttons_task,               /* Function that implements the task. */
                "Buttons",                  /* Text name for the task. */
                configMINIMAL_STACK_SIZE,                        /* Stack size in words, not bytes. */
                NULL,                       /* Parameter passed into the task. */
                3,                          /* Priority at which the task is created. */
                NULL );                     /* Used to pass out the created task's handle. */

    xTaskCreate(ads1x15_looper_task,               /* Function that implements the task. */
                "Buttons",                  /* Text name for the task. */
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


}

