/*
 * I2CDevice.c
 *
 *  Created on: Oct. 6, 2021
 *      Author: bruno
 */

#include <common/stm32/i2c/I2CDevice.h>
#include <common/stm32/i2c/I2CRead.h>
#include <common/stm32/i2c/I2CWrite.h>

// addr_mode - must be I2C_ADDRESSINGMODE_7BIT or I2C_ADDRESSINGMODE_10BIT
// TODO - should addr_mode be a property of the bus instead of the device?
// addr - must be the 7-BIT ADDRESS (NOT 8-bit address)
void i2c_device_init(I2CDevice* device, I2CBus* bus,
        uint32_t addr_mode, uint16_t addr, I2CFreq freq) {
    // TODO
    // Make sure to initialize a new Log struct for this struct
}

// Return true for success, false for failure
// Could save the specific error code in the I2CDevice struct for retrieval, or
// pass a pointer into the function to set a variable to the error code, or just
// return the error code as the return value of the function
bool i2c_device_read(I2CDevice* device, I2CRead* read, uint32_t count) {
    // TODO

    // Need to set the addressing mode (maybe) and frequency (definitely) before
    // every transfer, because the previous I2C device on the same bus may have
    // used a different mode/frequency
    // This may require calling HAL_I2C_Init() here

    // Do this before the read transfer
    read->count = count;

    // Do this after the read transfer
    debug_bytes(&device->log, read->buf, read->count,
            "I2C read (addr = 0x%.2X)", device->addr);
}

// Return true for success, false for failure (similar to above)
bool i2c_device_write(I2CDevice* device, I2CWrite* write) {
    // TODO

    // Need to set the addressing mode (maybe) and frequency (definitely) before
    // every transfer (same as above)

    // Do this before the write transfer
    debug_bytes(&device->log, write->buf, write->count,
            "I2C write (addr = 0x%.2X)", device->addr);
}
