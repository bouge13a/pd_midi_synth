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

static adc_pin_t joystick_x = {
    .name            = "pad 0",
    .gpio_peripheral = SYSCTL_PERIPH_GPIOE,
    .gpio_base       = GPIO_PORTB_AHB_BASE,
    .gpio_pin        = GPIO_PIN_3,
    .sequencer       = 0,
    .channel         = 11,
    .step            = 0,
};

static adc_pin_t joystick_y = {
    .name            = "joystick y",
    .gpio_peripheral = SYSCTL_PERIPH_GPIOD,
    .gpio_base       = GPIO_PORTD_AHB_BASE,
    .gpio_pin        = GPIO_PIN_3,
    .sequencer       = 0,
    .channel         = 4,
    .step            = 1,
};

static adc_pin_t accel_x = {
    .name            = "accel x",
    .gpio_peripheral = SYSCTL_PERIPH_GPIOD,
    .gpio_base       = GPIO_PORTD_AHB_BASE,
    .gpio_pin        = GPIO_PIN_0,
    .sequencer       = 0,
    .channel         = 7,
    .step            = 2,
};

static adc_pin_t accel_y = {
    .name            = "accel y",
    .gpio_peripheral = SYSCTL_PERIPH_GPIOD,
    .gpio_base       = GPIO_PORTD_AHB_BASE,
    .gpio_pin        = GPIO_PIN_1,
    .sequencer       = 0,
    .channel         = 6,
    .step            = 3,
};

static adc_pin_t accel_z = {
    .name            = "accel z",
    .gpio_peripheral = SYSCTL_PERIPH_GPIOD,
    .gpio_base       = GPIO_PORTD_AHB_BASE,
    .gpio_pin        = GPIO_PIN_2,
    .sequencer       = 0,
    .channel         = 5,
    .step            = 4,
};

static adc_pin_t* adc_pins[] = {
    &joystick_x,
    &joystick_y,
    &accel_x,
    &accel_y,
    &accel_z,
};

adc_pins_t adc_pin_struct = {
    .adc_pins     = adc_pins,
    .num_adc_pins = sizeof(adc_pins)/sizeof(adc_pins[0]),
};

#endif /* ADC_PINS_H_ */
