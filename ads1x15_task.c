/*
 * joystick_task.c
 *
 *  Created on: Apr 10, 2021
 *      Author: steph
 */

#include <stdint.h>
#include <uartstdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "text_controls.h"
#include "ads1x15_task.h"
#include "I2C_task.h"
#include "host_uart_task.h"
#include "rotary_enc_task.h"
#include "joystick_functions.h"
#include "utils.h"

// See ADS1115 data sheet for explanation
static const uint8_t JOY_Y_SEL[] = {
    0b00000001,
    0b11000000,
    0b11100011,
};

static i2c_msg_t joy_y_sel = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 0,
    .rx_data      = 0,
    .state        = i2c_ready,
    .tx_data      = JOY_Y_SEL,
    .num_tx_bytes = 3,
};

// See ADS1115 data sheet for explanation
static const uint8_t JOY_X_SEL[] = {
    0b00000001,
    0b11010000,
    0b11100011,
};

static i2c_msg_t joy_x_sel = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 0,
    .rx_data      = 0,
    .state        = i2c_ready,
    .tx_data      = JOY_X_SEL,
    .num_tx_bytes = 3,
};

static const uint8_t CONV_REG = 0b00000000;

static volatile value16_t joy_y_data;

static i2c_msg_t joy_y_read = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 2,
    .rx_data      = joy_y_data.bytes,
    .state        = i2c_ready,
    .tx_data      = &CONV_REG,
    .num_tx_bytes = 1,
};

static volatile value16_t joy_x_data;

static i2c_msg_t joy_x_read = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 2,
    .rx_data      = joy_x_data.bytes,
    .state        = i2c_ready,
    .tx_data      = &CONV_REG,
    .num_tx_bytes = 1,
};

void init_ads1x15(void) {
    joy_y_data.value = 0;
    joy_x_data.value = 0;
}

static void switch_bytes(void) {
    uint8_t dummy;
    dummy = joy_y_data.bytes[0];
    joy_y_data.bytes[0] = joy_y_data.bytes[1];
    joy_y_data.bytes[1] = dummy;

    dummy = joy_x_data.bytes[0];
    joy_x_data.bytes[0] = joy_x_data.bytes[1];
    joy_x_data.bytes[1] = dummy;

}

void ads1x15_midi_task(void* parm) {

    while(1) {

        add_i2c_msg(&joy_y_sel);
        vTaskDelay(1);
        add_i2c_msg(&joy_y_read);
        vTaskDelay(1);
        add_i2c_msg(&joy_x_sel);
        vTaskDelay(1);
        add_i2c_msg(&joy_x_read);

        //process_joystick();

        vTaskDelay(20);

    }
} // End joystick_task


void ads1x15_drawpage(void) {
    cursor_pos(5, 0);
    UARTprintf("Joystick Y:\r\n");
    UARTprintf("JoyStick X:\r\n");
}
void ads1x15_drawdata(void) {

//    switch_bytes();

    cursor_pos(5, 16);
    UARTprintf("        ");
    cursor_pos(5, 16);
    UARTprintf("%d", joy_y_data.value);

    cursor_pos(6, 16);
    UARTprintf("        ");
    cursor_pos(6, 16);
    UARTprintf("%d", joy_x_data.value);
}
void ads1x15_drawinput(int character) {

}
