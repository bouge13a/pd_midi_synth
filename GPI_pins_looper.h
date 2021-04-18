/*
 * GPI_pins_looper.h
 *
 *  Created on: Apr 11, 2021
 *      Author: steph
 */

#ifndef GPI_PINS_LOOPER_H_
#define GPI_PINS_LOOPER_H_

#include "board_pin_defs.h"

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

static gpio_pin_t loop_0 = {
    .name       = "loop 0",
    .pin        = GPIO_PIN_0,
    .peripheral = SYSCTL_PERIPH_GPIOE,
    .port       = GPIO_PORTE_AHB_BASE,
    .direction  = GPIO_DIR_MODE_IN
};

static gpio_pin_t loop_1 = {
    .name       = "loop 1",
    .pin        = GPIO_PIN_6,
    .peripheral = SYSCTL_PERIPH_GPIOB,
    .port       = GPIO_PORTB_AHB_BASE,
    .direction  = GPIO_DIR_MODE_IN
};

static gpio_pin_t loop_2 = {
    .name       = "loop 2",
    .pin        = GPIO_PIN_3,
    .peripheral = SYSCTL_PERIPH_GPIOB,
    .port       = GPIO_PORTB_AHB_BASE,
    .direction  = GPIO_DIR_MODE_IN
};

static gpio_pin_t loop_3 = {
    .name       = "loop 3",
    .pin        = GPIO_PIN_4,
    .peripheral = SYSCTL_PERIPH_GPIOC,
    .port       = GPIO_PORTC_AHB_BASE,
    .direction  = GPIO_DIR_MODE_IN
};

static gpio_pin_t loop_4 = {
    .name       = "loop 4",
    .pin        = GPIO_PIN_5,
    .peripheral = SYSCTL_PERIPH_GPIOC,
    .port       = GPIO_PORTC_AHB_BASE,
    .direction  = GPIO_DIR_MODE_IN
};

static gpio_pin_t loop_5 = {
    .name       = "loop 5",
    .pin        = GPIO_PIN_6,
    .peripheral = SYSCTL_PERIPH_GPIOC,
    .port       = GPIO_PORTC_AHB_BASE,
    .direction  = GPIO_DIR_MODE_IN
};

static gpio_pin_t loop_6 = {
    .name       = "loop 6",
    .pin        = GPIO_PIN_7,
    .peripheral = SYSCTL_PERIPH_GPIOC,
    .port       = GPIO_PORTC_AHB_BASE,
    .direction  = GPIO_DIR_MODE_IN
};

static gpio_pin_t loop_7 = {
    .name       = "loop 7",
    .pin        = GPIO_PIN_6,
    .peripheral = SYSCTL_PERIPH_GPIOD,
    .port       = GPIO_PORTD_AHB_BASE,
    .direction  = GPIO_DIR_MODE_IN
};


static gpio_pin_t* looper_gpi_pins[] = {
    &loop_0,
    &loop_1,
    &loop_2,
    &loop_3,
    &loop_4,
    &loop_5,
    &loop_6,
    &loop_7,
};

gpis_t looper_gpi_info = {
    .gpis     = looper_gpi_pins,
    .num_gpis = sizeof(looper_gpi_pins)/sizeof(looper_gpi_pins[0])
};



#endif /* GPI_PINS_LOOPER_H_ */
