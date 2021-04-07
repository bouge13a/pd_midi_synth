/*
 * uart_task.c
 *
 *  Created on: Apr 1, 2021
 *      Author: steph
 */

#include <host_uart_task.h>
#include <stdbool.h>

#include "GPOs.h"
#include "console_task.h"

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
#include "driverlib/inc/hw_gpio.h"
#include "driverlib/inc/hw_types.h"

static QueueHandle_t uart_tx_q = NULL;
static gpio_pin_t* blue_debug;
static uint32_t page_num;

void init_host_uart(uint32_t page_number) {

    page_num = page_number;

    uart_tx_q = xQueueCreate(12, sizeof(uart_msg_u));
    blue_debug = get_gpo_config("blue debug");

    set_gpo(blue_debug, 0);

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Enable the peripherals used by UART
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART1));

    // Set GPIO A0 and A1 as UART pins.
    MAP_GPIOPinConfigure(GPIO_PB0_U1RX);
    MAP_GPIOPinConfigure(GPIO_PB1_U1TX);

    GPIOPinTypeUART(GPIO_PORTB_AHB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Use the internal 16MHz oscillator as the UART clock source.
    MAP_UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);

    UARTFIFOEnable(UART1_BASE);

    MAP_UARTConfigSetExpClk(UART1_BASE, MAP_SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

} // End init_host_uart

void host_uart_task(void* parm) {

    uint32_t loop_index;

    uart_msg_u uart_msg;

    while(1){

        xQueueReceive(uart_tx_q, &uart_msg, portMAX_DELAY);

        set_gpo(blue_debug, 1);

        switch(uart_msg.bitfield.message_type) {
            case NOTE_ON:
            case OVERDRIVE:
                for(loop_index=0; loop_index<3; loop_index++) {
                    MAP_UARTCharPutNonBlocking(UART1_BASE, uart_msg.bytes[loop_index]);
                }

                if (is_on_screen(page_num)) {
                    UARTprintf("%d %d %d\n", uart_msg.bitfield.message_type,
                                             uart_msg.bitfield.pad_num,
                                             uart_msg.bitfield.value);
                }

                break;

            case NOTE_OFF:
                MAP_UARTCharPutNonBlocking(UART1_BASE, uart_msg.bytes[0]);

                if (is_on_screen(page_num)) {
                    UARTprintf("%d %d\n", uart_msg.bitfield.message_type,
                                             uart_msg.bitfield.pad_num);
                }

                break;

            default:
                break;
        }



        set_gpo(blue_debug, 0);

    }

} // End host_uart_task

void send_to_host(uart_msg_u message) {

    xQueueSend(uart_tx_q, &message, 0);

}

void send_to_host_from_isr(uart_msg_u message) {

    xQueueSendFromISR(uart_tx_q, &message, 0);

}

// GUI functions
void uartmsg_drawpage(void) {

}
void uartmsg_drawdata(void){

}
void uartmsg_drawinput(int character){

}

