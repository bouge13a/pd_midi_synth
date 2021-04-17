/*
 * looper_buttons.c
 *
 *  Created on: Apr 16, 2021
 *      Author: steph
 */


#include <looper_buttons_task.h>
#include "GPIs.h"
#include "board_pin_defs.h"

typedef enum {
    BUTTON_UP_IDLE,
    BUTTON_DOWN_START,
    BUTTON_UP_START,
    BUTTON_DOWN_PLAY,
    BUTTON_UP_PLAY,
    BUTTON_DOWN_IDLE,
} loop_state_e;

typedef struct {
    gpio_pin_t* loop_pin;
    gpio_pin_t* led;
    loop_state_e loop_state;
}looper_pin_t;

static looper_pin_t looper_pins[8];

void init_looper_buttons(void) {

} // End init_looper_buttons

void looper_buttons_task(void* parm) {

} // End looper_buttons_task
