/*
 * LEDs.c
 *
 *  Created on: Aug 6, 2020
 *      Author: steph
 */

#include <string.h>
#include <assert.h>
#include <GPOs.h>
#include "GPO_pins_midi.h"
#include "GPO_pins_looper.h"

#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "board_select.h"

gpos_t gpo_info;

void init_gpos(void) {

    uint32_t idx;

    if(MIDI_CONTROLLER == get_board_type()) {
        gpo_info = midi_gpo_info;
    } else {
        gpo_info = looper_gpo_info;
    }

    if (0 != gpo_info.num_gpos) {

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
