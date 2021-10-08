/*
 * I2CWrite.c
 *
 *  Created on: Oct. 6, 2021
 *      Author: bruno
 */

#include <common/stm32/i2c/I2CWrite.h>

void i2c_write_init(I2CWrite* write) {
    // TODO
}

void i2c_write_add(I2CWrite* write, uint8_t byte) {
    // TODO
    // Adds `byte` to the buffer
}

void i2c_write_add_repeated(I2CWrite* write, uint8_t byte, uint32_t count) {
    // TODO
    // Adds `byte` to the buffer repeatedly, `count` number of times
}
