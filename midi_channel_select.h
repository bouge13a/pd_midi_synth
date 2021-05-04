/*
 * midi_channel_select.h
 *
 *  Created on: May 4, 2021
 *      Author: steph
 */

#ifndef MIDI_CHANNEL_SELECT_H_
#define MIDI_CHANNEL_SELECT_H_

#include <stdint.h>

void init_midi_channel_select(void);

void midi_channel_select_task(void* parm);

uint32_t get_channel(void);

#endif /* MIDI_CHANNEL_SELECT_H_ */
