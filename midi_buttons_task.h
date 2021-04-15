/*
 * midi_buttons.h
 *
 *  Created on: Apr 15, 2021
 *      Author: steph
 */

#ifndef MIDI_BUTTONS_TASK_H_
#define MIDI_BUTTONS_TASK_H_

#include <stdint.h>
#include <stdbool.h>

void init_midi_buttons(void);

void midi_buttons_task(void* parm);

uint32_t get_octave(void);

bool pressure_sense_is_on(void);

#endif /* MIDI_BUTTONS_TASK_H_ */
