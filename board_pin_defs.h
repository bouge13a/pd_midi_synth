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
    uint32_t    pin;
    uint32_t    peripheral;
    uint32_t    port;
    uint32_t    direction;
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
    uint32_t gpio_peripheral;
    uint32_t gpio_base;
    uint32_t gpio_pin;
    uint32_t sequencer;
    uint32_t channel;
    uint32_t step;
}adc_pin_t;

typedef struct {
    adc_pin_t** adc_pins;
    uint32_t    num_adc_pins;
}adc_pins_t;

#endif /* BOARD_PIN_DEFS_H_ */
