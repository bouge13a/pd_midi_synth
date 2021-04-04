/*
 * comp_ints.h
 *
 *  Created on: Apr 3, 2021
 *      Author: steph
 */

#ifndef COMP_INTS_H_
#define COMP_INTS_H_

#include <stdint.h>

void init_comp_ints(uint32_t page_num);

void drumpad_drawpage(void);
void drumpad_drawdata(void);
void drumpad_drawinput(int character);

#endif /* COMP_INTS_H_ */
