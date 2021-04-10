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

#include "driverlib/timer.h"
#include "driverlib/inc/hw_ints.h"
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

#include "drum_pad_functions.h"

#define COMP_LOW_REF   300
#define COMP_HIGH_REF 1000

void ADCSeq0Handler(void);
void ADCSeq1Handler(void);

static uint32_t adc00_step_values[8];
static uint32_t adc11_step_values[4];

static EventGroupHandle_t adc_event = NULL;

static const uint32_t BIT_0 = 1;
static const uint32_t BIT_1 = 1 << 1;
static volatile uint32_t adc00time;
static volatile uint32_t adc11time;

void init_adc(void) {

    int idx;

    adc_event = xEventGroupCreate();

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_COMP0);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC1));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_COMP0));

    ADCSequenceDisable(ADC0_BASE, 0);
    ADCSequenceDisable(ADC1_BASE, 1);

    for(idx=0; idx<adc_pin_struct.num_adc_pins; idx++) {

        ADCSequenceConfigure(adc_pin_struct.adc_pins[idx]->adc_base,
                             adc_pin_struct.adc_pins[idx]->sequencer,
                             ADC_TRIGGER_PROCESSOR,
                             1);

        SysCtlPeripheralEnable(adc_pin_struct.adc_pins[idx]->gpio_peripheral);

        SysCtlGPIOAHBEnable(adc_pin_struct.adc_pins[idx]->gpio_peripheral);

        GPIOPinTypeADC(adc_pin_struct.adc_pins[idx]->gpio_base,
                       adc_pin_struct.adc_pins[idx]->gpio_pin);

        ADCSequenceStepConfigure(adc_pin_struct.adc_pins[idx]->adc_base,
                                 adc_pin_struct.adc_pins[idx]->sequencer,
                                 adc_pin_struct.adc_pins[idx]->step,
                                 adc_pin_struct.adc_pins[idx]->channel);

    }

    ADCSequenceStepConfigure(ADC0_BASE,
                             0,
                             adc_pin_struct.adc_pins[7]->step,
                             adc_pin_struct.adc_pins[7]->channel | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceStepConfigure(ADC1_BASE,
                             1,
                             adc_pin_struct.adc_pins[11]->step,
                             adc_pin_struct.adc_pins[11]->channel | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceEnable(ADC0_BASE, 0);
    ADCSequenceEnable(ADC1_BASE, 1);

    // Clear the interrupt status flag.  This is done to make sure the
    // interrupt flag is cleared before we sample.
    ADCIntClear(ADC0_BASE, 0);
    ADCIntClear(ADC1_BASE, 1);

    // Set the priority "below" max syscall because of FR ISR routine
    IntPrioritySet(INT_ADC0SS0, configMAX_SYSCALL_INTERRUPT_PRIORITY + 1);
    IntPrioritySet(INT_ADC1SS1, configMAX_SYSCALL_INTERRUPT_PRIORITY + 1);

    // Enable the ADC 0 sample sequence 0 interrupt.
    ADCIntEnable(ADC0_BASE, 0);
    ADCIntEnable(ADC1_BASE, 1);

    // Enable the interrupt for ADC0 sequence 0 on the processor (NVIC).
    IntEnable(INT_ADC0SS0);
    IntEnable(INT_ADC1SS1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1));

    TimerClockSourceSet(TIMER1_BASE, TIMER_CLOCK_PIOSC);

    TimerDisable(TIMER1_BASE, TIMER_BOTH);

    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

    TimerEnable(TIMER1_BASE, TIMER_BOTH);


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
    } else if (1 == config->sequencer){
        return adc11_step_values[config->step];
    } else {
        assert(0);
    }

    return 0;
} // End get_adc_val

adc_pins_t get_adc_struct(void) {
    return adc_pin_struct;
} // End get_adc_struct

void ADCSeq0Handler(void) {

    BaseType_t xHigherPriorityTaskWoken, xResult;

    adc00time = TimerValueGet(TIMER1_BASE, TIMER_A);

    // xHigherPriorityTaskWoken must be initialised to pdFALSE.
    xHigherPriorityTaskWoken = pdFALSE;

    // Clear the Interrupt Flag.
    ADCIntClear(ADC0_BASE, 0);

    xResult = xEventGroupSetBitsFromISR(adc_event,
                              BIT_0,
                              &xHigherPriorityTaskWoken);

    // Was the message posted successfully?
    if( xResult != pdFAIL ) {
        /* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
        switch should be requested.  The macro used is port specific and will
        be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
        the documentation page for the port being used. */
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }

} // End ADCSeq0Handler

void ADCSeq1Handler(void) {

    BaseType_t xHigherPriorityTaskWoken, xResult;

    adc11time = TimerValueGet(TIMER1_BASE, TIMER_A);

    // xHigherPriorityTaskWoken must be initialised to pdFALSE.
    xHigherPriorityTaskWoken = pdFALSE;

    //Clear the Interrupt Flag.
    ADCIntClear(ADC1_BASE, 1);

    xResult = xEventGroupSetBitsFromISR(adc_event,
                               BIT_1,
                               &xHigherPriorityTaskWoken);

    // Was the message posted successfully?
    if( xResult != pdFAIL ) {
        /* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
        switch should be requested.  The macro used is port specific and will
        be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
        the documentation page for the port being used. */
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }

} // End ADCSeq1Handler

void adc_task(void* parm) {

    // Trigger the ADC conversion.
    ADCProcessorTrigger(ADC0_BASE, 0);
    ADCProcessorTrigger(ADC1_BASE, 1);

    while(1) {

        xEventGroupWaitBits(adc_event,
                            BIT_0 | BIT_1,
                            pdTRUE,
                            pdTRUE,
                            portMAX_DELAY);

        ADCSequenceDataGet(ADC0_BASE, 0, adc00_step_values);
        ADCSequenceDataGet(ADC1_BASE, 1, adc11_step_values);

        process_drumpad(adc00_step_values,
                        adc11_step_values,
                        adc00time,
                        adc11time);

        // Trigger the ADC conversion.
        ADCProcessorTrigger(ADC0_BASE, 0);
        ADCProcessorTrigger(ADC1_BASE, 1);

        vTaskDelay(0);
    }

} // End task

