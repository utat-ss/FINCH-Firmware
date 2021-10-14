/*
 * SPIDevice.c
 *
 *  Created on: Oct. 13, 2021
 *      Author: bruno
 */

#include <common/stm32/spi/SPIDevice.h>
#include <common/stm32/spi/SPITransfer.h>

void spi_device_init(SPIDevice* device, SPIBus* bus,
        GPIO_TypeDef* cs_port, uint16_t cs_pin,
        SPIMode mode, double freq_hz) {

    // TODO

    // Make sure to initialize the CS pin in push-pull mode!

    // Need to decide whether to use the automatic hardware NSS (CS) management,
    // or just toggle NSS (CS) manually with a standard GPIO output

    // Make sure to initialize a new Log struct for this struct
}

// Return true for success, false for failure
// Could save the specific error code in the SPIDevice struct for retrieval, or
// pass a pointer into the function to set a variable to the error code, or just
// return the error code as the return value of the function
bool spi_device_transfer(SPIDevice* device, SPITransfer* transfer) {
    // TODO

    // Need to set the SPI mode and frequency before every transfer, because the
    // previous SPI device on the same bus may have used a different mode/frequency
    // This may require calling HAL_SPI_Init() here

    // Make sure to set CS low before the transfer and high after the transfer,
    // or use the automatic hardware NSS (CS) management

    // Do this before the transfer
    debug_bytes(&device->log, transfer->tx_buf, transfer->count, "SPI TX");

    // Do this after the transfer
    debug_bytes(&device->log, transfer->rx_buf, transfer->count, "SPI RX");
}
