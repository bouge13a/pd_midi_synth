/*
 * usb_hid_task.h
 *
 *  Created on: Apr 15, 2021
 *      Author: steph
 */

#ifndef USB_HID_TASK_H_
#define USB_HID_TASK_H_

#include <stdint.h>

typedef struct {
    uint8_t message_type : 8;
    uint8_t ctrl_num     : 8;
    uint16_t value       : 16;
}usb_hid_msg_t;

typedef union {
    uint8_t       bytes[4];
    usb_hid_msg_t bitfield;
    uint32_t      raw;
}usb_hid_msg_u;

void init_usb_hid(uint32_t page_index);

void usb_hid_task(void* parm);

void usb_hid_send(usb_hid_msg_u msg);
void usb_hid_send_from_isr(usb_hid_msg_u msg);

// GUI functions
void usbhid_drawpage(void);
void usbhid_drawdata(void);
void usbhid_drawinput(int character);


#endif /* USB_HID_TASK_H_ */
