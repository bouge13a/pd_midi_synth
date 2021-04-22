/*
 * midi_buttons_task.c
 *
 *  Created on: Apr 15, 2021
 *      Author: steph
 */

#include "FreeRTOS.h"
#include "task.h"

#include "midi_buttons_task.h"
#include "GPIs.h"
#include "board_pin_defs.h"
#include "assert.h"

static uint32_t octave;

static gpio_pin_t* pressure_sense;

typedef enum {
    BUTTON_UP,
    BUTTON_DOWN,
}button_state_e;

enum {
    OCTAVE_UP,
    OCTAVE_DOWN,
};

typedef struct {
    gpio_pin_t* pin;
    button_state_e last_button_state;
}button_states_t;

button_states_t button_states[2];

void init_midi_buttons(void) {

    octave = 63;

    pressure_sense = get_gpi_config("p sense");

    button_states[OCTAVE_UP].pin = get_gpi_config("octave up");
    button_states[OCTAVE_UP].last_button_state = BUTTON_UP;

    button_states[OCTAVE_DOWN].pin = get_gpi_config("octave down");
    button_states[OCTAVE_DOWN].last_button_state = BUTTON_UP;

} // End init_midi_buttons

void midi_buttons_task(void* parm) {

    uint32_t index;

    while(1) {

        for(index=0; index<2; index++) {
            switch(button_states[index].last_button_state) {
            case BUTTON_UP :
                if(1 == read_gpi(button_states[index].pin)) {
                    if (index == OCTAVE_UP) {

                        if (octave < 115) {
                            octave = octave + 12;
                        }

                    } else {

                        if(octave >= 12) {
                            octave = octave - 12;
                        }

                    }

                    button_states[index].last_button_state = BUTTON_DOWN;
                }
                break;

            case BUTTON_DOWN :

                if(0 == read_gpi(button_states[index].pin)) {
                    button_states[index].last_button_state = BUTTON_UP;
                }

                break;

            default :
                break;
            }
        }

        vTaskDelay(50);

    }

} // End midi_buttons_task

uint32_t get_octave(void) {
    return octave;
}

bool pressure_sense_is_on(void) {
    if (read_gpi(pressure_sense)) {
        return true;
    } else {
        return false;
    }
} // End pressure_sense_is_on
