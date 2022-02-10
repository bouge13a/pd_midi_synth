/*
 * utils.h
 *
 *  Created on: Aug 4, 2020
 *      Author: steph
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>
#include "board_pin_defs.h"

typedef union {
    uint16_t value;
    uint8_t bytes[2];
}value16_t;

typedef enum {
    BUTTON_UP,
    BUTTON_DOWN,
}button_state_e;


typedef struct {
    gpio_pin_t* pin;
    button_state_e last_button_state;
}button_states_t;


//void int_to_bin_string(uint32_t integer, uint8_t* string);


#endif /* UTILS_H_ */
