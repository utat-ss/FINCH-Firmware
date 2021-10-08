/*
 * I2CBus.c
 *
 *  Created on: Oct. 6, 2021
 *      Author: bruno
 */

#include <common/stm32/i2c/I2CBus.h>

// alternate - e.g. GPIO_AF3_I2C3
void i2c_bus_init(I2CBus* bus, UART* uart,
        I2C_TypeDef* instance, uint8_t alternate,
        GPIO_TypeDef* scl_port, uint16_t scl_pin,
        GPIO_TypeDef* sda_port, uint16_t sda_pin) {

    // TODO

    // Make sure to initialize the SCL and SDA pins in open-drain mode!
    // Also make sure the GPIO speed is high enough to hit the max I2C frequency
    // that could be used

    // Make sure to initialize a new Log struct for this struct
}
