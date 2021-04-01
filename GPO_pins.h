/*
 * LED_pins.h
 *
 *  Created on: Aug 6, 2020
 *      Author: steph
 */

#ifndef LED_PINS_H_
#define LED_PINS_H_


#include "board_pin_defs.h"

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

static gpio_pin_t green_led = {
    .name       = "green debug",
    .pin        = GPIO_PIN_3,
    .peripheral = SYSCTL_PERIPH_GPIOF,
    .port       = GPIO_PORTF_AHB_BASE,
    .direction  = GPIO_DIR_MODE_OUT,
};

static gpio_pin_t blue_led = {
    .name       = "blue debug",
    .pin        = GPIO_PIN_2,
    .peripheral = SYSCTL_PERIPH_GPIOF,
    .port       = GPIO_PORTF_AHB_BASE,
    .direction  = GPIO_DIR_MODE_OUT,
};

static gpio_pin_t red_led = {
    .name       = "red debug",
    .pin        = GPIO_PIN_1,
    .peripheral = SYSCTL_PERIPH_GPIOF,
    .port       = GPIO_PORTF_AHB_BASE,
    .direction  = GPIO_DIR_MODE_OUT,
};

static gpio_pin_t* gpos[] = {
    &green_led,
    &red_led,
    &blue_led,
};

const gpos_t gpo_info = {
    .gpos     = gpos,
    .num_gpos = sizeof(gpos)/sizeof(gpos[0]),
};

#endif /* LED_PINS_H_ */
