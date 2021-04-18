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

static gpio_pin_t led_0 = {
    .name       = "led 0",
    .pin        = GPIO_PIN_7,
    .peripheral = SYSCTL_PERIPH_GPIOD,
    .port       = GPIO_PORTD_AHB_BASE,
    .direction  = GPIO_DIR_MODE_OUT,
};

static gpio_pin_t led_1 = {
    .name       = "led 1",
    .pin        = GPIO_PIN_4,
    .peripheral = SYSCTL_PERIPH_GPIOF,
    .port       = GPIO_PORTF_AHB_BASE,
    .direction  = GPIO_DIR_MODE_OUT,
};

static gpio_pin_t led_2 = {
    .name       = "led 2",
    .pin        = GPIO_PIN_6,
    .peripheral = SYSCTL_PERIPH_GPIOB,
    .port       = GPIO_PORTB_AHB_BASE,
    .direction  = GPIO_DIR_MODE_OUT,
};

static gpio_pin_t led_3 = {
    .name       = "led 3",
    .pin        = GPIO_PIN_7,
    .peripheral = SYSCTL_PERIPH_GPIOB,
    .port       = GPIO_PORTB_AHB_BASE,
    .direction  = GPIO_DIR_MODE_OUT,
};

static gpio_pin_t led_4 = {
    .name       = "led 4",
    .pin        = GPIO_PIN_0,
    .peripheral = SYSCTL_PERIPH_GPIOF,
    .port       = GPIO_PORTF_AHB_BASE,
    .direction  = GPIO_DIR_MODE_OUT,
};

static gpio_pin_t led_5 = {
    .name       = "led 5",
    .pin        = GPIO_PIN_2,
    .peripheral = SYSCTL_PERIPH_GPIOB,
    .port       = GPIO_PORTB_AHB_BASE,
    .direction  = GPIO_DIR_MODE_OUT,
};

static gpio_pin_t led_6 = {
    .name       = "led 6",
    .pin        = GPIO_PIN_0,
    .peripheral = SYSCTL_PERIPH_GPIOF,
    .port       = GPIO_PORTF_AHB_BASE,
    .direction  = GPIO_DIR_MODE_OUT,
};

static gpio_pin_t led_7 = {
    .name       = "led 7",
    .pin        = GPIO_PIN_7,
    .peripheral = SYSCTL_PERIPH_GPIOB,
    .port       = GPIO_PORTB_AHB_BASE,
    .direction  = GPIO_DIR_MODE_OUT,
};

static gpio_pin_t* looper_gpos[] = {
    &greenled,
    &redled,
    &blueled,
    &led_0,
    &led_1,
    &led_2,
    &led_3,
    &led_4,
    &led_5,
    &led_6,
    &led_7,
};

const gpos_t looper_gpo_info = {
    .gpos     = looper_gpos,
    .num_gpos = sizeof(looper_gpos)/sizeof(looper_gpos[0]),
};

#endif /* GPO_PINS_LOOPER_H_ */
