/*
 * usb_structs.h
 *
 *  Created on: Aug 10, 2020
 *      Author: steph
 */

#ifndef USB_STRUCTS_H_
#define USB_STRUCTS_H_


extern uint32_t GamepadHandler(void *pvCBData, uint32_t ui32Event,
                               uint32_t ui32MsgData, void *pvMsgData);

extern tUSBDHIDGamepadDevice g_sGamepadDevice;


#endif /* USB_STRUCTS_H_ */
