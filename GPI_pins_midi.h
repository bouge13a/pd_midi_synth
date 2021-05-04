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


static gpio_pin_t chan_up = {
    .name       = "channel up",
    .pin        = GPIO_PIN_4,
    .peripheral = SYSCTL_PERIPH_GPIOF,
    .port       = GPIO_PORTF_AHB_BASE,
    .direction  = GPIO_DIR_MODE_IN
};

static gpio_pin_t chan_down = {
    .name       = "channel down",
    .pin        = GPIO_PIN_7,
    .peripheral = SYSCTL_PERIPH_GPIOD,
    .port       = GPIO_PORTD_AHB_BASE,
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


static gpio_pin_t* midi_gpi_pins[] = {
    &chan_up,
    &chan_down,
    &pressure_sense,
    &octave_up,
    &octave_down,
};

gpis_t midi_gpi_info = {
    .gpis     = midi_gpi_pins,
    .num_gpis = sizeof(midi_gpi_pins)/sizeof(midi_gpi_pins[0])
};



