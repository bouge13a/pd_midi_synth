/*
 * joystick_task.h
 *
 *  Created on: Apr 10, 2021
 *      Author: steph
 */

#ifndef JOYSTICK_TASK_H_
#define JOYSTICK_TASK_H_

#include <stdint.h>

void init_ads1x15(void);

void ads1x15_midi_task(void* parm);
void ads1x15_looper_task(void* parm);

float get_volume(void);
float get_sensitivity(void);

void ads1x15_drawpage(void);
void ads1x15_drawdata(void);
void ads1x15_drawinput(int character);

#endif /* JOYSTICK_TASK_H_ */
