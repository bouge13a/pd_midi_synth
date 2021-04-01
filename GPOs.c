/*
 * LEDs.c
 *
 *  Created on: Aug 6, 2020
 *      Author: steph
 */

#include <string.h>
#include <assert.h>
#include <GPOs.h>
#include "GPO_pins.h"

#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

void init_gpos(void) {

    uint32_t idx;

    for(idx=0; idx<gpo_info.num_gpos; idx++) {

        MAP_SysCtlPeripheralEnable(gpo_info.gpos[idx]->peripheral);

        SysCtlGPIOAHBEnable(gpo_info.gpos[idx]->peripheral);

        MAP_GPIODirModeSet(gpo_info.gpos[idx]->port,
                           gpo_info.gpos[idx]->pin,
                           gpo_info.gpos[idx]->direction);

        MAP_GPIOPadConfigSet(gpo_info.gpos[idx]->port,
                             gpo_info.gpos[idx]->pin,
                             GPIO_STRENGTH_12MA,
                             GPIO_PIN_TYPE_STD);
    }
} // End init_leds

gpio_pin_t* get_gpo_config(const char* name) {

    int idx;

    for(idx=0; idx<gpo_info.num_gpos; idx++) {

        if(0 == strcmp(name, gpo_info.gpos[idx]->name)){
            return gpo_info.gpos[idx];
        }

    }

    assert(0);
    return 0;
} // End get_button_config

void set_gpo(gpio_pin_t* config, uint32_t value) {

    if(value > 0) {
        MAP_GPIOPinWrite(config->port,
                         config->pin,
                         config->pin);
    } else {
        MAP_GPIOPinWrite(config->port,
                         config->pin,
                         0);
    }

} // End set_led

uint32_t get_gpo(gpio_pin_t* config) {
    if (0 == MAP_GPIOPinRead(config->port, config->pin)) {
        return 0;
    } else {
        return 1;
    }
}
