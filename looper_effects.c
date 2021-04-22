/*
 * looper_effects.c
 *
 *  Created on: Apr 20, 2021
 *      Author: steph
 */

#include <assert.h>

#include "looper_effects.h"
#include "usb_hid_task.h"

#include <stdlib.h>
#include <assert.h>

static const uint32_t EFFECT_INCREMENT = 100;

typedef struct {
    uint32_t last_value;
} effect_state_t;

static effect_state_t effects[12];

void init_looper_effects(void) {

    uint32_t index;

    for(index=0; index<12; index++) {
        effects[index].last_value = 0;
    }

} // ENd init_effects

void process_effects(uint32_t* adc00values,
                     uint32_t* adc11values) {

    assert(adc00values);
    assert(adc11values);

    uint32_t index;
    usb_hid_msg_u msg;

    for (index=0; index<12; index++) {
        if (index < 8) {

            if(abs(adc00values[index] - effects[index].last_value) > EFFECT_INCREMENT) {
                msg.bitfield.message_type = EFFECT_MSG;
                msg.bitfield.ctrl_num = index;
                msg.bitfield.value = (adc00values[index]/4096.0)*256;

                usb_hid_send(msg);

            }

            effects[index].last_value = adc00values[index];

        } else {

            if(abs(adc11values[index] - effects[index].last_value) > EFFECT_INCREMENT) {
                msg.bitfield.message_type = EQ_MSG;
                msg.bitfield.ctrl_num = index;
                msg.bitfield.value = (adc11values[index]/4096.0)*256;

                usb_hid_send(msg);
            }

            effects[index].last_value = adc11values[index];

        }
    }

} // End process_effects
