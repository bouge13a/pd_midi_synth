/*
 * midi_channel_select.c
 *
 *  Created on: May 4, 2021
 *      Author: steph
 */


#include "midi_channel_select.h"

#include "FreeRTOS.h"
#include "task.h"

#include <assert.h>

#include "midi_channel_select.h"
#include "board_pin_defs.h"
#include "GPIs.h"
#include "I2C_task.h"

#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/ssi.h"
#include "utils.h"

static const uint8_t ZERO_SEG   = 0b01111110;
static const uint8_t ONE_SEG    = 0b00001010;
static const uint8_t TWO_SEG    = 0b10110110;
static const uint8_t THREE_SEG  = 0b10011110;
static const uint8_t FOUR_SEG   = 0b11001010;
static const uint8_t FIVE_SEG   = 0b11011100;
static const uint8_t SIX_SEG    = 0b11111100;
static const uint8_t SEVEN_SEG  = 0b00001110;
static const uint8_t EIGHT_SEG  = 0b11111110;
static const uint8_t NINE_SEG   = 0b11011110;

static button_states_t button_states[2];

static uint32_t channel = 0;

enum {
    CHANNEL_UP   = 0,
    CHANNEL_DOWN = 1,
};

void init_midi_channel_select(void) {

    uint32_t dummy_data;

    button_states[CHANNEL_UP].pin = get_gpi_config("channel up");
    button_states[CHANNEL_UP].last_button_state = BUTTON_UP;

    button_states[CHANNEL_DOWN].pin = get_gpi_config("channel down");
    button_states[CHANNEL_DOWN].last_button_state = BUTTON_UP;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);

    // Configure the GPIO settings for the SSI pins.  This function also gives
    // control of these pins to the SSI hardware.  Consult the data sheet to
    // see which functions are allocated per pin.
    // The pins are assigned as follows:
    //      PA5 - SSI0Tx (TM4C123x) / SSI0XDAT1 (TM4C129x)
    //      PA4 - SSI0Rx (TM4C123x) / SSI0XDAT0 (TM4C129x)
    //      PA3 - SSI0Fss
    //      PA2 - SSI0CLK
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 |
                   GPIO_PIN_2);

    SSIConfigSetExpClk(SSI0_BASE, 16000000, SSI_FRF_MOTO_MODE_0,
                       SSI_MODE_MASTER, 1000000, 8);

    SSIEnable(SSI0_BASE);

    while(SSIDataGetNonBlocking(SSI0_BASE, &dummy_data));

    SSIDataPut(SSI0_BASE, ZERO_SEG);
    SSIDataPut(SSI0_BASE, ZERO_SEG);

} // End init_midi_channel_select

static void set_channel(uint32_t channel) {

    switch (channel) {
    case 0 :
        SSIDataPut(SSI0_BASE, ZERO_SEG);
        SSIDataPut(SSI0_BASE, ZERO_SEG);
        break;
    case 1 :
        SSIDataPut(SSI0_BASE, ONE_SEG);
        SSIDataPut(SSI0_BASE, ZERO_SEG);
        break;
    case 2:
        SSIDataPut(SSI0_BASE, TWO_SEG);
        SSIDataPut(SSI0_BASE, ZERO_SEG);
        break;
    case 3 :
        SSIDataPut(SSI0_BASE, THREE_SEG);
        SSIDataPut(SSI0_BASE, ZERO_SEG);
        break;
    case 4 :
        SSIDataPut(SSI0_BASE, FOUR_SEG);
        SSIDataPut(SSI0_BASE, ZERO_SEG);
        break;
    case 5 :
        SSIDataPut(SSI0_BASE, FIVE_SEG);
        SSIDataPut(SSI0_BASE, ZERO_SEG);
        break;
    case 6 :
        SSIDataPut(SSI0_BASE, SIX_SEG);
        SSIDataPut(SSI0_BASE, ZERO_SEG);
        break;
    case 7 :
        SSIDataPut(SSI0_BASE, SEVEN_SEG);
        SSIDataPut(SSI0_BASE, ZERO_SEG);
        break;
    case 8 :
        SSIDataPut(SSI0_BASE, EIGHT_SEG);
        SSIDataPut(SSI0_BASE, ZERO_SEG);
        break;
    case 9 :
        SSIDataPut(SSI0_BASE, NINE_SEG);
        SSIDataPut(SSI0_BASE, ZERO_SEG);
        break;
    case 10 :
        SSIDataPut(SSI0_BASE, ZERO_SEG);
        SSIDataPut(SSI0_BASE, ONE_SEG);
        break;
    case 11 :
        SSIDataPut(SSI0_BASE, ONE_SEG);
        SSIDataPut(SSI0_BASE, ONE_SEG);
        break;
    case 12 :
        SSIDataPut(SSI0_BASE, TWO_SEG);
        SSIDataPut(SSI0_BASE, ONE_SEG);
        break;
    case 13 :
        SSIDataPut(SSI0_BASE, THREE_SEG);
        SSIDataPut(SSI0_BASE, ONE_SEG);
        break;
    case 14 :
        SSIDataPut(SSI0_BASE, FOUR_SEG);
        SSIDataPut(SSI0_BASE, ONE_SEG);
        break;
    case 15 :
        SSIDataPut(SSI0_BASE, FIVE_SEG);
        SSIDataPut(SSI0_BASE, ONE_SEG);
        break;
    case 16 :
        SSIDataPut(SSI0_BASE, SIX_SEG);
        SSIDataPut(SSI0_BASE, ONE_SEG);
        break;
    default :
        break;
    }
}

void midi_channel_select_task(void* parm) {

    uint32_t index;

    while(1) {

        for(index=0; index<2; index++) {
            switch(button_states[index].last_button_state) {
            case BUTTON_UP :

                if(1 == read_gpi(button_states[index].pin)) {
                    if (index == CHANNEL_UP) {

                        channel = (channel + 1) % 16;

                    } else {

                        if (channel > 1) {
                            channel--;
                        } else {
                            channel = 15;
                        }
                    }

                    set_channel(channel);

                    button_states[index].last_button_state = BUTTON_DOWN;
                }
                break;
            case BUTTON_DOWN :
                if(0 == read_gpi(button_states[index].pin)) {
                    button_states[index].last_button_state = BUTTON_UP;
                }
                break;
            default:
                assert(0);
                break;

            }
        }

        vTaskDelay(50);
    }
} // End midi_channel_select_task

uint32_t get_channel(void) {
    return channel;
} // End get_channel
