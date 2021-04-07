/*
 * drum_pad_function.h
 *
 *  Created on: Apr 5, 2021
 *      Author: steph
 */

#ifndef DRUM_PAD_FUNCTIONS_H_
#define DRUM_PAD_FUNCTIONS_H_

#include <stdint.h>

void init_drumpad(uint32_t page_number);

void process_drumpad(uint32_t* adc00values, uint32_t* adc11values);

void slope_drawpage(void);
void slope_drawdata(void);
void slope_drawinput(int character);

#endif /* DRUM_PAD_FUNCTIONS_H_ */
