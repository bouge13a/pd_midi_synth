/*
 * GPO_pins_looper.h
 *
 *  Created on: Apr 11, 2021
 *      Author: steph
 */

#ifndef GPO_PINS_LOOPER_H_
#define GPO_PINS_LOOPER_H_

#include "board_pin_defs.h"

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

static gpio_pin_t greenled = {
    .name       = "green debug",
    .pin        = GPIO_PIN_3,
    .peripheral = SYSCTL_PERIPH_GPIOF,
    .port       = GPIO_PORTF_AHB_BASE,
    .direction  = GPIO_DIR_MODE_OUT,
};

static gpio_pin_t blueled = {
    .name       = "blue debug",
    .pin        = GPIO_PIN_2,
    .peripheral = SYSCTL_PERIPH_GPIOF,
    .port       = GPIO_PORTF_AHB_BASE,
    .direction  = GPIO_DIR_MODE_OUT,
};

static gpio_pin_t redled = {
    .name       = "red debug",
    .pin        = GPIO_PIN_1,
    .peripheral = SYSCTL_PERIPH_GPIOF,
    .port       = GPIO_PORTF_AHB_BASE,
    .direction  = GPIO_DIR_MODE_OUT,
};

static gpio_pin_t* looper_gpos[] = {
    &greenled,
    &redled,
    &blueled,
};

const gpos_t looper_gpo_info = {
    .gpos     = looper_gpos,
    .num_gpos = sizeof(looper_gpos)/sizeof(looper_gpos[0]),
};

#endif /* GPO_PINS_LOOPER_H_ */
