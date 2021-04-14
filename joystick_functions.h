/*
 * joystick_functions.h
 *
 *  Created on: Apr 12, 2021
 *      Author: steph
 */

#ifndef JOYSTICK_FUNCTIONS_H_
#define JOYSTICK_FUNCTIONS_H_

#include <stdint.h>
#include "utils.h"

void process_joystick(uint16_t x_raw, uint16_t y_raw);

#endif /* JOYSTICK_FUNCTIONS_H_ */
