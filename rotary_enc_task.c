/*
 * rotary_encoder.c
 *
 *  Created on: Apr 9, 2021
 *      Author: steph
 */

#include "FreeRTOS.h"
#include "task.h"

#include <rotary_enc_task.h>
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

static gpio_pin_t* enc_bit_0;
static gpio_pin_t* enc_bit_1;
static gpio_pin_t* enc_bit_2;
static gpio_pin_t* enc_bit_3;

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

static uint32_t bits;


void init_rotary_enc(void) {

    uint32_t dummy_data;

    enc_bit_0 = get_gpi_config("bit 0");
    enc_bit_1 = get_gpi_config("bit 1");
    enc_bit_2 = get_gpi_config("bit 2");
    enc_bit_3 = get_gpi_config("bit 3");

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

    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
                       SSI_MODE_MASTER, 1000000, 8);

    SSIEnable(SSI0_BASE);

    while(SSIDataGetNonBlocking(SSI0_BASE, &dummy_data));

} // end init_rotary_enc

void rotary_enc_task(void* parm) {

    while(1) {

        bits = read_gpi(enc_bit_0);
        bits = bits | (read_gpi(enc_bit_1) << 1);
        bits = bits | (read_gpi(enc_bit_2) << 2);
        bits = bits | (read_gpi(enc_bit_3) << 3);

        switch (bits) {
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

        vTaskDelay(200);
    }
} // End rotary_enc_task

uint32_t get_channel(void) {
    return bits;
}

