/*
 * joystick_task.h
 *
 *  Created on: Apr 10, 2021
 *      Author: steph
 */

#ifndef JOYSTICK_TASK_H_
#define JOYSTICK_TASK_H_

void init_joystick(void);

void joystick_task(void* parm);

void joystick_drawpage(void);
void joystick_drawdata(void);
void joystick_drawinput(int character);

#endif /* JOYSTICK_TASK_H_ */
