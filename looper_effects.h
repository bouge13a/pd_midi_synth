/*
 * looper_effects.h
 *
 *  Created on: Apr 20, 2021
 *      Author: steph
 */

#ifndef LOOPER_EFFECTS_H_
#define LOOPER_EFFECTS_H_

#include <stdint.h>

void init_looper_effects(void);

void process_effects(uint32_t* adc00values,
                     uint32_t* adc11values);

#endif /* LOOPER_EFFECTS_H_ */
