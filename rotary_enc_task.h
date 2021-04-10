/*
 * rotary_encoder.h
 *
 *  Created on: Apr 9, 2021
 *      Author: steph
 */

#ifndef ROTARY_ENC_TASK_H_
#define ROTARY_ENC_TASK_H_

void init_rotary_enc(void);

void rotary_enc_task(void* parm);

uint32_t get_channel(void);

#endif /* ROTARY_ENC_TASK_H_ */
