/*
 * looper_volume_functions.c
 *
 *  Created on: Apr 20, 2021
 *      Author: steph
 */

#include "host_uart_task.h"
#include "looper_volume_functions.h"

#include <stdlib.h>
#include <stdbool.h>

static bool send_all_volumes = false;

static const uint32_t VOLUME_INCREMENT = 100;

static const uint32_t VOLUME_OFFSET = 54;

static uint32_t last_value[8];

void init_looper_volumes(void) {

    uint32_t index;

    for (index=0; index<8; index++) {
        last_value[index] = 0;
    }

} // End init_looper_volumes

void process_looper_volumes(uint32_t* adc0_data,
                            uint32_t* adc1_data) {

    uint32_t index;
    uart_msg_u uart_msg;

    if (send_all_volumes) {
        for (index=0; index<4; index++) {
            uart_msg.bitfield.message_type = 10;
            uart_msg.bitfield.pad_num = VOLUME_OFFSET + index;
            uart_msg.bitfield.value = (adc0_data[index]/((1<<12)*1.0))*256;

            send_to_host(uart_msg);
        }

        for (index=0; index<4; index++) {
            uart_msg.bitfield.message_type = 10;
            uart_msg.bitfield.pad_num = VOLUME_OFFSET + 4 + index;
            uart_msg.bitfield.value = (adc1_data[index]/((1<<12)*1.0))*256;

            send_to_host(uart_msg);
        }

        send_all_volumes = false;
    }

    for (index=0; index<4; index++) {
        if (abs(adc0_data[index] - last_value[index]) > VOLUME_INCREMENT ) {

            uart_msg.bitfield.message_type = 10;
            uart_msg.bitfield.pad_num = VOLUME_OFFSET + index;
            uart_msg.bitfield.value = (adc0_data[index]/((1<<12)*1.0))*256;

            send_to_host(uart_msg);
        }

        last_value[index] = adc0_data[index];
    }

    for (index=4; index<8; index++) {
        if (abs(adc1_data[index] - last_value[index]) > VOLUME_INCREMENT ) {

            uart_msg.bitfield.message_type = 10;
            uart_msg.bitfield.pad_num = VOLUME_OFFSET + 4 + index;
            uart_msg.bitfield.value = (adc1_data[index]/((1<<12)*1.0))*256;

            send_to_host(uart_msg);
        }

        last_value[index] = adc1_data[index];
    }

} // End process_looper_volumes

void send_volumes(void) {
    send_all_volumes = true;
} // End send_volume_function
