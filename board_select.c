/*
 * board_select.c
 *
 *  Created on: Apr 11, 2021
 *      Author: steph
 */

#include <stdint.h>
#include <stdbool.h>

#include "board_select.h"

#include "driverlib/inc/hw_gpio.h"
#include "driverlib/inc/hw_types.h"
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"

static board_type_e board_type;

void init_board_select(void) {
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOB);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));

    // Unlock port so we can change it to a GPIO input
    // Once we have enabled (unlocked) the commit register then re-lock it
    // to prevent further changes.  PF0 is muxed with NMI thus a special case.
    HWREG(GPIO_PORTB_AHB_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTB_AHB_BASE + GPIO_O_CR) |= GPIO_PIN_2;
    HWREG(GPIO_PORTB_AHB_BASE + GPIO_O_LOCK) = 0;

    MAP_GPIODirModeSet(GPIO_PORTB_AHB_BASE,
                       GPIO_PIN_2,
                       GPIO_DIR_MODE_IN);

    MAP_GPIOPadConfigSet(GPIO_PORTB_AHB_BASE,
                         GPIO_PIN_2,
                         GPIO_STRENGTH_2MA,
                         GPIO_PIN_TYPE_STD_WPU);

    if( 0 == MAP_GPIOPinRead(GPIO_PORTB_AHB_BASE, GPIO_PIN_2)) {
        board_type = MIDI_CONTROLLER;
    } else {
        board_type = LOOPER;
    }
}

board_type_e get_board_type(void){
    return board_type;
}
