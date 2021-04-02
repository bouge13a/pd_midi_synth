    /*
 * console_uart.c
 *
 *  Created on: Aug 3, 2020
 *      Author: steph
 */

#include <console_uart.h>
#include <stdbool.h>

#include "task.h"

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

static QueueHandle_t uart_rx_q;
static QueueHandle_t uart_tx_q;


int get_char(void) {
    uint8_t character;

    xQueueReceive(uart_rx_q, &character, 0);

    return character;
}

static void UART_RX_int_handler(void) {

    uint8_t character = 0;

    // Get the interrrupt status.
    uint32_t ui32Status = 0;

    //Get the charater from UART0
    character = MAP_UARTCharGetNonBlocking(UART0_BASE);

    xQueueSendFromISR(uart_rx_q, &character, 0);

    ui32Status = MAP_UARTIntStatus(UART0_BASE, true);

    // Clear the asserted interrupts.
    MAP_UARTIntClear(UART0_BASE, ui32Status);

} // End UART_RX_int_handler(void)

void init_console_uart(QueueHandle_t uart_rx_queue) {

    // Create the UART tx queue
    uart_tx_q = xQueueCreate(100, sizeof(uint8_t));

    // Set the UART rx queue
    uart_rx_q = uart_rx_queue;

    // Enable the peripherals used by UART
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));

    // Set GPIO A0 and A1 as UART pins.
    MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
    MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Use the internal 16MHz oscillator as the UART clock source.
    MAP_UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    // Initialize the UART for console I/O.
    UARTStdioConfig(0, 115200, 16000000);

    // Enable RX interrupt only
    MAP_IntEnable(INT_UART0);
    MAP_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    MAP_IntPrioritySet(INT_UART0, 191+1);

    // Register the interrupt
    UARTIntRegister(UART0_BASE, UART_RX_int_handler);

} // End init_console_uart

