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

typedef struct {
    uint8_t reserved : 4;
    uint8_t bit0 : 1;
    uint8_t bit1 : 1;
    uint8_t bit2 : 1;
    uint8_t bit3 : 1;
}enc_bits_t;

typedef union {
    enc_bits_t enc_bits;
    uint8_t number;
}enc_bits_u;

enc_bits_u enc_bits;

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

    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 |
                   GPIO_PIN_2);

    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
                       SSI_MODE_MASTER, 1000000, 8);

    SSIEnable(SSI0_BASE);

    while(SSIDataGetNonBlocking(SSI0_BASE, &dummy_data));

} // end init_rotary_enc

void rotary_enc_task(void* parm) {

    while(1) {

        enc_bits.enc_bits.bit0 = read_gpi(enc_bit_0);
        enc_bits.enc_bits.bit1 = read_gpi(enc_bit_1);
        enc_bits.enc_bits.bit2 = read_gpi(enc_bit_2);
        enc_bits.enc_bits.bit3 = read_gpi(enc_bit_3);

        vTaskDelay(200);
    }
} // End rotary_enc_task

uint32_t get_channel(void) {
    return enc_bits.number;
}

