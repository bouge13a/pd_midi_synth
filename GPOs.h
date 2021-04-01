/*
 * LEDs.h
 *
 *  Created on: Aug 6, 2020
 *      Author: steph
 */

#ifndef LEDS_H_
#define LEDS_H_

#include <stdint.h>
#include "board_pin_defs.h"

void init_gpos(void);

gpio_pin_t* get_gpo_config(const char* name);

void set_gpo(gpio_pin_t* config, uint32_t value);

uint32_t get_gpo(gpio_pin_t* config);

#endif /* LEDS_H_ */
