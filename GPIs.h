/*
 * buttons.h
 *
 *  Created on: Aug 4, 2020
 *      Author: steph
 */

#ifndef GPIS_H_
#define GPIS_H_

#include "board_pin_defs.h"

// Button functions
void init_gpis(void);
gpio_pin_t* get_gpi_config(const char* name);
uint32_t read_gpi(gpio_pin_t* config);
gpis_t get_gpi_struct(void);

#endif /* GPIS_H_ */
