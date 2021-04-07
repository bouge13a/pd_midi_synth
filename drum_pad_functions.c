/*
 * drum_pad_functions.c
 *
 *  Created on: Apr 5, 2021
 *      Author: steph
 */
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <uartstdio.h>
#include <stdio.h>

#include "drum_pad_functions.h"
#include "host_uart_task.h"
#include "console_task.h"


#include "driverlib/timer.h"
#include "driverlib/inc/hw_ints.h"
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/sysctl.h"

static const uint32_t NUM_OF_PADS = 12;
static const uint32_t LOW_REF = 50;
static const uint32_t SAMPLES_TO_WAIT = 100;
static const uint32_t IDLE_CYCLES = 1000;

typedef enum {
    IDLE_STATE,
    LOW_STATE,
    MID_STATE,
    HIGH_STATE,
}pad_states_e;

typedef struct {
    uint32_t low_value;
    uint64_t low_time;
    pad_states_e state;
    uint32_t value_idx;
    uint32_t idle_idx;
}pad_states_t;

pad_states_t pad_states[12];

static uint32_t page_num;

static char float_string[20];


void init_drumpad(uint32_t page_number) {

    page_num = page_number;

    uint32_t idx;

    for(idx=0; idx < NUM_OF_PADS; idx++){
        pad_states[idx].state = LOW_STATE;
        pad_states[idx].value_idx = 0;
    }

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC_UP);

    TimerPrescaleSet(TIMER0_BASE, TIMER_BOTH, 1000);

    TimerEnable(TIMER0_BASE, TIMER_BOTH);

}

void process_drumpad(uint32_t* adc00values, uint32_t* adc11values) {

    uint32_t idx;
    uart_msg_u uart_msg;
    float slope = 0.0;

    for(idx=0; idx < NUM_OF_PADS; idx++){
        switch (pad_states[idx].state) {

            case IDLE_STATE :
                if (pad_states[idx].idle_idx >= IDLE_CYCLES) {
                    pad_states[idx].state = LOW_STATE;
                    pad_states[idx].idle_idx = 0;
                }
                pad_states[idx].idle_idx++;
                break;

            case LOW_STATE:
                if (idx < 8) {
                    if (adc00values[idx] > LOW_REF) {
                        pad_states[idx].state = MID_STATE;
                        pad_states[idx].low_value = adc00values[idx];
                        pad_states[idx].low_time = TimerValueGet(TIMER0_BASE, TIMER_BOTH);
                    }
                } else {
                    if (adc11values[idx - 8] > LOW_REF) {
                        pad_states[idx].state = MID_STATE;
                        pad_states[idx].low_value = adc11values[idx-8];
                        pad_states[idx].low_time = TimerValueGet(TIMER0_BASE, TIMER_BOTH);
                    }
                }
                break;

            case MID_STATE:

                // Make sure that the note stays high long enough
                if (idx < 8) {

                    if (adc00values[idx] < LOW_REF) {
                        pad_states[idx].state = LOW_STATE;
                        pad_states[idx].value_idx = 0;
                        break;
                    }
                } else {

                    if (adc11values[idx - 8] < LOW_REF) {
                        pad_states[idx].state = LOW_STATE;
                        pad_states[idx].value_idx = 0;
                        break;
                    }
                }

                if(pad_states[idx].value_idx>=SAMPLES_TO_WAIT) {
                    pad_states[idx].state = HIGH_STATE;
                    pad_states[idx].value_idx = 0;

                    uart_msg.bitfield.message_type = NOTE_ON;
                    uart_msg.bitfield.pad_num = idx;

                    if (idx < 8) {
                        slope = (adc00values[idx] - pad_states[idx].low_value);
                        slope = slope / ((float)(TimerValueGet(TIMER0_BASE, TIMER_BOTH) - pad_states[idx].low_time));
                    } else {
                        slope = (adc11values[idx - 8] - pad_states[idx].low_value);
                    }

                    uart_msg.bitfield.value = 63;

                    if (slope > 0) {
                        if((idx == 0) && (is_on_screen(page_num))) {
                            sprintf(float_string, "%f", slope*1000000);
                            UARTprintf("%s\n", float_string);
                        }
                        send_to_host(uart_msg);
                    }

                    break;
                }
                pad_states[idx].value_idx++;

                break;

            case HIGH_STATE:
                if (idx < 8) {
                    if(adc00values[idx] < LOW_REF) {
                        uart_msg.bitfield.message_type = NOTE_OFF;
                        uart_msg.bitfield.pad_num = idx;
                        pad_states[idx].state = IDLE_STATE;
                        send_to_host(uart_msg);
                        break;
                    }
                } else {
                    if(adc11values[idx-8] < LOW_REF) {
                        uart_msg.bitfield.message_type = NOTE_OFF;
                        uart_msg.bitfield.pad_num = idx;
                        pad_states[idx].state = IDLE_STATE;
                        send_to_host(uart_msg);
                        break;
                    }
                }
                break;
            default :
                assert(0);
                break;
        }
    }
}

void slope_drawpage(void) {

}
void slope_drawdata(void) {

}
void slope_drawinput(int character) {

}
