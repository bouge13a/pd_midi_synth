/*
 * ADC_task.h
 *
 *  Created on: Aug 5, 2020
 *      Author: steph
 */

#ifndef ADC_TASK_H_
#define ADC_TASK_H_

#include "board_pin_defs.h"

void init_adc(void);

adc_pin_t* get_adc_config(const char* name);

uint32_t get_adc_val(adc_pin_t* config);

adc_pins_t get_adc_struct(void);

void adc_task(void* parm);


#endif /* ADC_TASK_H_ */
