/*
 * comp_ints.c
 *
 *  Created on: Apr 2, 2021
 *      Author: steph
 */

#include "comp_ints.h"

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <uartstdio.h>

#include "driverlib/inc/hw_ints.h"
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

#include "ADC_task.h"
#include "board_pin_defs.h"
#include "host_uart_task.h"
#include "console_task.h"

void Comp0IntHandler(void);
void Comp1IntHandler(void);

typedef enum {
    LOW,
    MEDIUM,
    HIGH,
}comp_state_e;

typedef struct {
    comp_state_e previous_state;
    uint64_t     low_time;
}comp_state_t;

static comp_state_t comp_states[12];

static uint32_t page_number;

void init_comp_ints(uint32_t page_num) {

    uint32_t idx;

    page_number = page_num;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    for(idx=0; idx<12; idx++) {
        comp_states[idx].previous_state = HIGH;
    }

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC_UP);

    TimerEnable(TIMER0_BASE,
                TIMER_BOTH);

} // End init_comp_ints

static uint32_t get_pad_num(uint32_t status, uint32_t base) {

    uint32_t note = 0;

    assert(status);

    if (base == ADC0_BASE) {

        while (status != 1) {
            status = status >> 1;
            note++;
        }

    } else if (ADC1_BASE == base) {

        note = 8;

        while (status != 1) {
            status = status >> 1;
            note++;
        }

    } else {
        assert(0);
    }

    return note;
}

static void process_interrupt(uint32_t status, uint32_t base) {

    uart_msg_u uart_msg;
    uint32_t pad_num = get_pad_num(status, base);

    switch(comp_states[pad_num].previous_state) {
    case HIGH :

        //Pack the message
        // High to low is a note off
        uart_msg.bitfield.message_type = NOTE_OFF;
        uart_msg.bitfield.pad_num = pad_num;

        // send the message to the teensy
        send_to_host_from_isr(uart_msg);

        if(is_on_screen(page_number)){
            UARTprintf("%d %d %d\n", uart_msg.bitfield.message_type,
                                     uart_msg.bitfield.pad_num,
                                     uart_msg.bitfield.value);
        }

        // Reconfigure the comparator int to LOW_ONCE
        if (ADC0_BASE == base) {
            ADCComparatorConfigure(ADC0_BASE,
                                   pad_num,
                                   ADC_COMP_INT_MID_ONCE);
        } else {
            ADCComparatorConfigure(ADC1_BASE,
                                   pad_num - 8,
                                   ADC_COMP_INT_MID_ONCE);
        }

        comp_states[pad_num].previous_state = LOW;

        break;

    case MEDIUM :

        //Pack the message
        // High to low is a note off
        uart_msg.bitfield.message_type = NOTE_ON;
        uart_msg.bitfield.pad_num = pad_num;

        uart_msg.bitfield.value = 63;

        // send the message to the teensy
        send_to_host_from_isr(uart_msg);

        if(is_on_screen(page_number)){
            UARTprintf("%d %d %d\n", uart_msg.bitfield.message_type,
                                     uart_msg.bitfield.pad_num,
                                     uart_msg.bitfield.value);
        }

        // Reconfigure the comparator int to HIGH_ONCE
        if (ADC0_BASE == base) {
            ADCComparatorConfigure(ADC0_BASE,
                                   pad_num,
                                   ADC_COMP_INT_LOW_ONCE);
        } else {
            ADCComparatorConfigure(ADC1_BASE,
                                   pad_num - 8,
                                   ADC_COMP_INT_LOW_ONCE);
        }

        comp_states[pad_num].previous_state = HIGH;

        break;

    case LOW :

        // Reconfigure the comparator int to HIGH_ONCE
        if (ADC0_BASE == base) {
            ADCComparatorConfigure(ADC0_BASE,
                                   pad_num,
                                   ADC_COMP_INT_HIGH_ONCE);
        } else {
            ADCComparatorConfigure(ADC1_BASE,
                                   pad_num - 8,
                                   ADC_COMP_INT_HIGH_ONCE);
        }

        comp_states[pad_num].previous_state = MEDIUM;

        break;

    default:
        assert(0);
        break;

    }

} // End process_interrupt

void Comp0IntHandler(void) {

    uint32_t status = 0;

    ADCComparatorIntDisable(ADC0_BASE, 0);

    status = ADCComparatorIntStatus(ADC0_BASE);

    process_interrupt(status, ADC0_BASE);

    ADCComparatorIntClear(ADC0_BASE, status);

    ADCComparatorIntEnable(ADC0_BASE, 0);

} // End Comp0IntHandler

void Comp1IntHandler(void) {

    uint32_t status = 0;

    ADCComparatorIntDisable(ADC1_BASE, 1);

    status = ADCComparatorIntStatus(ADC1_BASE);

    process_interrupt(status, ADC1_BASE);

    ADCComparatorIntClear(ADC1_BASE, status);

    ADCComparatorIntEnable(ADC1_BASE, 1);

} // End Comp1IntHandler

void drumpad_drawpage(void){

}
void drumpad_drawdata(void){

}
void drumpad_drawinput(int character) {

}
