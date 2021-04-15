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
#include "driverlib/sysctl.h"
#include "rotary_enc_task.h"
#include "ads1x15_task.h"
#include "midi_buttons_task.h"

static const uint32_t NUM_OF_PADS = 12;
static const uint32_t LOW_REF     = 10;
static const uint32_t HIGH_REF    = 3000;
static const uint32_t IDLE_CYCLES = 10;

static const float    VOLUME100   = 0.040;
static const float    VOLUME75    = 0.010;
static const float    VOLUME50    = 0.005;
static const float    VOLUME25    = 0.001;


typedef enum {
    IDLE_STATE,
    LOW_STATE,
    MID_STATE,
    HIGH_STATE,
}pad_states_e;

typedef struct {
    uint32_t low_value;
    uint32_t low_time;
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

} // End init

static uint32_t get_value(float slope) {

    if (pressure_sense_is_on()){
        return (uint32_t)(get_volume() * 127);
    } else {
        if (slope > VOLUME100) {
            return (uint32_t)(get_volume()*127);
        } else if (slope > VOLUME75) {
            return (uint32_t)(get_volume()*127*0.75);
        } else if (slope > VOLUME50) {
            return (uint32_t)(get_volume()*127*0.50);
        } else if (slope > VOLUME25) {
            return (uint32_t)(get_volume()*127*0.25);
        } else {
            return (uint32_t)(get_volume()*127*0.10);
        }
    }
}

void process_drumpad(uint32_t* adc00values,
                     uint32_t* adc11values,
                     uint32_t adc00time,
                     uint32_t adc11time) {

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
                        pad_states[idx].low_time = adc00time;
                    }
                } else {
                    if (adc11values[idx - 8] > LOW_REF) {
                        pad_states[idx].state = MID_STATE;
                        pad_states[idx].low_value = adc11values[idx-8];
                        pad_states[idx].low_time = adc11time;
                    }
                }
                break;

            case MID_STATE:

                    // Make sure that the note stays high long enough
                    if (idx < 8) {

                        if (adc00values[idx] < HIGH_REF) {
                            pad_states[idx].state = LOW_STATE;
                            pad_states[idx].value_idx = 0;
                            break;
                        } else if (adc00values[idx] > HIGH_REF) {
                            if (pad_states[idx].low_value > adc00values[idx]) {
                                pad_states[idx].state = LOW_STATE;
                                break;
                            }
                            slope = (adc00values[idx] - pad_states[idx].low_value);
                            slope = (float)(slope / ( pad_states[idx].low_time - adc00time));

                            pad_states[idx].state = HIGH_STATE;
                            pad_states[idx].value_idx = 0;

                            uart_msg.bitfield.message_type = NOTE_ON;
                            uart_msg.bitfield.pad_num = idx;
                            uart_msg.bitfield.value = get_value(slope);
                            uart_msg.bitfield.channel = get_channel();

                            if (slope > 0) {
                                if((idx == 0) && (is_on_screen(page_num))) {
                                    sprintf(float_string, "%f", slope);
                                    UARTprintf("%s\n", float_string);
                                }
                                send_to_host(uart_msg);
                            }

                            break;

                        }
                    } else {

                        if (adc11values[idx - 8] < HIGH_REF) {
                            pad_states[idx].state = LOW_STATE;
                            pad_states[idx].value_idx = 0;
                            break;
                        } else if (adc11values[idx] > HIGH_REF) {

                            if (pad_states[idx].low_value > adc11values[idx-8]) {
                                pad_states[idx].state = LOW_STATE;
                                break;
                            }

                            slope = (adc11values[idx-8] - pad_states[idx].low_value);
                            slope = slope / ((float)( pad_states[idx].low_time - adc11time ));

                            pad_states[idx].state = HIGH_STATE;
                            pad_states[idx].value_idx = 0;

                            uart_msg.bitfield.message_type = NOTE_ON;
                            uart_msg.bitfield.pad_num = idx;
                            uart_msg.bitfield.value = get_value(slope);
                            uart_msg.bitfield.channel = get_channel();

                            if (slope > 0) {
                                send_to_host(uart_msg);
                            }

                            break;
                        }
                    }

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
