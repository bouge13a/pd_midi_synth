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

static gpio_pin_t* looper_gpi_pins[] = {

};

gpis_t looper_gpi_info = {
    .gpis     = looper_gpi_pins,
    .num_gpis = sizeof(looper_gpi_pins)/sizeof(looper_gpi_pins[0])
};



#endif /* GPI_PINS_LOOPER_H_ */
