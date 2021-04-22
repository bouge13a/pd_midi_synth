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

static const uint8_t MOD_CRTL_CHG      = 0x01;
static const uint8_t EFFECT1_CTRL_CHG  = 0x0C;
static const uint8_t EFFECT2_CTRL_CHG  = 0x0D;
static const uint8_t PAN_CTRL_CHG      = 0x0A;
static const uint8_t CHAN_VOL_CTRL_CHG = 0x07;

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
    case PITCH :
        msg_bytes[0] = ((STATUS_BIT | PITCH_MSG_TYPE) << 4) | msg.bitfield.channel ;
        msg_bytes[1] = 0x7F & (msg.bitfield.value >> 7);
        msg_bytes[2] = (0x7F & msg.bitfield.value);
        break;
    case MODULATION :
        msg_bytes[0] = (STATUS_BIT | CTRL_CHG_MSG_TYPE << 4) | msg.bitfield.channel;
        msg_bytes[1] = 0x7F & MOD_CRTL_CHG;
        msg_bytes[2] = 0x7F & msg.bitfield.value;
        break;
    case KNOB1 :
        msg_bytes[0] = (STATUS_BIT | CTRL_CHG_MSG_TYPE << 4) | msg.bitfield.channel;
        msg_bytes[1] = 0x7F & EFFECT1_CTRL_CHG;
        msg_bytes[2] = 0x7F & msg.bitfield.value;
        break;
    case KNOB2 :
        msg_bytes[0] = (STATUS_BIT | CTRL_CHG_MSG_TYPE << 4) | msg.bitfield.channel;
        msg_bytes[1] = 0x7F & EFFECT2_CTRL_CHG;
        msg_bytes[2] = 0x7F & msg.bitfield.value;
        break;
    case KNOB3 :
        msg_bytes[0] = (STATUS_BIT | CTRL_CHG_MSG_TYPE << 4) | msg.bitfield.channel;
        msg_bytes[1] = 0x7F & PAN_CTRL_CHG;
        msg_bytes[2] = 0x7F & msg.bitfield.value;
        break;
    case KNOB4 :
        msg_bytes[0] = (STATUS_BIT | CTRL_CHG_MSG_TYPE << 4) | msg.bitfield.channel;
        msg_bytes[1] = 0x7F & CHAN_VOL_CTRL_CHG;
        msg_bytes[2] = 0x7F & msg.bitfield.value;
        break;
    default :
        assert(0);
        break;
    }

    UARTCharPutNonBlocking(UART1_BASE, msg_bytes[0]);
    UARTCharPutNonBlocking(UART1_BASE, msg_bytes[1]);
    UARTCharPutNonBlocking(UART1_BASE, msg_bytes[2]);


} // End process_midi_message
