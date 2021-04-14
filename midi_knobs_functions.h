/*
 * midi_knobs_task.h
 *
 *  Created on: Apr 14, 2021
 *      Author: steph
 */

#ifndef MIDI_KNOBS_FUNCTIONS_H_
#define MIDI_KNOBS_FUNCTIONS_H_

#include "utils.h"

void init_midi_knobs(void);

void process_midi_knobs(volatile value16_t* knobs);

#endif /* MIDI_KNOBS_FUNCTIONS_H_ */
