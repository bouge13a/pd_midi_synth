/*
 * ADC_task.c
 *
 *  Created on: Aug 5, 2020
 *      Author: steph
 */

#include "ADC_task.h"
#include "ADC_pins.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#include "driverlib/inc/hw_ints.h"
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

static uint32_t adc00_step_values[8];
static uint32_t adc01_step_values[8];

static const uint32_t ADC_PERIOD_MS = 10;

static void ADCSeq0Handler(void);
static void ADCSeq1Handler(void);

EventGroupHandle_t adc_event = NULL;

void init_adc(void) {

    int idx;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));

    ADCSequenceDisable(ADC0_BASE, 0);
    ADCSequenceDisable(ADC0_BASE, 1);

    for(idx=0; idx<adc_pin_struct.num_adc_pins; idx++) {

        ADCSequenceConfigure(ADC0_BASE,
                             adc_pin_struct.adc_pins[idx]->sequencer,
                             ADC_TRIGGER_PROCESSOR,
                             0);

        SysCtlPeripheralEnable(adc_pin_struct.adc_pins[idx]->gpio_peripheral);

        SysCtlGPIOAHBEnable(adc_pin_struct.adc_pins[idx]->gpio_peripheral);

        GPIOPinTypeADC(adc_pin_struct.adc_pins[idx]->gpio_base,
                       adc_pin_struct.adc_pins[idx]->gpio_pin);

        ADCSequenceStepConfigure(ADC0_BASE,
                                 adc_pin_struct.adc_pins[idx]->sequencer,
                                 adc_pin_struct.adc_pins[idx]->step,
                                 adc_pin_struct.adc_pins[idx]->channel);

    }

    ADCSequenceStepConfigure(ADC0_BASE,
                             0,
                             adc_pin_struct.adc_pins[idx-1]->step,
                             adc_pin_struct.adc_pins[idx-1]->channel | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceStepConfigure(ADC0_BASE,
                             1,
                             adc_pin_struct.adc_pins[idx-1]->step,
                             adc_pin_struct.adc_pins[idx-1]->channel | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceEnable(ADC0_BASE, 0);
    ADCSequenceEnable(ADC0_BASE, 1);

    // Clear the interrupt status flag.  This is done to make sure the
    // interrupt flag is cleared before we sample.
    ADCIntClear(ADC0_BASE, 0);
    ADCIntClear(ADC0_BASE, 1);

    // Enable the ADC 0 sample sequence 0 interrupt.
    ADCIntEnable(ADC0_BASE, 0);
    ADCIntEnable(ADC0_BASE, 1);

    // Enable the interrupt for ADC0 sequence 0 on the processor (NVIC).
    IntEnable(INT_ADC0SS0);
    IntEnable(INT_ADC0SS1);

    IntPrioritySet(INT_ADC0SS0, 195);
    IntPrioritySet(INT_ADC0SS1, 196);

    ADCIntRegister(ADC0_BASE, 0, ADCSeq0Handler);
    //ADCIntRegister(ADC0_BASE, 0, ADCSeq1Handler);

    adc_event = xEventGroupCreate();
} // End init_adc

adc_pin_t* get_adc_config(const char* name) {

    int idx;

    for(idx=0; idx<adc_pin_struct.num_adc_pins; idx++) {

        if(0 == strcmp(name, adc_pin_struct.adc_pins[idx]->name)){
            return adc_pin_struct.adc_pins[idx];
        }

    }

    assert(0);
    return NULL;
} // End get_adc_config

uint32_t get_adc_val(adc_pin_t* config) {

    if (0 == config->sequencer) {
        return adc00_step_values[config->step];
    } else {
        return adc01_step_values[config->step];
    }


} // End get_adc_val

adc_pins_t get_adc_struct(void) {
    return adc_pin_struct;
} // End get_adc_struct

static void ADCSeq0Handler(void) {

    xEventGroupSetBitsFromISR(adc_event,
                              0x1,
                              pdFALSE);

    // Clear the Interrupt Flag.
    ADCIntClear(ADC0_BASE, 0);

} // End ADCSeq0Handler

static void ADCSeq1Handler(void) {

    xEventGroupSetBitsFromISR(adc_event,
                               0x2,
                               pdFALSE);

    //Clear the Interrupt Flag.
    ADCIntClear(ADC0_BASE, 1);

} // End ADCSeq0Handler

void adc_task(void* parm) {

    uint32_t event_bits;

    while(1) {

        // Trigger the ADC conversion.
        ADCProcessorTrigger(ADC0_BASE, 0);
        ADCProcessorTrigger(ADC0_BASE, 1);

        event_bits = xEventGroupWaitBits(adc_event,
                                         0x3,
                                         pdTRUE,
                                         pdTRUE,
                                         portMAX_DELAY);

        ADCSequenceDataGet(ADC0_BASE, 0, adc00_step_values);
        ADCSequenceDataGet(ADC0_BASE, 1, adc01_step_values);

        vTaskDelay(ADC_PERIOD_MS);
    }

} // End task
