/*
 * SPITransfer.c
 *
 *  Created on: Oct. 13, 2021
 *      Author: bruno
 */

#include <common/stm32/spi/SPITransfer.h>

void spi_transfer_init(SPITransfer* transfer) {
    // TODO
}

void spi_transfer_add(SPITransfer* transfer, uint8_t byte) {
    // TODO
    // Adds `byte` to the TX buffer
}

void spi_transfer_add_repeated(SPITransfer* transfer, uint8_t byte, uint32_t count) {
    // TODO
    // Adds `byte` to the TX buffer repeatedly, `count` number of times
}
