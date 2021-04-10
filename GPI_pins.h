/*
 * board_pins.h
 *
 *  Created on: Aug 4, 2020
 *      Author: steph
 */

#include "board_pin_defs.h"

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"


static gpio_pin_t enc_bit_0 = {
    .name       = "bit 0",
    .pin        = GPIO_PIN_4,
    .peripheral = SYSCTL_PERIPH_GPIOF,
    .port       = GPIO_PORTF_AHB_BASE,
    .direction  = GPIO_DIR_MODE_IN
};

static gpio_pin_t enc_bit_1 = {
    .name       = "bit 1",
    .pin        = GPIO_PIN_7,
    .peripheral = SYSCTL_PERIPH_GPIOD,
    .port       = GPIO_PORTD_AHB_BASE,
    .direction  = GPIO_DIR_MODE_IN
};

static gpio_pin_t enc_bit_2 = {
    .name       = "bit 2",
    .pin        = GPIO_PIN_4,
    .peripheral = SYSCTL_PERIPH_GPIOC,
    .port       = GPIO_PORTC_AHB_BASE,
    .direction  = GPIO_DIR_MODE_IN
};

static gpio_pin_t enc_bit_3 = {
    .name       = "bit 3",
    .pin        = GPIO_PIN_5,
    .peripheral = SYSCTL_PERIPH_GPIOC,
    .port       = GPIO_PORTC_AHB_BASE,
    .direction  = GPIO_DIR_MODE_IN
};

static gpio_pin_t pressure_sense = {
    .name       = "p sense",
    .pin        = GPIO_PIN_6,
    .peripheral = SYSCTL_PERIPH_GPIOC,
    .port       = GPIO_PORTC_AHB_BASE,
    .direction  = GPIO_DIR_MODE_IN
};

static gpio_pin_t octave_up = {
    .name       = "octave up",
    .pin        = GPIO_PIN_7,
    .peripheral = SYSCTL_PERIPH_GPIOC,
    .port       = GPIO_PORTC_AHB_BASE,
    .direction  = GPIO_DIR_MODE_IN
};

static gpio_pin_t octave_down = {
    .name       = "octave down",
    .pin        = GPIO_PIN_6,
    .peripheral = SYSCTL_PERIPH_GPIOD,
    .port       = GPIO_PORTD_AHB_BASE,
    .direction  = GPIO_DIR_MODE_IN
};


static gpio_pin_t* gpi_pins[] = {
    &enc_bit_0,
    &enc_bit_1,
    &enc_bit_2,
    &enc_bit_3,
    &pressure_sense,
    &octave_up,
    &octave_down,
};

gpis_t gpi_info = {
    .gpis     = gpi_pins,
    .num_gpis = sizeof(gpi_pins)/sizeof(gpi_pins[0])
};


