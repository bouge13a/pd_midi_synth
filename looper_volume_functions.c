/*
 * looper_volume_functions.c
 *
 *  Created on: Apr 20, 2021
 *      Author: steph
 */


#include "looper_volume_functions.h"

#include <stdlib.h>

#include "usb_hid_task.h"

static const uint32_t VOLUME_INCREMENT = 100;

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
    usb_hid_msg_u msg;

    for (index=0; index<4; index++) {
        if (abs(adc0_data[index] - last_value[index]) > VOLUME_INCREMENT ) {
            msg.bitfield.message_type = TRACK_VOL_MSG;
            msg.bitfield.ctrl_num = index;
            msg.bitfield.value = (adc0_data[index]/((1<<12)*1.0))*256;

            usb_hid_send(msg);
        }

        last_value[index] = adc0_data[index];
    }

    for (index=4; index<8; index++) {
        if (abs(adc1_data[index] - last_value[index]) > VOLUME_INCREMENT ) {
            msg.bitfield.message_type = TRACK_VOL_MSG;
            msg.bitfield.ctrl_num = index;
            msg.bitfield.value = (adc1_data[index]/((1<<12)*1.0))*256;

            usb_hid_send(msg);
        }

        last_value[index] = adc1_data[index];
    }

} // End process_looper_volumes
