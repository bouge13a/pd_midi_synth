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
#include "joystick_task.h"
#include "I2C_task.h"


typedef union {
    uint16_t value;
    uint8_t bytes[2];
}value16_t;

static uint8_t JOY_Y_SEL[] = {
    0b00000001,
    0b11000001,
    0b11100011,
};

static i2c_msg_t joy_y_sel = {
    .address      = 0x90,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 0,
    .rx_data      = 0,
    .state        = i2c_ready,
    .tx_data      = JOY_Y_SEL,
    .num_tx_bytes = 3,
};

static uint8_t JOY_X_SEL[] = {
    0b00000001,
    0b11010001,
    0b11100011,
};

static i2c_msg_t joy_x_sel = {
    .address      = 0x90,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 0,
    .rx_data      = 0,
    .state        = i2c_ready,
    .tx_data      = JOY_X_SEL,
    .num_tx_bytes = 3,
};

static uint8_t CONV_REG = 0b00000000;

static value16_t joy_y_data;

static i2c_msg_t joy_y_read = {
    .address      = 0x90,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 2,
    .rx_data      = joy_y_data.bytes,
    .state        = i2c_ready,
    .tx_data      = &CONV_REG,
    .num_tx_bytes = 3,
};

static value16_t joy_x_data;

static i2c_msg_t joy_x_read = {
    .address      = 0x90,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 2,
    .rx_data      = joy_x_data.bytes,
    .state        = i2c_ready,
    .tx_data      = &CONV_REG,
    .num_tx_bytes = 3,
};

void init_joystick(void) {
    joy_y_data.value = 0;
    joy_x_data.value = 0;
}

void joystick_task(void* parm) {

    while(1) {
        add_i2c_msg(&joy_y_sel);
        add_i2c_msg(&joy_y_read);
        add_i2c_msg(&joy_x_sel);
        add_i2c_msg(&joy_x_read);

        vTaskDelay(50);
    }
} // End joystick_task


void joystick_drawpage(void) {

}
void joystick_drawdata(void) {

}
void joystick_drawinput(int character) {

}
