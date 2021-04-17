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


static const uint8_t NOTE_OFF_MSG_TYPE = 0x00;
static const uint8_t NOTE_ON_MSG_TYPE  = 0x01;
static const uint8_t CTRL_CHG_MSG_TYPE = 0x03;
static const uint8_t PITCH_MSG_TYPE    = 0x06;

static const uint8_t PITCH_CRTL_CHG = 1;

typedef struct {
    uint8_t status : 1;
    uint8_t msg_type : 3;
    uint8_t channel : 4;
}command_byte_t;

typedef union {
    uint8_t        byte;
    command_byte_t bitfield;
}command_byte_u;

typedef struct {
    uint8_t status : 1;
    uint8_t val      : 7;
}value_byte_t;

typedef union {
    uint8_t byte;
    value_byte_t bitfield;
}value_byte_u;

void send_midi_message(uart_msg_u msg) {

    command_byte_u cmd_byte;
    value_byte_u value_byte_0;
    value_byte_u value_byte_1;

    cmd_byte.bitfield.status     = 1;
    value_byte_0.bitfield.status = 0;
    value_byte_1.bitfield.status = 0;

    // Set the channel of the command byte
    cmd_byte.bitfield.channel = msg.bitfield.channel;

    switch(msg.bitfield.message_type) {
    case NOTE_ON :
        cmd_byte.bitfield.msg_type = NOTE_ON_MSG_TYPE;
        value_byte_0.bitfield.val  = msg.bitfield.pad_num;
        value_byte_1.bitfield.val  = msg.bitfield.value;
        break;
    case NOTE_OFF :
        cmd_byte.bitfield.msg_type = NOTE_OFF_MSG_TYPE;
        value_byte_0.bitfield.val  = msg.bitfield.pad_num;
        value_byte_1.bitfield.val  = 0x00;
        break;
    case PITCH :
        cmd_byte.bitfield.msg_type = PITCH_MSG_TYPE;
        value_byte_0.bitfield.val  = (msg.bitfield.value >> 7) & 0x007F;
        value_byte_1.bitfield.val  = msg.bitfield.value & 0x007F;
        break;
    case MODULATION :
        cmd_byte.bitfield.msg_type = CTRL_CHG_MSG_TYPE;
        value_byte_0.bitfield.val = PITCH_CRTL_CHG;
        value_byte_1.bitfield.val = msg.bitfield.value;
        break;
//    case KNOB1 :
//    case KNOB2 :
//    case KNOB3 :
//    case KNOB4 :
//        cmd_byte.bitfield.msg_type = CTRL_CHG_MSG_TYPE;
//        break;
    default :

        break;
    }

    UARTCharPutNonBlocking(UART1_BASE, cmd_byte.byte);
    UARTCharPutNonBlocking(UART1_BASE, value_byte_0.byte);
    UARTCharPutNonBlocking(UART1_BASE, value_byte_1.byte);


} // End process_midi_message
