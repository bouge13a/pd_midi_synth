/*
 * looper_effects.c
 *
 *  Created on: Apr 20, 2021
 *      Author: steph
 */

#include <assert.h>

#include "looper_effects.h"
#include "host_uart_task.h"

#include <stdlib.h>
#include <assert.h>

static const uint32_t EFFECT_INCREMENT = 100;

static const uint32_t EFFECT_OFFSET = 102;
static const uint32_t MASTER_OFFSET = 110;

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
    uart_msg_u uart_msg;

    for (index=0; index<12; index++) {
        if (index < 8) {

            if(abs(adc00values[index] - effects[index].last_value) > EFFECT_INCREMENT) {

                uart_msg.bitfield.message_type = EFFECT;
                uart_msg.bitfield.pad_num = EFFECT_OFFSET + index;
                uart_msg.bitfield.value = (adc00values[index]/4096.0)*256;

                send_to_host(uart_msg);

            }

            effects[index].last_value = adc00values[index];

        } else {

            if(abs(adc11values[index] - effects[index].last_value) > EFFECT_INCREMENT) {
                uart_msg.bitfield.message_type = EFFECT;
                uart_msg.bitfield.pad_num = MASTER_OFFSET + index;
                uart_msg.bitfield.value = (adc11values[index]/4096.0)*256;

                send_to_host(uart_msg);
            }

            effects[index].last_value = adc11values[index];

        }
    }

} // End process_effects
