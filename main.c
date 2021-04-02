

/**
 * main.c
 */

#include <GPIs.h>
#include <GPOs.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdbool.h>

#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

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

int main(void){

    QueueHandle_t console_uart_rx_q = NULL;

    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    MAP_FPUEnable();
    MAP_FPULazyStackingEnable();

    // Set the system clock to 50 MHz
    MAP_SysCtlClockSet(SYSCTL_SYSDIV_4 |
                       SYSCTL_USE_PLL |
                       SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

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

    init_logger();

    init_console_uart(console_uart_rx_q);

    init_console(console_uart_rx_q);

    // Enable processor interrupts.
    IntMasterEnable();

    /////////////////////////////////////////////////////////
    //                  Add pages to Console
    /////////////////////////////////////////////////////////

    add_page("Homepage1",
             homepage_drawpage,
             homepage_drawdata,
             homepage_drawinput,
             portMAX_DELAY);

    add_page("Controls",
             control_drawpage,
             control_drawdata,
             control_drawinput,
             250);

    add_page("Logger",
             logger_drawpage,
             logger_drawdata,
             logger_drawinput,
             1000);

    add_page("Task Manager",
             taskmanager_drawpage,
             taskmanager_drawdata,
             taskmanager_drawinput,
             portMAX_DELAY);

    ///////////////////////////////////////////////////////
    //                Create Tasks
    ///////////////////////////////////////////////////////

    xTaskCreate(console_task,               /* Function that implements the task. */
                "Console",                  /* Text name for the task. */
                220,                        /* Stack size in words, not bytes. */
                NULL,                       /* Parameter passed into the task. */
                1,                          /* Priority at which the task is created. */
                NULL );                     /* Used to pass out the created task's handle. */

    xTaskCreate(adc_task,               /* Function that implements the task. */
                "ADC",                  /* Text name for the task. */
                220,                        /* Stack size in words, not bytes. */
                NULL,                       /* Parameter passed into the task. */
                3,                          /* Priority at which the task is created. */
                NULL );                     /* Used to pass out the created task's handle. */

    xTaskCreate(i2c_task,               /* Function that implements the task. */
                "I2C1",                  /* Text name for the task. */
                configMINIMAL_STACK_SIZE,                        /* Stack size in words, not bytes. */
                NULL,                       /* Parameter passed into the task. */
                3,                          /* Priority at which the task is created. */
                NULL );                     /* Used to pass out the created task's handle. */



    ///////////////////////////////////////////////////////
    //               Start Scheduler
    ///////////////////////////////////////////////////////
    vTaskStartScheduler();
}


