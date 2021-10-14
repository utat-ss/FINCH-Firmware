/*
 * SPIBus.c
 *
 *  Created on: Oct. 13, 2021
 *      Author: bruno
 */

#include <common/stm32/spi/SPIBus.h>

// alternate - e.g. GPIO_AF5_SPI1
void spi_bus_init(SPIBus* bus, UART* uart,
        SPI_TypeDef* instance, uint8_t alternate,
        GPIO_TypeDef* sck_port, uint16_t sck_pin,
        GPIO_TypeDef* mosi_port, uint16_t mosi_pin,
        GPIO_TypeDef* miso_port, uint16_t miso_pin) {

    // TODO

    // Make sure to initialize the SCK, MOSI, and MISO pins in push-pull mode!
    // Also make sure the GPIO speed is high enough to hit the max SPI frequency
    // that could be used

    // Make sure to initialize a new Log struct for this struct
}
