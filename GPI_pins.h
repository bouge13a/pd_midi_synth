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

static gpio_pin_t* gpi_pins[] = {

};

gpis_t gpi_info = {
    .gpis     = gpi_pins,
    .num_gpis = sizeof(gpi_pins)/sizeof(gpi_pins[0])
};


