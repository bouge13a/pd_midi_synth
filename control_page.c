/*
 * control_page.c
 *
 *  Created on: Aug 5, 2020
 *      Author: steph
 */

#include <GPIs.h>
#include <stdint.h>
#include <uartstdio.h>

#include "control_page.h"
#include "board_pin_defs.h"
#include "text_controls.h"
#include "ADC_task.h"

gpis_t gpis_info;
adc_pins_t adc_info;

static const uint32_t BUTTON_VAL_COL = 20;
static const uint32_t ADC_NAME_COL   = 30;
static const uint32_t ADC_VAL_COL    = 50;

///////////////////////////////////////////////////////////////////////
//                        Console Page
///////////////////////////////////////////////////////////////////////
void control_drawpage(void) {

    int idx;

    gpis_info     = get_gpi_struct();
    adc_info      = get_adc_struct();

    cursor_pos(5, 0);
    set_text_mode(mode_underline);
    UARTprintf("GPIs\r\n");
    set_text_mode(mode_reset);

    text_color(green_text);
    for(idx=0; idx<gpis_info.num_gpis; idx++) {
        UARTprintf("%s\r\n", gpis_info.gpis[idx]->name);
    }

    set_text_mode(mode_reset);

    cursor_pos(5, ADC_NAME_COL);
    set_text_mode(mode_underline);
    UARTprintf("ADCs");
    set_text_mode(mode_reset);


    text_color(magenta_text);
    for(idx=0; idx<adc_info.num_adc_pins; idx++) {
        cursor_pos(6+idx, ADC_NAME_COL);
        UARTprintf("%s", adc_info.adc_pins[idx]->name);
    }

    set_text_mode(mode_reset);
}

void control_drawdata(void) {

    int idx;

    for(idx=0; idx<gpis_info.num_gpis; idx++){
        cursor_pos(6+idx, BUTTON_VAL_COL);
        UARTprintf("%d", read_gpi(gpis_info.gpis[idx]));
    }

    for(idx=0; idx<adc_info.num_adc_pins; idx++){
        cursor_pos(6+idx, ADC_VAL_COL);
        UARTprintf("    ");
        cursor_pos(6+idx, ADC_VAL_COL);
        UARTprintf("%d", get_adc_val(adc_info.adc_pins[idx]));
    }

} // End control_drawdata

void control_drawinput(int character) {

}
