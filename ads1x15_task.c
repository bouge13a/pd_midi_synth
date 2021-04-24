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
#include "midi_knobs_functions.h"
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
    0b11000000,
    0b11100011,
};

// See ADS1115 data sheet for explanation
static const uint8_t ADC1_SEL[] = {
    0b00000001,
    0b11010000,
    0b11100011,
};

// See ADS1115 data sheet for explanation
static const uint8_t ADC2_SEL[] = {
    0b00000001,
    0b11100000,
    0b11100011,
};

// See ADS1115 data sheet for explanation
static const uint8_t ADC3_SEL[] = {
    0b00000001,
    0b11110000,
    0b11100011,
};

// See ADS1115 data sheet for explanation
static const uint8_t CONV_REG = 0b00000000;


////////////////////////////////////////////////////////
//         Analog Select MSGS
////////////////////////////////////////////////////////

static i2c_msg_t ads1015_adc00_sel = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 0,
    .rx_data      = 0,
    .state        = i2c_ready,
    .tx_data      = ADC0_SEL,
    .num_tx_bytes = 3,
};

static i2c_msg_t ads1015_adc01_sel = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 0,
    .rx_data      = 0,
    .state        = i2c_ready,
    .tx_data      = ADC1_SEL,
    .num_tx_bytes = 3,
};

static i2c_msg_t ads1015_adc02_sel = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 0,
    .rx_data      = 0,
    .state        = i2c_ready,
    .tx_data      = ADC2_SEL,
    .num_tx_bytes = 3,
};

static i2c_msg_t ads1015_adc03_sel = {
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

static i2c_msg_t adc00_read = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 2,
    .rx_data      = adc0_data[0].bytes,
    .state        = i2c_ready,
    .tx_data      = &CONV_REG,
    .num_tx_bytes = 1,
};

static i2c_msg_t adc01_read = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 2,
    .rx_data      = adc0_data[1].bytes,
    .state        = i2c_ready,
    .tx_data      = &CONV_REG,
    .num_tx_bytes = 1,
};

static i2c_msg_t adc02_read = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 2,
    .rx_data      = adc0_data[2].bytes,
    .state        = i2c_ready,
    .tx_data      = &CONV_REG,
    .num_tx_bytes = 1,
};

static i2c_msg_t adc03_read = {
    .address      = 0x48,
    .bytes_rxed   = 0,
    .bytes_txed   = 0,
    .num_rx_bytes = 2,
    .rx_data      = adc0_data[3].bytes,
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

void ad1x15_looper_task(void* parm) {

    while (1) {

        add_i2c_msg(&ads1015_adc00_sel);
        vTaskDelay(1);
        add_i2c_msg(&adc00_read);
        vTaskDelay(1);
        add_i2c_msg(&ads1015_adc01_sel);
        vTaskDelay(1);
        add_i2c_msg(&adc01_read);
        vTaskDelay(1);
        add_i2c_msg(&ads1015_adc02_sel);
        vTaskDelay(1);
        add_i2c_msg(&adc02_read);
        vTaskDelay(1);
        add_i2c_msg(&ads1015_adc03_sel);
        vTaskDelay(1);
        add_i2c_msg(&adc03_read);
        vTaskDelay(1);
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

    }

} // End ad1x15_looper_task

void ads1x15_midi_task(void* parm) {

    uint32_t index = 0;

    while(1) {

        add_i2c_msg(&ads1015_adc00_sel);
        vTaskDelay(1);
        add_i2c_msg(&adc00_read);
        vTaskDelay(1);
        add_i2c_msg(&ads1015_adc01_sel);
        vTaskDelay(1);
        add_i2c_msg(&adc01_read);
        vTaskDelay(1);

        process_joystick(adc0_data[0].value, adc0_data[1].value);

        if (index % 10 == 0) {

            add_i2c_msg(&ads1015_adc02_sel);
            vTaskDelay(1);
            add_i2c_msg(&adc02_read);
            vTaskDelay(1);
            add_i2c_msg(&ads1015_adc03_sel);
            vTaskDelay(1);
            add_i2c_msg(&adc03_read);
            vTaskDelay(1);

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

            process_midi_knobs(adc1_data);

        } else {
            vTaskDelay(30);
        }

        vTaskDelay(25);

        index++;
    }
} // End joystick_task

float get_volume(void) {
    return adc0_data[3].value/17537.0;
}// End get_volume

float get_sensitivity(void) {
    return adc0_data[2].value/17537.0;
} // End get_Sensitivity


///////////////////////////////////////////////////////
//           GUI functions
///////////////////////////////////////////////////////

void ads1x15_drawpage(void) {

    cursor_pos(5, 0);

    UARTprintf("adc 0:\r\n");
    UARTprintf("adc 1:\r\n");
    UARTprintf("adc 2:\r\n");
    UARTprintf("adc 3:\r\n\n");

    UARTprintf("adc 0:\r\n");
    UARTprintf("adc 1:\r\n");
    UARTprintf("adc 2:\r\n");
    UARTprintf("adc 3:\r\n\n");

}
void ads1x15_drawdata(void) {

    cursor_pos(5, 16);
    UARTprintf("        ");
    cursor_pos(5, 16);
    UARTprintf("%d", adc0_data[0].value);

    cursor_pos(6, 16);
    UARTprintf("        ");
    cursor_pos(6, 16);
    UARTprintf("%d", adc0_data[1].value);

    cursor_pos(7, 16);
    UARTprintf("        ");
    cursor_pos(7, 16);
    UARTprintf("%d", adc0_data[2].value);

    cursor_pos(8, 16);
    UARTprintf("        ");
    cursor_pos(8, 16);
    UARTprintf("%d", adc0_data[3].value);

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
