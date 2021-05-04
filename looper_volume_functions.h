/*
 * looper_volume_functions.h
 *
 *  Created on: Apr 20, 2021
 *      Author: steph
 */

#ifndef LOOPER_VOLUME_FUNCTIONS_H_
#define LOOPER_VOLUME_FUNCTIONS_H_

#include <stdint.h>


void init_looper_volumes(void);

void process_looper_volumes(uint32_t* adc0_data,
                            uint32_t* adc1_data);

void send_volumes(void);

#endif /* LOOPER_VOLUME_FUNCTIONS_H_ */
