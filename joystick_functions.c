/*
 * joystick_functions.c
 *
 *  Created on: Apr 12, 2021
 *      Author: steph
 */
#include <assert.h>
#include <stdlib.h>

#include "joystick_functions.h"
#include "host_uart_task.h"
#include "rotary_enc_task.h"



static const uint32_t MIDPOINT = 1<<13;
static const uint32_t WINDOW   = 1200;

static const uint32_t JS_HIGH_REF = MIDPOINT + WINDOW;
static const uint32_t JS_LOW_REF  = MIDPOINT - WINDOW;;

static uart_msg_u js_uart_msg;

typedef enum {
    SEND,
    SEND_STOP,
    IDLE,
}js_state_e;

static js_state_e js_x_state = IDLE;
static js_state_e js_y_state = IDLE;

void process_joystick(uint16_t x_raw, uint16_t y_raw) {

    switch (js_x_state) {
    case IDLE :

        if ((x_raw > JS_HIGH_REF) || (x_raw < JS_LOW_REF)) {
            js_x_state = SEND;
        }

        break;
    case SEND :

        if (x_raw > JS_HIGH_REF) {

            x_raw = (x_raw - JS_HIGH_REF) * 2;

            js_uart_msg.bitfield.channel = get_channel();
            js_uart_msg.bitfield.pad_num = 0;
            js_uart_msg.bitfield.message_type = MODULATION;
            js_uart_msg.bitfield.value = (x_raw/17356.0)*127;

            send_to_host(js_uart_msg);

        } else if (x_raw < JS_LOW_REF) {

            x_raw = (JS_LOW_REF - x_raw ) * 2;

            js_uart_msg.bitfield.channel = get_channel();
            js_uart_msg.bitfield.pad_num = 0;
            js_uart_msg.bitfield.message_type = MODULATION;
            js_uart_msg.bitfield.value = (x_raw/17356.0)*127;

            send_to_host(js_uart_msg);

        } else {
            js_x_state = SEND_STOP;
        }

        break;
    case SEND_STOP :

        js_uart_msg.bitfield.channel = get_channel();
        js_uart_msg.bitfield.pad_num = 0;
        js_uart_msg.bitfield.message_type = MODULATION;
        js_uart_msg.bitfield.value = 0;

        send_to_host(js_uart_msg);

        js_x_state = IDLE;

        break;

    default :
        assert(0);

        break;
    }


    switch (js_y_state) {
    case IDLE:
        if (y_raw > JS_HIGH_REF || y_raw < JS_LOW_REF) {
            js_y_state = SEND;
        }

        break;
    case SEND :

        if (y_raw > JS_HIGH_REF) {



            js_uart_msg.bitfield.channel = get_channel();
            js_uart_msg.bitfield.pad_num = 0;
            js_uart_msg.bitfield.message_type = PITCH;
            js_uart_msg.bitfield.value = y_raw - WINDOW;

            send_to_host(js_uart_msg);


        } else if (y_raw < JS_LOW_REF) {

            js_uart_msg.bitfield.channel = get_channel();
            js_uart_msg.bitfield.pad_num = 0;
            js_uart_msg.bitfield.message_type = PITCH;
            js_uart_msg.bitfield.value = y_raw + WINDOW;

            send_to_host(js_uart_msg);

        } else {
            js_y_state = SEND_STOP;
        }

        break;
    case SEND_STOP :

        js_uart_msg.bitfield.channel = get_channel();
        js_uart_msg.bitfield.pad_num = 0;
        js_uart_msg.bitfield.message_type = PITCH;
        js_uart_msg.bitfield.value = MIDPOINT;

        send_to_host(js_uart_msg);

        js_y_state = IDLE;

        break;

    default :
        assert(0);
        break;
    }



} // End process_joystick
