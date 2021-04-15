/*
 * midi_knobs_functions.c
 *
 *  Created on: Apr 14, 2021
 *      Author: steph
 */

#include <assert.h>
#include "midi_knobs_functions.h"
#include "host_uart_task.h"
#include "rotary_enc_task.h"

typedef enum {
    KNOB_SEND,
    KNOB_IDLE,
}midi_knob_state_e;

typedef struct {
    midi_knob_state_e knob_state;
    uint16_t last_value;
}midi_knob_t;

midi_knob_t midi_knobs[4];

void init_midi_knobs(void) {
    uint32_t index;

    for(index=0; index<4; index++) {
        midi_knobs[index].knob_state = KNOB_IDLE;
        midi_knobs[index].last_value = 0;
    }
} // End init_midi_knobs

void process_midi_knobs(volatile value16_t* knobs) {

    uart_msg_u host_message;

    uint32_t index;

    for(index=0; index<4; index++) {
        switch(midi_knobs[index].knob_state) {
        case KNOB_IDLE:
            if (midi_knobs[index].last_value - knobs[index].value > 20 || midi_knobs[index].last_value - knobs[index].value < -20 ) {
                midi_knobs[index].knob_state = KNOB_SEND;
            }
            break;
        case KNOB_SEND:

            host_message.bitfield.channel = get_channel();

            if (index == 0) {
                host_message.bitfield.message_type = KNOB1;
            } else if (index == 1) {
                host_message.bitfield.message_type = KNOB2;
            } else if (index == 2) {
                host_message.bitfield.message_type = KNOB3;
            } else if (index == 3) {
                host_message.bitfield.message_type = KNOB4;
            } else {
                assert(0);
            }

            host_message.bitfield.pad_num = 0;
            host_message.bitfield.value = knobs[index].value;

            send_to_host(host_message);
            midi_knobs[index].knob_state = KNOB_IDLE;
            midi_knobs[index].last_value = knobs[index].value;

            break;
        default :
            assert(0);
            break;
        }
    }

} // End process_midi_knobs
