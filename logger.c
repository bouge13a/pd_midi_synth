/*
 * logger.c
 *
 *  Created on: Aug 7, 2020
 *      Author: steph
 */

#include <stdint.h>
#include <uartstdio.h>
#include <assert.h>
#include "logger.h"
#include "GPOs.h"
#include "board_pin_defs.h"

#include "text_controls.h"

static const uint32_t MAX_NUM_ERRORS = 20;
static error_t errors[20];
static uint32_t num_of_errors = 0;

gpio_pin_t* red_debug;
gpio_pin_t* green_debug;
gpio_pin_t* blue_debug;

error_t* create_error(const char* name,
                      const char* info) {

    if( num_of_errors + 1 > MAX_NUM_ERRORS) {
        assert(0);
    }

    errors[num_of_errors].name = name;
    errors[num_of_errors].info = info;
    errors[num_of_errors].occurences = 0;
    num_of_errors++;

    return &errors[num_of_errors-1];

} // End init_logger

void set_error(error_t* error) {

    error->occurences++;

} // End set_error

void init_logger(void) {

    red_debug = get_gpo_config("red debug");
    green_debug = get_gpo_config("green debug");
    blue_debug = get_gpo_config("blue debug");

    set_gpo(green_debug, 1);
}

///////////////////////////////////////////////////////////
//                Console Page
//////////////////////////////////////////////////////////

static const uint32_t INFO_COL  = 17;
static const uint32_t OCCUR_COL = 60;


void logger_drawpage(void){

    cursor_pos(5, 0);
    set_text_mode(mode_underline);
    UARTprintf("NAME");

    cursor_pos(5, INFO_COL);
    UARTprintf("INFORMATION");

    cursor_pos(5, OCCUR_COL);
    UARTprintf("OCCURANCES");
    set_text_mode(mode_underline_off);

} // End logger_drawpage

void logger_drawdata(void){

    uint32_t idx;
    uint32_t printed_errors = 0;

    text_color(bright_red_text);

    for (idx=0; idx < num_of_errors; idx++) {

        if (errors[idx].occurences > 0) {

            cursor_pos(6+printed_errors, 0);
            UARTprintf("%s", errors[idx].name);
            cursor_pos(6+printed_errors,INFO_COL);
            UARTprintf("%s", errors[idx].info);
            cursor_pos(6+printed_errors,OCCUR_COL);
            UARTprintf("%d", errors[idx].occurences);
            printed_errors++;

            set_gpo(green_debug, 0);
            set_gpo(red_debug, 1);

        }
    }

    text_color(white_text);

} // End logger_drawdata

void logger_drawinput(int character){

} // End logger_drawinput
