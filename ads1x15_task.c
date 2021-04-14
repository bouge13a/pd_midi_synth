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


static volatile value16_t joy_y_data;
static volatile value16_t joy_x_data;
static volatile value16_t sense_data;
static volatile value16_t volume_data;

// See ADS1115 data sheet for explanation
static const uint8_t JOY_Y_SEL[] = {
    0b00000001,
    0b11000000,
    0b11100011,
};

// See ADS1115 data sheet for explanation
static const uint8_t JOY_X_SEL[] = {
    0b00000001,
    0b11010000,
    0b11100011,
};

// See ADS1115 data sheet for explanation
static const uint8_t SENSE_SEL[] = {
    0b00000001,
    0b11100000,
    0b11100011,
};

// See ADS1115 data sheet for explanation
static const uint8_t VOLUME_SEL[] = {
    0b00000001,
    0b11110000,
    0b11100011,
};

// See ADS1115 data sheet for explanation
static const uint8_t CONV_REG = 0b00000000;

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

static i2c_msg_t sense_sel = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 0,
    .rx_data      = 0,
    .state        = i2c_ready,
    .tx_data      = SENSE_SEL,
    .num_tx_bytes = 3,
};

static i2c_msg_t volume_sel = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 0,
    .rx_data      = 0,
    .state        = i2c_ready,
    .tx_data      = VOLUME_SEL,
    .num_tx_bytes = 3,
};

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

static i2c_msg_t sense_read = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 2,
    .rx_data      = sense_data.bytes,
    .state        = i2c_ready,
    .tx_data      = &CONV_REG,
    .num_tx_bytes = 1,
};

static i2c_msg_t volume_read = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 2,
    .rx_data      = volume_data.bytes,
    .state        = i2c_ready,
    .tx_data      = &CONV_REG,
    .num_tx_bytes = 1,
};

void init_ads1x15(void) {
    joy_y_data.value = 0;
    joy_x_data.value = 0;
    sense_data.value = 0;
    volume_data.value = 0;
}

void ads1x15_midi_task(void* parm) {

    uint32_t index = 0;

    while(1) {

        add_i2c_msg(&joy_y_sel);
        vTaskDelay(1);
        add_i2c_msg(&joy_y_read);
        vTaskDelay(1);
        add_i2c_msg(&joy_x_sel);
        vTaskDelay(1);
        add_i2c_msg(&joy_x_read);
        vTaskDelay(1);

        process_joystick(joy_x_data.value, joy_y_data.value);

        if (index % 10 == 0) {
            add_i2c_msg(&sense_sel);
            vTaskDelay(1);
            add_i2c_msg(&sense_read);
            vTaskDelay(1);
            add_i2c_msg(&volume_sel);
            vTaskDelay(1);
            add_i2c_msg(&volume_read);
        }

        vTaskDelay(20);

        index++;
    }
} // End joystick_task

float get_volume(void) {
    return volume_data.value/65535;
}// End get_volume

float get_sensitivity(void) {
    return sense_data.value/65535;
} // End get_Sensitivity


void ads1x15_drawpage(void) {
    cursor_pos(5, 0);
    UARTprintf("Joystick Y:\r\n");
    UARTprintf("JoyStick X:\r\n");
    UARTprintf("Volume:\r\n");
    UARTprintf("Sensitivity:\r\n");
}
void ads1x15_drawdata(void) {

    cursor_pos(5, 16);
    UARTprintf("        ");
    cursor_pos(5, 16);
    UARTprintf("%d", joy_y_data.value);

    cursor_pos(6, 16);
    UARTprintf("        ");
    cursor_pos(6, 16);
    UARTprintf("%d", joy_x_data.value);

    cursor_pos(7, 16);
    UARTprintf("        ");
    cursor_pos(7, 16);
    UARTprintf("%d", volume_data.value);

    cursor_pos(8, 16);
    UARTprintf("        ");
    cursor_pos(8, 16);
    UARTprintf("%d", sense_data.value);
}
void ads1x15_drawinput(int character) {

}
