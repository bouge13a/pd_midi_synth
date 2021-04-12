/*
 * I2C_task.h
 *
 *  Created on: Aug 6, 2020
 *      Author: steph
 */

#ifndef I2C_TASK_H_
#define I2C_TASK_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    I2C_IDLE,
    I2C_SEND,
    I2C_RECEIVE_START,
    I2C_RECEIVE,
    I2C_FINISH,
    I2C_ERROR,
}i2c_state_e;

typedef enum {
    i2c_ready,
    i2c_processing,
}i2c_msg_state_e;

typedef struct {
    uint8_t address;
    const uint8_t* tx_data;
    uint32_t num_tx_bytes;
    uint32_t bytes_txed;
    volatile uint8_t* rx_data;
    uint32_t num_rx_bytes;
    uint32_t bytes_rxed;
    i2c_msg_state_e state;
}i2c_msg_t;

void init_i2c(void);

bool add_i2c_msg(i2c_msg_t* i2c_msg);

void i2c_task(void* parm);

#endif /* I2C_TASK_H_ */
