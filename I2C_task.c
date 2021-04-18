/*
 * I2C_task.c
 *
 *  Created on: Aug 6, 2020
 *      Author: steph
 */

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "I2C_task.h"

#include "logger.h"

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "driverlib/inc/hw_i2c.h"
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

QueueHandle_t i2c_msg_queue = NULL;

error_t* addr_ack_err;
error_t* data_ack_err;
error_t* arb_lost_err;
error_t* clk_tout_err;

static i2c_state_e i2c_state = I2C_IDLE;
static i2c_msg_t* i2c_msg;

static bool log_errors(void);

void init_i2c(void) {

    // The I2C0 peripheral must be enabled before use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));

    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);

    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);

    I2CMasterTimeoutSet(I2C1_BASE, 0x7d);

    i2c_msg_queue = xQueueCreate(20, sizeof(i2c_msg_t*));

    addr_ack_err = create_error("I2C1", "No ack from address");
    data_ack_err = create_error("I2C1", "No ack from data");
    arb_lost_err = create_error("I2C1", "Arbitration lost");
    clk_tout_err = create_error("I2C1", "Clock timeout");

} // End init_i2c

bool add_i2c_msg(i2c_msg_t* i2c_msg) {

    return xQueueSend(i2c_msg_queue, &i2c_msg, 0);
}

void i2c_task(void* parm) {

    while(1) {

        switch(i2c_state) {
        case I2C_IDLE:

            xQueueReceive(i2c_msg_queue, &i2c_msg, portMAX_DELAY);

            assert(i2c_msg);

            i2c_msg->state = i2c_processing;

            if(i2c_msg->num_tx_bytes > 0 ) {

                I2CMasterSlaveAddrSet(I2C1_BASE,
                                      i2c_msg->address,
                                      false);

            } else if (i2c_msg->num_rx_bytes > 0) {

                I2CMasterSlaveAddrSet(I2C1_BASE,
                                      i2c_msg->address,
                                      true);

            } else {

                // no rx or tx bytes
                assert(0);

            }

            if (1 == i2c_msg->num_tx_bytes) {

                I2CMasterDataPut(I2C1_BASE, i2c_msg->tx_data[0]);
                I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
                i2c_msg->bytes_txed++;
                i2c_state = I2C_RECEIVE_START;

            } else if (i2c_msg->num_tx_bytes > 1) {

                I2CMasterDataPut(I2C1_BASE, i2c_msg->tx_data[0]);
                I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
                i2c_msg->bytes_txed++;
                i2c_state = I2C_SEND;

            } else if (i2c_msg->num_rx_bytes > 1) {

                i2c_state = I2C_RECEIVE_START;

            } else if (1 == i2c_msg->num_rx_bytes) {

                i2c_state = I2C_RECEIVE_START;

            }


            break;

        case I2C_SEND:

            if (I2CMasterBusy(I2C1_BASE)) break;

            if(log_errors()){
                i2c_state = I2C_FINISH;
                break;
            }

            if (1 == i2c_msg->num_tx_bytes - i2c_msg->bytes_txed) {

                I2CMasterDataPut(I2C1_BASE, i2c_msg->tx_data[i2c_msg->bytes_txed]);
                I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
                i2c_msg->bytes_txed++;

                if (0 == i2c_msg->num_rx_bytes) {

                    i2c_state = I2C_FINISH;

                } else {

                    i2c_state = I2C_RECEIVE_START;

                }

            } else if (1 < i2c_msg->num_tx_bytes - i2c_msg->bytes_txed) {

                I2CMasterDataPut(I2C1_BASE, i2c_msg->tx_data[i2c_msg->bytes_txed]);
                I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
                i2c_msg->bytes_txed++;
                i2c_state = I2C_SEND;

            }

            break;

        case I2C_RECEIVE_START :

            if (I2CMasterBusy(I2C1_BASE)) break;

            if(log_errors()){
                i2c_state = I2C_FINISH;
                break;
            }

            I2CMasterSlaveAddrSet(I2C1_BASE,
                                  i2c_msg->address,
                                  true);

            if (i2c_msg->num_rx_bytes > 1) {

                I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
                i2c_state = I2C_RECEIVE;

            } else if (1 == i2c_msg->num_rx_bytes) {

                I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
                i2c_state = I2C_RECEIVE;

            }



            break;

        case I2C_RECEIVE :

            if (I2CMasterBusy(I2C1_BASE)) break;

            if(log_errors()){
                i2c_state = I2C_FINISH;
                break;
            }

            if (1 == i2c_msg->num_rx_bytes - i2c_msg->bytes_rxed) {

                i2c_msg->rx_data[i2c_msg->num_rx_bytes - ++i2c_msg->bytes_rxed ] = I2CMasterDataGet(I2C1_BASE);
                i2c_state = I2C_FINISH;

            } else if (2 == i2c_msg->num_rx_bytes - i2c_msg->bytes_rxed) {

                i2c_msg->rx_data[i2c_msg->num_rx_bytes - ++i2c_msg->bytes_rxed] = I2CMasterDataGet(I2C1_BASE);
                I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
                i2c_state = I2C_RECEIVE;

            } else if (i2c_msg->num_rx_bytes - i2c_msg->bytes_rxed > 2 ) {

                i2c_msg->rx_data[i2c_msg->num_rx_bytes - ++i2c_msg->bytes_rxed] = I2CMasterDataGet(I2C1_BASE);
                I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
                i2c_state = I2C_RECEIVE;

            }

            break;

        case I2C_FINISH :

            i2c_state = I2C_IDLE;
            i2c_msg->state = i2c_ready;
            i2c_msg->bytes_rxed = 0;
            i2c_msg->bytes_txed = 0;

            break;

        default :

            assert(0);
            break;
        }

        vTaskDelay(0);

    }

} // End i2c_task

static bool log_errors(void) {

    uint32_t status = I2CMasterErr(I2C1_BASE);

    if(I2C_MASTER_ERR_ADDR_ACK == (status & I2C_MASTER_ERR_ADDR_ACK)) {
        set_error(addr_ack_err);
    }

    if( I2C_MASTER_ERR_DATA_ACK  == (status &  I2C_MASTER_ERR_DATA_ACK )) {
        set_error(data_ack_err);
    }

    if(I2C_MASTER_ERR_ARB_LOST == (status & I2C_MASTER_ERR_ARB_LOST)) {
        set_error(arb_lost_err);
    }

    if(I2C_MASTER_ERR_CLK_TOUT == (status & I2C_MASTER_ERR_CLK_TOUT)) {
        set_error(clk_tout_err);
    }

    if (status) {
        return true;
    } else {
        return false;
    }

} // End log_errors
