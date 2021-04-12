/*
 * board_pin_defs.h
 *
 *  Created on: Aug 4, 2020
 *      Author: steph
 */

#ifndef BOARD_PIN_DEFS_H_
#define BOARD_PIN_DEFS_H_

#include <stdint.h>

typedef struct  {
    const char* name;
    const uint32_t pin;
    const uint32_t peripheral;
    const uint32_t port;
    const uint32_t direction;
} gpio_pin_t;

typedef struct {
    gpio_pin_t** gpis;
    uint32_t num_gpis;
}gpis_t;

typedef struct {
    gpio_pin_t** gpos;
    uint32_t num_gpos;
}gpos_t;

typedef struct {
    const char* name;
    const uint32_t gpio_peripheral;
    const uint32_t gpio_base;
    const uint32_t gpio_pin;
    const uint32_t sequencer;
    const uint32_t channel;
    const uint32_t step;
    const uint32_t adc_base;
    const uint32_t comparator;
}adc_pin_t;

typedef struct {
    adc_pin_t** adc_pins;
    uint32_t    num_adc_pins;
}adc_pins_t;

#endif /* BOARD_PIN_DEFS_H_ */
