/*
 * looper_buttons.c
 *
 *  Created on: Apr 16, 2021
 *      Author: steph
 */


#include <looper_buttons_task.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "FreeRTOS.h"
#include "task.h"

#include "GPIs.h"
#include "GPOs.h"
#include "board_pin_defs.h"
#include "usb_hid_task.h"

typedef enum {
    BUTTON_UP_IDLE,
    BUTTON_DOWN_RECORD,
    BUTTON_UP_RECORD,
    BUTTON_DOWN_PLAY,
    BUTTON_UP_PLAY,
    BUTTON_DOWN_IDLE,
} pin_state_e;

typedef struct {
    gpio_pin_t* loop_pin;
    gpio_pin_t* led;
    pin_state_e loop_state;
}looper_pin_t;

static looper_pin_t looper_pins[8];

static bool recording = false;

static const uint32_t BLINK_PERIOD = 250; // ms

void init_looper_buttons(void) {

    uint32_t index;
    char string[7];

    // Initialize looper pins array
    for (index=0; index<8; index++) {
        sprintf(string, "loop %d", index);
        looper_pins[index].loop_pin = get_gpi_config(string);
        sprintf(string, "led %d", index);
        looper_pins[index].led = get_gpo_config(string);
        looper_pins[index].loop_state = BUTTON_UP_IDLE;
    }

} // End init_looper_buttons

void looper_buttons_task(void* parm) {

    uint32_t index;
    uint32_t count = 0;

    usb_hid_msg_u usb_hid_msg;

    while(1) {
        for (index=0; index<8; index++) {

            switch (looper_pins[index].loop_state) {

                case BUTTON_UP_IDLE:

                    if(!recording) {
                        if(1 == read_gpi(looper_pins[index].loop_pin)) {
                            recording = true;
                            looper_pins[index].loop_state = BUTTON_DOWN_RECORD;

                            usb_hid_msg.bitfield.message_type = RECORD_MSG;
                            usb_hid_msg.bitfield.ctrl_num = index;
                            usb_hid_msg.bitfield.value = 0;

                            usb_hid_send(usb_hid_msg);

                        }
                    }

                    break;

                case BUTTON_DOWN_RECORD:

                    if(0 == read_gpi(looper_pins[index].loop_pin)) {
                        looper_pins[index].loop_state = BUTTON_UP_RECORD;

                    }

                    break;

                case BUTTON_UP_RECORD:

                    if (1 == read_gpi(looper_pins[index].loop_pin)) {
                        looper_pins[index].loop_state = BUTTON_DOWN_PLAY;
                        set_gpo(looper_pins[index].led, 1);
                        recording = false;

                        usb_hid_msg.bitfield.message_type = PLAY_MSG;
                        usb_hid_msg.bitfield.ctrl_num = index;
                        usb_hid_msg.bitfield.value = 0;

                        usb_hid_send(usb_hid_msg);

                        break;
                    }

                    if (0 == count % BLINK_PERIOD) {
                        set_gpo(looper_pins[index].led,
                                 get_gpo(looper_pins[index].led) == 1 ? 0 : 1);
                    }

                    break;

                case BUTTON_DOWN_PLAY:

                    if (0 == read_gpi(looper_pins[index].loop_pin)) {
                        looper_pins[index].loop_state = BUTTON_UP_PLAY;
                        set_gpo(looper_pins[index].led, 0);
                    }

                    break;

                case BUTTON_UP_PLAY:

                    if (1 == read_gpi(looper_pins[index].loop_pin)) {
                        looper_pins[index].loop_state = BUTTON_DOWN_IDLE;

                        usb_hid_msg.bitfield.message_type = STOP_MSG;
                        usb_hid_msg.bitfield.ctrl_num = index;
                        usb_hid_msg.bitfield.value = 0;

                        usb_hid_send(usb_hid_msg);
                    }

                    break;

                case BUTTON_DOWN_IDLE:

                    if (0 == read_gpi(looper_pins[index].loop_pin)) {
                        looper_pins[index].loop_state = BUTTON_UP_IDLE;
                    }
                    break;
                default :
                    assert(0);

            }

        }
        count++;
        vTaskDelay(1);
    }

} // End looper_buttons_task
