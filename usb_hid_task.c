/*
 * usb_hid_task.c
 *
 *  Created on: Apr 15, 2021
 *      Author: steph
 */



#include "usb_hid_task.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdbool.h>
#include <uartstdio.h>

#include "driverlib/inc/hw_memmap.h"
#include "driverlib/inc/hw_types.h"
#include "driverlib/inc/hw_gpio.h"
#include "driverlib/inc/hw_sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "usb/usbdhidgamepad.h"
#include "usb_structs.h"

#include "logger.h"
#include "board_pin_defs.h"
#include "console_task.h"

static error_t* usb_reconnected_err;
static error_t* usb_suspended_err;

static uint32_t page_idx;

static QueueHandle_t usb_hid_q = NULL;

// This enumeration holds the various states that the gamepad can be in during
// normal operation.
static volatile enum {

    // Not yet configured.
    eStateNotConfigured,

    // Connected and not waiting on data to be sent.
    eStateIdle,

    // Suspended.
    eStateSuspend,

    // Connected and waiting on data to be sent out.
    eStateSending
} g_iGamepadState;

// The HID gamepad report that is returned to the host.
static tGamepadReport sReport;

// Handles asynchronous events from the HID gamepad driver.
//
// \param pvCBData is the event callback pointer provided during
// USBDHIDGamepadInit().  This is a pointer to our gamepad device structure
// (&g_sGamepadDevice).
// \param ui32Event identifies the event we are being called back for.
// \param ui32MsgData is an event-specific value.
// \param pvMsgData is an event-specific pointer.
//
// This function is called by the HID gamepad driver to inform the application
// of particular asynchronous events related to operation of the gamepad HID
// device.
//
// \return Returns 0 in all cases.
uint32_t GamepadHandler(void *pvCBData,
                        uint32_t ui32Event,
                        uint32_t ui32MsgData,
                        void *pvMsgData) {

    switch (ui32Event)
    {
        // The host has connected to us and configured the device.
        case USB_EVENT_CONNECTED: {

            g_iGamepadState = eStateIdle;

            break;
        }

        // The host has disconnected from us.
        case USB_EVENT_DISCONNECTED: {

            g_iGamepadState = eStateNotConfigured;

            set_error(usb_reconnected_err);

            break;
        }


        // This event occurs every time the host acknowledges transmission
        // of a report.  It is to return to the idle state so that a new report
        // can be sent to the host.
        case USB_EVENT_TX_COMPLETE: {

            // Enter the idle state since we finished sending something.
            g_iGamepadState = eStateIdle;

            break;
        }

        // This event indicates that the host has suspended the USB bus.
        case USB_EVENT_SUSPEND: {

            // Go to the suspended state.
            g_iGamepadState = eStateSuspend;

            set_error(usb_suspended_err);

            break;
        }

        // This event signals that the host has resumed signaling on the bus.
        case USB_EVENT_RESUME: {

            // Go back to the idle state.
            g_iGamepadState = eStateIdle;

            break;
        }

        // Return the pointer to the current report.  This call is
        // rarely if ever made, but is required by the USB HID
        // specification.
        case USBD_HID_EVENT_GET_REPORT: {

            *(void **)pvMsgData = (void *)&sReport;

            break;
        }

        // We ignore all other events.
        default: {
            break;
        }
    }

    return(0);
}


void init_usb_hid(uint32_t page_index) {

    page_idx = page_index;

    // Not configured initially.
    g_iGamepadState = eStateNotConfigured;

    // Enable the GPIO peripheral used for USB, and configure the USB
    // pins.
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));

    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOD);
    MAP_GPIOPinTypeUSBAnalog(GPIO_PORTD_AHB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    //USBIntRegister(USB0_BASE, USB0DeviceIntHandler);

    // Set the USB stack mode to Device mode.
    USBStackModeSet(0, eUSBModeForceDevice, 0);

    // Pass the device information to the USB library and place the device
    // on the bus.
    USBDHIDGamepadInit(0, &g_sGamepadDevice);

    usb_reconnected_err = create_error("USB0",
                                       "USB has reconnected");

    usb_suspended_err = create_error("USB0",
                                     "Host has suspended bus");

    usb_hid_q = xQueueCreate(20, sizeof(uint32_t));


} // End init_usb_hid

void usb_hid_task(void* parm) {

    usb_hid_msg_u usb_hid_msg;

    while(1) {

        // Wait for state to be idle
        if(g_iGamepadState == eStateIdle) {

            xQueueReceive(usb_hid_q, &usb_hid_msg, portMAX_DELAY);

            sReport.msg_type     = usb_hid_msg.bitfield.message_type;
            sReport.ctrl_num     = usb_hid_msg.bitfield.ctrl_num;
            sReport.value_msb    = usb_hid_msg.bitfield.value;

            // Send sReport
            USBDHIDGamepadSendReport(&g_sGamepadDevice,
                                     &sReport,
                                     sizeof(sReport));

            IntMasterDisable();
            g_iGamepadState = eStateSending;
            IntMasterEnable();

            if (is_on_screen(page_idx)) {
                UARTprintf("%d %d %d", sReport.msg_type,
                                       sReport.ctrl_num,
                                       sReport.value_msb);
            }
        }

        vTaskDelay(0);

    }
} // End init_usb_hid

void usb_hid_send(usb_hid_msg_u msg) {
    xQueueSend(usb_hid_q, &msg, 0);
}

void usb_hid_send_from_isr(usb_hid_msg_u msg) {
    xQueueSendFromISR(usb_hid_q, &msg, 0);
}

// GUI functions
void usbhid_drawpage(void) {

} // End usbhid_drawpage

void usbhid_drawdata(void) {

} // End usbhid_drawdata

void usbhid_drawinput(int character) {

} // End usbhid_drawinput
