/*
 * comp_ints.c
 *
 *  Created on: Apr 2, 2021
 *      Author: steph
 */


#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "driverlib/inc/hw_ints.h"
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

#include "ADC_task.h"
#include "board_pin_defs.h"

static const uint32_t TIMER0_PRESCALE = 16000000/16000;

void Comp0IntHandler(void);
void Comp1IntHandler(void);

typedef enum {
    LOW,
    MEDIUM,
    HIGH,
}comp_state_e;

typedef struct {
    comp_state_e previous_state;
    comp_state_e current_state;
    uint32_t     low_time;
    uint32_t     high_time;
    adc_pin_t*   adc_config;
}comp_state_t;

static const uint32_t BIT_0 = 1;
static const uint32_t BIT_1 = 1 << 1;
static const uint32_t BIT_2 = 2 << 1;
static const uint32_t BIT_3 = 3 << 1;
static const uint32_t BIT_4 = 4 << 1;
static const uint32_t BIT_5 = 5 << 1;
static const uint32_t BIT_6 = 6 << 1;
static const uint32_t BIT_7 = 7 << 1;
static const uint32_t BIT_8 = 8 << 1;

static comp_state_t comp_states[12];


void init_comp_ints(void) {

    uint32_t idx;

    for(idx=0; idx<12; idx++) {
        comp_states[idx].previous_state = MEDIUM;
        comp_states[idx].current_state  = LOW;
    }

    comp_states[0].adc_config = get_adc_config("pad 0");
    comp_states[1].adc_config = get_adc_config("pad 1");
    comp_states[2].adc_config = get_adc_config("pad 2");
    comp_states[3].adc_config = get_adc_config("pad 3");
    comp_states[4].adc_config = get_adc_config("pad 4");
    comp_states[5].adc_config = get_adc_config("pad 5");
    comp_states[6].adc_config = get_adc_config("pad 6");
    comp_states[7].adc_config = get_adc_config("pad 7");
    comp_states[8].adc_config = get_adc_config("pad 8");
    comp_states[9].adc_config = get_adc_config("pad 9");
    comp_states[10].adc_config = get_adc_config("pad 10");
    comp_states[11].adc_config = get_adc_config("pad 11");

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC_UP);

    TimerPrescaleSet(TIMER0_BASE,
                     TIMER_BOTH,
                     TIMER0_PRESCALE);

    TimerEnable(TIMER0_BASE,
                TIMER_BOTH);

} // End init_comp_ints

void process_interrupt(uint32_t status, uint32_t base) {

    uint32_t note;

    if (base == ADC0_BASE) {
        switch(status) {
            case BIT_0 : note = 0;
                break;
            case BIT_1 : note = 1;
                break;
            case BIT_2 : note = 2;
                break;
            case BIT_3 : note = 3;
                break;
            case BIT_4 : note = 4;
                break;
            case BIT_5 : note = 5;
                break;
            case BIT_6 : note = 6;
                break;
            case BIT_7 : note = 7;
                break;
            default:
                assert(0);
        }
    } else if (ADC1_BASE == base) {
        switch(status) {
            case BIT_1 : note = 8;
                break;
            case BIT_2 : note = 9;
                break;
            case BIT_3 : note = 10;
                break;
            case BIT_4 : note = 11;
                break;
            default :
                assert(0);
                break;
        }
    } else {
        assert(0);
    }




} // End process_interrupt

void Comp0IntHandler(void) {

    uint32_t status = 0;

    ADCComparatorIntDisable(ADC0_BASE, 0);

    status = ADCComparatorIntStatus(ADC0_BASE);

    process_interrupt(status, ADC0_BASE);

    ADCComparatorIntEnable(ADC0_BASE, 0);

} // End Comp0IntHandler

void Comp1IntHandler(void) {

    uint32_t status = 0;

    ADCComparatorIntDisable(ADC1_BASE, 1);

    status = ADCComparatorIntStatus(ADC1_BASE);

    process_interrupt(status, ADC1_BASE);

    ADCComparatorIntEnable(ADC1_BASE, 1);

} // End Comp1IntHandler
