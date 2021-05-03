/*
 * looper_buttons.h
 *
 *  Created on: Apr 16, 2021
 *      Author: steph
 */

#ifndef LOOPER_BUTTONS_TASK_H_
#define LOOPER_BUTTONS_TASK_H_

typedef enum {
    RECORD_START,
    PLAY_START,
    PLAY_STOP
}loop_msg_e;

void init_looper_buttons(void);

void looper_buttons_task(void* parm);

#endif /* LOOPER_BUTTONS_TASK_H_ */
