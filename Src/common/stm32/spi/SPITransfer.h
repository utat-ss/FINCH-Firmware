/*
 * SPITransfer.h
 *
 *  Created on: Oct. 13, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_SPI_SPITRANSFER_H_
#define COMMON_STM32_SPI_SPITRANSFER_H_

#include <stdint.h>

#define SPI_TRANSFER_BUF_SIZE 50

typedef struct {
    // If using C++, should make the buffer size a template parameter
    uint8_t tx_buf[SPI_TRANSFER_BUF_SIZE];
    uint8_t rx_buf[SPI_TRANSFER_BUF_SIZE];

    // Number of bytes to transfer
    uint32_t count;
} SPITransfer;

#endif /* COMMON_STM32_SPI_SPITRANSFER_H_ */
