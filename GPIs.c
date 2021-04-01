/*
 * buttons.c
 *
 *  Created on: Aug 4, 2020
 *      Author: steph
 */

#ifndef BUTTONS_C_
#define BUTTONS_C_

#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <GPI_pins.h>
#include <GPIs.h>
#include <uartstdio.h>

#include "text_controls.h"
#include "driverlib/inc/hw_gpio.h"
#include "driverlib/inc/hw_types.h"
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"



///////////////////////////////////////////////////////////////////
//                      Buttons
//////////////////////////////////////////////////////////////////

void init_gpis(void) {

    uint32_t idx;

    for(idx=0; idx<gpi_info.num_gpis; idx++) {

        MAP_SysCtlPeripheralEnable(gpi_info.gpis[idx]->peripheral);

        SysCtlGPIOAHBEnable(gpi_info.gpis[idx]->peripheral);

        while(!SysCtlPeripheralReady(gpi_info.gpis[idx]->peripheral));

        // Unlock port so we can change it to a GPIO input
        // Once we have enabled (unlocked) the commit register then re-lock it
        // to prevent further changes.  PF0 is muxed with NMI thus a special case.
        HWREG(gpi_info.gpis[idx]->port + GPIO_O_LOCK) = GPIO_LOCK_KEY;
        HWREG(gpi_info.gpis[idx]->port + GPIO_O_CR) |= gpi_info.gpis[idx]->pin;
        HWREG(gpi_info.gpis[idx]->port + GPIO_O_LOCK) = 0;

        MAP_GPIODirModeSet(gpi_info.gpis[idx]->port,
                           gpi_info.gpis[idx]->pin,
                           gpi_info.gpis[idx]->direction);

        MAP_GPIOPadConfigSet(gpi_info.gpis[idx]->port,
                             gpi_info.gpis[idx]->pin,
                             GPIO_STRENGTH_2MA,
                             GPIO_PIN_TYPE_STD_WPU);
    }

} // End init_buttons

gpio_pin_t* get_gpi_config(const char* name) {

    int idx;

    for(idx=0; idx<gpi_info.num_gpis; idx++) {

        if(0 == strcmp(name, gpi_info.gpis[idx]->name)){
            return gpi_info.gpis[idx];
        }

    }

    assert(0);
    return NULL;
} // End get_button_config

uint32_t read_gpi(gpio_pin_t* config) {
    uint32_t value = MAP_GPIOPinRead(config->port, config->pin);

    return value >= 1 ? 0 : 1;
}

gpis_t get_gpi_struct(void) {
    return gpi_info;
}


#endif /* BUTTONS_C_ */
