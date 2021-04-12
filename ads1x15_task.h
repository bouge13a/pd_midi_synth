/*
 * joystick_task.h
 *
 *  Created on: Apr 10, 2021
 *      Author: steph
 */

#ifndef JOYSTICK_TASK_H_
#define JOYSTICK_TASK_H_

void init_ads1x15(void);

void ads1x15_midi_task(void* parm);

void ads1x15_drawpage(void);
void ads1x15_drawdata(void);
void ads1x15_drawinput(int character);

#endif /* JOYSTICK_TASK_H_ */
