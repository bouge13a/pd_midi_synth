/*
 * UART_midi_functions.c
 *
 *  Created on: Apr 15, 2021
 *      Author: steph
 */

#include <assert.h>
#include <midi_uart_functions.h>
#include <stdbool.h>
#include <stdint.h>

#include "driverlib/uart.h"
#include "driverlib/inc/hw_memmap.h"

static const uint8_t STATUS_BIT        = 0x80;
static const uint8_t NOTE_OFF_MSG_TYPE = 0x00;
static const uint8_t NOTE_ON_MSG_TYPE  = 0x01;
static const uint8_t CTRL_CHG_MSG_TYPE = 0x03;
static const uint8_t PITCH_MSG_TYPE    = 0x06;

static const uint8_t PITCH_CRTL_CHG = 1;

static uint8_t msg_bytes[3];

void send_midi_message(uart_msg_u msg) {


    switch(msg.bitfield.message_type) {
    case NOTE_ON :
        msg_bytes[0] = (STATUS_BIT | NOTE_ON_MSG_TYPE << 4) | msg.bitfield.channel;
        msg_bytes[1] = 0x7F & msg.bitfield.pad_num;
        msg_bytes[2] = 0x7F & msg.bitfield.value;
        break;
    case NOTE_OFF :
        msg_bytes[0] = (STATUS_BIT | NOTE_OFF_MSG_TYPE << 4) | msg.bitfield.channel;
        msg_bytes[1] = 0x7F & msg.bitfield.pad_num;
        msg_bytes[2] = 0x7F & msg.bitfield.value;
        break;
//    case PITCH :
//        msg_bytes[0] = STATUS_BIT | PITCH_MSG_TYPE;
//        msg_bytes[1] = (msg.bitfield.channel << 4) | msg.bitfield.pad_num;
//        msg_bytes[2] = (msg.bitfield.value);
//        break;
//    case MODULATION :
//
//        break;
//    case KNOB1 :
//    case KNOB2 :
//    case KNOB3 :
//    case KNOB4 :
//        cmd_byte.bitfield.msg_type = CTRL_CHG_MSG_TYPE;
//        break;
    default :

        break;
    }

    UARTCharPutNonBlocking(UART1_BASE, msg_bytes[0]);
    UARTCharPutNonBlocking(UART1_BASE, msg_bytes[1]);
    UARTCharPutNonBlocking(UART1_BASE, msg_bytes[2]);


} // End process_midi_message
