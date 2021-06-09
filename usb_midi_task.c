/*
 * uart_task.c
 *
 *  Created on: Apr 1, 2021
 *      Author: steph
 */

#include <usb_midi_task.h>
#include <stdbool.h>
#include <assert.h>

#include <usbmidi_types.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "GPOs.h"
#include "console_task.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "utils/uartstdio.h"
#include "driverlib/inc/hw_gpio.h"
#include "driverlib/inc/hw_types.h"
#include "usblib/usblib.h"
#include "usblib/device/usbdevice.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"
#include "midi.h"
#include "usbmidi_types.h"
#include "usbmidi.h"

static uint32_t SysTickCount = 0;

static USBMIDI_Message_t txmsg;
static USBMIDI_Message_t rxmsg;

static QueueHandle_t uart_tx_q = NULL;
static uint32_t page_num;

static const uint8_t STATUS_BIT        = 0x80;
static const uint8_t NOTE_OFF_MSG_TYPE = 0x00;
static const uint8_t POLYPHON_MSG_TYPE = 0x02;
static const uint8_t NOTE_ON_MSG_TYPE  = 0x01;
static const uint8_t CTRL_CHG_MSG_TYPE = 0x03;
static const uint8_t PITCH_MSG_TYPE    = 0x06;

static const uint8_t MOD_CRTL_CHG      = 0x01;
static const uint8_t EFFECT1_CTRL_CHG  = 0x0C;
static const uint8_t EFFECT2_CTRL_CHG  = 0x0D;
static const uint8_t PAN_CTRL_CHG      = 0x0A;
static const uint8_t CHAN_VOL_CTRL_CHG = 0x07;

static bool prev_state = false;

static void noteOn(short channel, uint8_t note, uint8_t velocity) {
    txmsg.header = USB_MIDI_HEADER(1, USB_MIDI_CIN_NOTEON);
    txmsg.byte1 = MIDI_MSG_NOTEON | channel;
    txmsg.byte2 = note;
    txmsg.byte3 = velocity;
    USBMIDI_InEpMsgWrite(&txmsg);
}

static void noteOff(short channel, uint8_t note, uint8_t velocity) {
    txmsg.header = USB_MIDI_HEADER(1, USB_MIDI_CIN_NOTEOFF);
    txmsg.byte1 = MIDI_MSG_NOTEOFF | channel;
    txmsg.byte2 = note;
    txmsg.byte3 = velocity;
    USBMIDI_InEpMsgWrite(&txmsg);
}

static void controlChange(short channel, uint8_t key, uint8_t value) {
    txmsg.header = USB_MIDI_HEADER(1, USB_MIDI_CIN_CTRLCHANGE);
    txmsg.byte1 = MIDI_MSG_CTRLCHANGE | channel;
    txmsg.byte2 = key;
    txmsg.byte3 = value;
    USBMIDI_InEpMsgWrite(&txmsg);
}

static void pitchBend(short channel, uint16_t pitch) {
    txmsg.header = USB_MIDI_HEADER(1, USB_MIDI_CIN_PITCHBEND);
    txmsg.byte1 = MIDI_MSG_PITCHBEND | channel;
    txmsg.byte2 = pitch & 0x7F; // lsb
    txmsg.byte3 = (pitch >> 7) & 0x7F; // msb
    USBMIDI_InEpMsgWrite(&txmsg);
}

static void overdrive(short channel, uint8_t key, uint8_t value) {
    txmsg.header = USB_MIDI_HEADER(1, USB_MIDI_CIN_POLYKEYPRESS);
    txmsg.byte1 = MIDI_MSG_POLYPRESSURE | channel;
    txmsg.byte2 = key; // lsb
    txmsg.byte3 = value; // msb
    USBMIDI_InEpMsgWrite(&txmsg);
}

static void MIDI_USB_Rx_Task(void) {
    while(USBMIDI_OutEpFIFO_Pop(&rxmsg)) {
        //UARTprintf("%02x : %02x : %02x : %02x\n", msg.header, msg.byte1, msg.byte2, msg.byte3);
    }
}

static void send_usb_midi_msg(uart_msg_u msg) {

    switch(msg.bitfield.message_type) {
    case NOTE_ON :
        noteOn(msg.bitfield.channel, msg.bitfield.pad_num, msg.bitfield.value);
        break;
    case NOTE_OFF :
        noteOff(msg.bitfield.channel, msg.bitfield.pad_num, msg.bitfield.value);
        break;
    case PITCH :
        pitchBend(msg.bitfield.channel, msg.bitfield.value);
        break;
    case MODULATION :
        controlChange(msg.bitfield.channel, MOD_CRTL_CHG, msg.bitfield.value);
        break;
    case OVERDRIVE :
        overdrive(msg.bitfield.channel, MIDI_MSG_POLYPRESSURE, msg.bitfield.value);
        break;
    case KNOB1 :
        controlChange(msg.bitfield.channel, EFFECT1_CTRL_CHG, msg.bitfield.value);
        break;
    case KNOB2 :
        controlChange(msg.bitfield.channel, EFFECT2_CTRL_CHG, msg.bitfield.value);
        break;
    case KNOB3 :
        controlChange(msg.bitfield.channel, PAN_CTRL_CHG, msg.bitfield.value);
        break;
    case KNOB4 :
        controlChange(msg.bitfield.channel, CHAN_VOL_CTRL_CHG, msg.bitfield.value);
        break;
    case LOOP :
    case LOOPVOLUME :
    case EFFECT :
    case MASTER :
        controlChange(msg.bitfield.channel, msg.bitfield.pad_num, msg.bitfield.value);
        break;
    default :
        assert(0);
        break;

    }

} // End send_usb_midi_msg

void init_usb_midi(uint32_t page_number) {

    page_num = page_number;

    uart_tx_q = xQueueCreate(12, sizeof(uart_msg_u));

    // usb init
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)){}
    MAP_GPIOPinTypeUSBAnalog(GPIO_PORTD_AHB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    // notice: for tm4c123gh6pm, eUSBModeForceDevice is required
    USBStackModeSet(0, eUSBModeForceDevice, 0);

    USBMIDI_Init(0);

} // End init_host_uart

void usb_midi_task(void* parm) {

    uart_msg_u uart_msg;

    while(1){

        xQueueReceive(uart_tx_q, &uart_msg, portMAX_DELAY);

        if(USBMIDI_IsConnected()) {
            if(prev_state == false) {
               prev_state = true;
            }
        } else {
            if(prev_state == true) {
               prev_state = false;
               continue;
            }
        }

        send_usb_midi_msg(uart_msg);

        if (is_on_screen(page_num)) {
            UARTprintf("%d %d %d %d\n\r", uart_msg.bitfield.message_type,
                                        uart_msg.bitfield.pad_num,
                                        uart_msg.bitfield.value,
                                        uart_msg.bitfield.channel);
        }

    }

} // End host_uart_task

void usb_midi_rx_task(void* parm) {

    while(1) {

        if(USBMIDI_IsConnected()) {
            if(prev_state == false) {
               prev_state = true;
            }
        } else {
            if(prev_state == true) {
               prev_state = false;
               continue;
            }
        }

        MIDI_USB_Rx_Task();

        SysTickCount++;

        vTaskDelay(1);
    }
}

void send_to_host(uart_msg_u message) {

    xQueueSend(uart_tx_q, &message, 0);

}

void send_to_host_from_isr(uart_msg_u message) {

    xQueueSendFromISR(uart_tx_q, &message, 0);

}

// GUI functions
void usb_midi_drawpage(void) {

}
void usb_midi_drawdata(void){

}
void usb_midi_drawinput(int character){

}

