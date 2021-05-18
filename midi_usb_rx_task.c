#include "midi_usb_rx_task.h"

void MIDI_USB_Rx_Task(void) {
	while(USBMIDI_OutEpFIFO_Pop(&rxmsg)) {
		//UARTprintf("%02x : %02x : %02x : %02x\n", msg.header, msg.byte1, msg.byte2, msg.byte3);
	}
}

void MIDI_USB_Echo_Task(void) {
    while(USBMIDI_OutEpFIFO_Pop(&rxmsg)) {
        if(rxmsg.header!=0) {
            USBMIDI_InEpMsgWrite(&rxmsg);
            MAP_SysCtlDelay(10000);
        }
    }
}
