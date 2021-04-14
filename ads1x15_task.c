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

static volatile value16_t adc0_data[4];
static volatile value16_t adc1_data[4];

// See ADS1115 data sheet for explanation
static const uint8_t ADC0_SEL[] = {
    0b00000001,
    0b11000001,
    0b11100011,
};

// See ADS1115 data sheet for explanation
static const uint8_t ADC1_SEL[] = {
    0b00000001,
    0b11010001,
    0b11100011,
};

// See ADS1115 data sheet for explanation
static const uint8_t ADC2_SEL[] = {
    0b00000001,
    0b11100001,
    0b11100011,
};

// See ADS1115 data sheet for explanation
static const uint8_t ADC3_SEL[] = {
    0b00000001,
    0b11110001,
    0b11100011,
};

// See ADS1115 data sheet for explanation
static const uint8_t CONV_REG = 0b00000000;


////////////////////////////////////////////////////////
//         Analog Select MSGS
////////////////////////////////////////////////////////

static i2c_msg_t joy_y_sel = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 0,
    .rx_data      = 0,
    .state        = i2c_ready,
    .tx_data      = ADC0_SEL,
    .num_tx_bytes = 3,
};

static i2c_msg_t joy_x_sel = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 0,
    .rx_data      = 0,
    .state        = i2c_ready,
    .tx_data      = ADC1_SEL,
    .num_tx_bytes = 3,
};

static i2c_msg_t sense_sel = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 0,
    .rx_data      = 0,
    .state        = i2c_ready,
    .tx_data      = ADC2_SEL,
    .num_tx_bytes = 3,
};

static i2c_msg_t volume_sel = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 0,
    .rx_data      = 0,
    .state        = i2c_ready,
    .tx_data      = ADC3_SEL,
    .num_tx_bytes = 3,
};

static i2c_msg_t ads1015_adc10_sel = {
    .address      = 0x49,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 0,
    .rx_data      = 0,
    .state        = i2c_ready,
    .tx_data      = ADC0_SEL,
    .num_tx_bytes = 3,
};

static i2c_msg_t ads1015_adc11_sel = {
    .address      = 0x49,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 0,
    .rx_data      = 0,
    .state        = i2c_ready,
    .tx_data      = ADC1_SEL,
    .num_tx_bytes = 3,
};

static i2c_msg_t ads1015_adc12_sel = {
    .address      = 0x49,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 0,
    .rx_data      = 0,
    .state        = i2c_ready,
    .tx_data      = ADC2_SEL,
    .num_tx_bytes = 3,
};

static i2c_msg_t ads1015_adc13_sel = {
    .address      = 0x49,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 0,
    .rx_data      = 0,
    .state        = i2c_ready,
    .tx_data      = ADC3_SEL,
    .num_tx_bytes = 3,
};

////////////////////////////////////////////////////////
//            Analog read MSGS
///////////////////////////////////////////////////////

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

static i2c_msg_t adc10_read = {
    .address      = 0x49,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 2,
    .rx_data      = adc1_data[0].bytes,
    .state        = i2c_ready,
    .tx_data      = &CONV_REG,
    .num_tx_bytes = 1,
};

static i2c_msg_t adc11_read = {
    .address      = 0x49,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 2,
    .rx_data      = adc1_data[1].bytes,
    .state        = i2c_ready,
    .tx_data      = &CONV_REG,
    .num_tx_bytes = 1,
};

static i2c_msg_t adc12_read = {
    .address      = 0x49,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 2,
    .rx_data      = adc1_data[2].bytes,
    .state        = i2c_ready,
    .tx_data      = &CONV_REG,
    .num_tx_bytes = 1,
};

static i2c_msg_t adc13_read = {
    .address      = 0x49,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 2,
    .rx_data      = adc1_data[3].bytes,
    .state        = i2c_ready,
    .tx_data      = &CONV_REG,
    .num_tx_bytes = 1,
};

///////////////////////////////////////////////////
//       Task Functions
////////////////////////////////////////////////////

void init_ads1x15(void) {
    uint32_t index;

    joy_y_data.value = 0;
    joy_x_data.value = 0;
    sense_data.value = 0;
    volume_data.value = 0;

    for(index=0; index<4; index++) {
        adc0_data[index].value = 0;
    }

    for(index=0; index<4; index++) {
        adc1_data[index].value = 0;
    }
}

void ads1x15_midi_task(void* parm) {

    uint32_t index = 0;

    while(1) {

        add_i2c_msg(&joy_y_sel);
        vTaskDelay(2);
        add_i2c_msg(&joy_y_read);
        vTaskDelay(2);
        add_i2c_msg(&joy_x_sel);
        vTaskDelay(2);
        add_i2c_msg(&joy_x_read);
        vTaskDelay(2);

        process_joystick(joy_x_data.value, joy_y_data.value);

        if (index % 10 == 0) {

            add_i2c_msg(&sense_sel);
            vTaskDelay(2);
            add_i2c_msg(&sense_read);
            vTaskDelay(2);
            add_i2c_msg(&volume_sel);
            vTaskDelay(2);
            add_i2c_msg(&volume_read);
            vTaskDelay(2);

            add_i2c_msg(&ads1015_adc10_sel);
            vTaskDelay(1);
            add_i2c_msg(&adc10_read);
            vTaskDelay(1);
            add_i2c_msg(&ads1015_adc11_sel);
            vTaskDelay(1);
            add_i2c_msg(&adc11_read);
            vTaskDelay(1);
            add_i2c_msg(&ads1015_adc12_sel);
            vTaskDelay(1);
            add_i2c_msg(&adc12_read);
            vTaskDelay(1);
            add_i2c_msg(&ads1015_adc13_sel);
            vTaskDelay(1);
            add_i2c_msg(&adc13_read);
            vTaskDelay(1);

        } else {
            //vTaskDelay(12);
        }

        index++;
    }
} // End joystick_task

float get_volume(void) {
    return volume_data.value/65535;
}// End get_volume

float get_sensitivity(void) {
    return sense_data.value/65535;
} // End get_Sensitivity


///////////////////////////////////////////////////////
//           GUI functions
///////////////////////////////////////////////////////

void ads1x15_drawpage(void) {
    cursor_pos(5, 0);
    UARTprintf("Joystick Y:\r\n");
    UARTprintf("JoyStick X:\r\n");
    UARTprintf("Volume:\r\n");
    UARTprintf("Sensitivity:\r\n\n");
    UARTprintf("Effect 0:\r\n");
    UARTprintf("Effect 1:\r\n");
    UARTprintf("Effect 2:\r\n");
    UARTprintf("Effect 3:\r\n");
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

    cursor_pos(10, 16);
    UARTprintf("        ");
    cursor_pos(10, 16);
    UARTprintf("%d", adc1_data[0].value);

    cursor_pos(11, 16);
    UARTprintf("        ");
    cursor_pos(11, 16);
    UARTprintf("%d", adc1_data[1].value);

    cursor_pos(12, 16);
    UARTprintf("        ");
    cursor_pos(12, 16);
    UARTprintf("%d", adc1_data[2].value);

    cursor_pos(13, 16);
    UARTprintf("        ");
    cursor_pos(13, 16);
    UARTprintf("%d", adc1_data[3].value);


}
void ads1x15_drawinput(int character) {

}
