/*
 * ADC_pins.h
 *
 *  Created on: Aug 5, 2020
 *      Author: steph
 */

#ifndef ADC_PINS_H_
#define ADC_PINS_H_

#include <stdbool.h>
#include <stdint.h>
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#include "board_pin_defs.h"

static adc_pin_t pad0 = {
    .name            = "pad 0",
    .gpio_peripheral = SYSCTL_PERIPH_GPIOE,
    .gpio_base       = GPIO_PORTE_AHB_BASE,
    .gpio_pin        = GPIO_PIN_3,
    .sequencer       = 0,
    .channel         = 0,
    .step            = 0,
};

static adc_pin_t pad1 = {
    .name            = "pad 1",
    .gpio_peripheral = SYSCTL_PERIPH_GPIOE,
    .gpio_base       = GPIO_PORTE_AHB_BASE,
    .gpio_pin        = GPIO_PIN_2,
    .sequencer       = 0,
    .channel         = 1,
    .step            = 1,
};

static adc_pin_t pad2 = {
    .name            = "pad 2",
    .gpio_peripheral = SYSCTL_PERIPH_GPIOE,
    .gpio_base       = GPIO_PORTE_AHB_BASE,
    .gpio_pin        = GPIO_PIN_1,
    .sequencer       = 0,
    .channel         = 2,
    .step            = 2,
};

static adc_pin_t pad3 = {
    .name            = "pad 3",
    .gpio_peripheral = SYSCTL_PERIPH_GPIOE,
    .gpio_base       = GPIO_PORTE_AHB_BASE,
    .gpio_pin        = GPIO_PIN_0,
    .sequencer       = 0,
    .channel         = 3,
    .step            = 3,
};

static adc_pin_t pad4 = {
    .name            = "pad 4",
    .gpio_peripheral = SYSCTL_PERIPH_GPIOD,
    .gpio_base       = GPIO_PORTD_AHB_BASE,
    .gpio_pin        = GPIO_PIN_3,
    .sequencer       = 0,
    .channel         = 4,
    .step            = 4,
};

static adc_pin_t pad5 = {
    .name            = "pad 5",
    .gpio_peripheral = SYSCTL_PERIPH_GPIOD,
    .gpio_base       = GPIO_PORTD_AHB_BASE,
    .gpio_pin        = GPIO_PIN_2,
    .sequencer       = 0,
    .channel         = 5,
    .step            = 5,
};

static adc_pin_t pad6 = {
    .name            = "pad 6",
    .gpio_peripheral = SYSCTL_PERIPH_GPIOD,
    .gpio_base       = GPIO_PORTD_AHB_BASE,
    .gpio_pin        = GPIO_PIN_1,
    .sequencer       = 1,
    .channel         = 6,
    .step            = 0,
};

static adc_pin_t pad7 = {
    .name            = "pad 7",
    .gpio_peripheral = SYSCTL_PERIPH_GPIOD,
    .gpio_base       = GPIO_PORTD_AHB_BASE,
    .gpio_pin        = GPIO_PIN_0,
    .sequencer       = 1,
    .channel         = 7,
    .step            = 1,
};

static adc_pin_t pad8 = {
    .name            = "pad 8",
    .gpio_peripheral = SYSCTL_PERIPH_GPIOE,
    .gpio_base       = GPIO_PORTE_AHB_BASE,
    .gpio_pin        = GPIO_PIN_5,
    .sequencer       = 1,
    .channel         = 8,
    .step            = 2,
};

static adc_pin_t pad9 = {
    .name            = "pad 9",
    .gpio_peripheral = SYSCTL_PERIPH_GPIOE,
    .gpio_base       = GPIO_PORTE_AHB_BASE,
    .gpio_pin        = GPIO_PIN_4,
    .sequencer       = 1,
    .channel         = 9,
    .step            = 3,
};

static adc_pin_t pad10 = {
    .name            = "pad 10",
    .gpio_peripheral = SYSCTL_PERIPH_GPIOB,
    .gpio_base       = GPIO_PORTB_AHB_BASE,
    .gpio_pin        = GPIO_PIN_4,
    .sequencer       = 1,
    .channel         = 10,
    .step            = 4,
};

static adc_pin_t pad11 = {
    .name            = "pad 11",
    .gpio_peripheral = SYSCTL_PERIPH_GPIOB,
    .gpio_base       = GPIO_PORTB_AHB_BASE,
    .gpio_pin        = GPIO_PIN_5,
    .sequencer       = 1,
    .channel         = 11,
    .step            = 5,
};

static adc_pin_t* adc_pins[] = {
    &pad0,
    &pad1,
    &pad2,
    &pad3,
    &pad4,
    &pad5,
    &pad6,
    &pad7,
    &pad8,
    &pad9,
    &pad10,
    &pad11,
};

adc_pins_t adc_pin_struct = {
    .adc_pins     = adc_pins,
    .num_adc_pins = sizeof(adc_pins)/sizeof(adc_pins[0]),
};

#endif /* ADC_PINS_H_ */
