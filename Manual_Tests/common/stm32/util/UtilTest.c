/*
 * UtilTest.c
 *
 *  Created on: Sept. 28, 2021
 *      Author: bruno
 *  Last modified: May 29, 2022
 *
 * Tests miscellaneous utility functions (e.g. bit/byte manipulation).
 */

#include <common/stm32/uart/Log.h>
#include <common/stm32/util/Util.h>

int main() {
    // Try to automatically detect board based on MCU UID
    MCUBoard board = mcu_get_board();
    // If the UID is not matched, try to guess the board based on the device ID
    if (board == MCU_BOARD_NONE) {
        MCUDevID dev_id = mcu_get_dev_id();
        if (dev_id == MCU_DEV_ID_STM32G431_441) {
            board = MCU_BOARD_NUCLEO_G431RB;
        } else if (dev_id == MCU_DEV_ID_STM32G471_473_474_483_484) {
            board = MCU_BOARD_NUCLEO_G474RE;
        } else if (dev_id == MCU_DEV_ID_STM32H742_743_753_750) {
            board = MCU_BOARD_NUCLEO_H743ZI2;
        }
    }

    MCU mcu;
    mcu_init(&mcu, board);
    UART uart;
    uart_init_for_board(&uart, &mcu);
    Log log;
    log_init(&log, &uart);

    info(&log, "Starting utilities test");

    info(&log, "bit(0) = 0x%lX", bit(0));	// 0x1
    info(&log, "bit(1) = 0x%lX", bit(1));	// 0x2
    info(&log, "bit(11) = 0x%lX", bit(11));	// 0x800

    info(&log, "lsb_bits(0) = 0x%lX", lsb_bits(0));	// 0x0
    info(&log, "lsb_bits(1) = 0x%lX", lsb_bits(1));	// 0x1
    info(&log, "lsb_bits(5) = 0x%lX", lsb_bits(5));	// 0x1F

    info(&log, "bits(12, 12) = 0x%lX", bits(12, 12));	// 0x1000
    info(&log, "bits(15, 12) = 0x%lX", bits(15, 12));	// 0xF000
    info(&log, "bits(11, 1) = 0x%lX", bits(11, 1));		// 0xFFE

    info(&log, "get_bit(0xA3, 7) = 0x%lX", get_bit(0xA3, 7));	// 0x1
    info(&log, "get_bit(0xA3, 6) = 0x%lX", get_bit(0xA3, 6));	// 0x0
    info(&log, "get_bit(0xA3, 5) = 0x%lX", get_bit(0xA3, 5));	// 0x1

    info(&log, "get_bits(0xA3, 7, 5) = 0x%lX", get_bits(0xA3, 7, 5));	// 0x5
    info(&log, "get_bits(0xA3, 7, 4) = 0x%lX", get_bits(0xA3, 7, 4));	// 0xA

    uint8_t bytes1[3];
    serialize_be_bytes(0xFA329B, bytes1, 3);
    info_bytes(&log, bytes1, 3,
            "serialize_be_bytes(0xFA329B, ..., 3)");	// 0xFA:32:9B

    uint8_t bytes2[3] = {0xAF, 0x23, 0xB9};
    info(&log, "deserialize_be_bytes({0xAF, 0x23, 0xB9}, 3) = 0x%lX",
            deserialize_be_bytes(bytes2, 3));	// 0xAF23B9

    uint8_t bytes3[3];
    serialize_le_bytes(0xFA329B, bytes3, 3);
    info_bytes(&log, bytes3, 3,
            "serialize_le_bytes(0xFA329B, ..., 3)");	// 0x9B:32:FA

    uint8_t bytes4[3] = {0xAF, 0x23, 0xB9};
    info(&log, "deserialize_le_bytes({0xAF, 0x23, 0xB9}, 3) = 0x%lX",
            deserialize_le_bytes(bytes4, 3));	// 0xB923AF

    info(&log, "Done utilities test");
    while (1) {}

    return 0;
}
