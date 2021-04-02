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

static const uint32_t ADC_PERIOD_MS = 10;

static void ADCSeq0Handler(void);
static void ADCSeq1Handler(void);
static void ADCSeq2Handler(void);

static uint32_t adc00_step_values[8];
static uint32_t adc01_step_values[4];
static uint32_t adc02_step_values[4];

static EventGroupHandle_t adc_event = NULL;

static const uint32_t BIT_0 = 1;
static const uint32_t BIT_1 = 1 << 1;
static const uint32_t BIT_2 = 1 << 2;

void init_adc(void) {

    int idx;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));

    ADCSequenceDisable(ADC0_BASE, 0);
    ADCSequenceDisable(ADC0_BASE, 1);
    ADCSequenceDisable(ADC0_BASE, 2);

    for(idx=0; idx<adc_pin_struct.num_adc_pins; idx++) {

        ADCSequenceConfigure(ADC0_BASE,
                             adc_pin_struct.adc_pins[idx]->sequencer,
                             ADC_TRIGGER_PROCESSOR,
                             (idx<=5? 0 : 1));

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
                             adc_pin_struct.adc_pins[5]->step,
                             adc_pin_struct.adc_pins[5]->channel | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceStepConfigure(ADC0_BASE,
                             1,
                             adc_pin_struct.adc_pins[3]->step,
                             adc_pin_struct.adc_pins[3]->channel | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceStepConfigure(ADC0_BASE,
                             2,
                             adc_pin_struct.adc_pins[3]->step,
                             adc_pin_struct.adc_pins[3]->channel | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceEnable(ADC0_BASE, 0);
    ADCSequenceEnable(ADC0_BASE, 1);
    ADCSequenceEnable(ADC0_BASE, 2);

    // Clear the interrupt status flag.  This is done to make sure the
    // interrupt flag is cleared before we sample.
    ADCIntClear(ADC0_BASE, 0);
    ADCIntClear(ADC0_BASE, 1);
    ADCIntClear(ADC0_BASE, 2);

    IntPrioritySet(INT_ADC0SS0, configMAX_SYSCALL_INTERRUPT_PRIORITY + 1);
    IntPrioritySet(INT_ADC0SS1, configMAX_SYSCALL_INTERRUPT_PRIORITY + 1);
    IntPrioritySet(INT_ADC0SS2, configMAX_SYSCALL_INTERRUPT_PRIORITY + 1);

    // Enable the ADC 0 sample sequence 0 interrupt.
    ADCIntEnable(ADC0_BASE, 0);
    ADCIntEnable(ADC0_BASE, 1);
    ADCIntEnable(ADC0_BASE, 2);

    // Enable the interrupt for ADC0 sequence 0 on the processor (NVIC).
    IntEnable(INT_ADC0SS0);
    IntEnable(INT_ADC0SS1);
    IntEnable(INT_ADC0SS2);

    ADCIntRegister(ADC0_BASE, 0, ADCSeq0Handler);
    ADCIntRegister(ADC0_BASE, 1, ADCSeq1Handler);
    ADCIntRegister(ADC0_BASE, 2, ADCSeq2Handler);

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
    } else if (1 == config->sequencer){
        return adc01_step_values[config->step];
    } else if (2 == config->sequencer){
        return adc02_step_values[config->step];
    } else {
        assert(0);
    }


} // End get_adc_val

adc_pins_t get_adc_struct(void) {
    return adc_pin_struct;
} // End get_adc_struct

static void ADCSeq0Handler(void) {

    BaseType_t xHigherPriorityTaskWoken, xResult;

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

static void ADCSeq1Handler(void) {

    BaseType_t xHigherPriorityTaskWoken, xResult;

    // xHigherPriorityTaskWoken must be initialised to pdFALSE.
    xHigherPriorityTaskWoken = pdFALSE;

    //Clear the Interrupt Flag.
    ADCIntClear(ADC0_BASE, 1);

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

static void ADCSeq2Handler(void) {

    BaseType_t xHigherPriorityTaskWoken, xResult;

    // xHigherPriorityTaskWoken must be initialised to pdFALSE.
    xHigherPriorityTaskWoken = pdFALSE;

    //Clear the Interrupt Flag.
    ADCIntClear(ADC0_BASE, 2);

    xResult = xEventGroupSetBitsFromISR(adc_event,
                               BIT_2,
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

    while(1) {

        // Trigger the ADC conversion.
        ADCProcessorTrigger(ADC0_BASE, 0);
        ADCProcessorTrigger(ADC0_BASE, 1);
        ADCProcessorTrigger(ADC0_BASE, 2);

        xEventGroupWaitBits(adc_event,
                            BIT_0 | BIT_1 | BIT_2,
                            pdTRUE,
                            pdTRUE,
                            portMAX_DELAY);

        ADCSequenceDataGet(ADC0_BASE, 0, adc00_step_values);
        ADCSequenceDataGet(ADC0_BASE, 1, adc01_step_values);
        ADCSequenceDataGet(ADC0_BASE, 2, adc02_step_values);

        vTaskDelay(ADC_PERIOD_MS);
    }

} // End task
