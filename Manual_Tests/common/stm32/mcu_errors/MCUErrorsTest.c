/*
 * MCUErrorsTest.c
 *
 *  Created on: Sept. 28, 2021
 *      Author: bruno
 *
 * Produces various errors in the HAL and in our code for testing.
 */

#include <common/stm32/mcu/errors.h>
#include <common/stm32/uart/log.h>
#include <stdlib.h>

int main() {
    // Try to automatically detect board based on MCU UID
    MCUBoard board = mcu_get_board();
    // If the UID is not matched, try to guess the board based on the device ID
    if (board == MCU_BOARD_NONE) {
        MCUDevID dev_id = mcu_get_dev_id();
        if (dev_id == MCU_DEV_ID_STM32G471_473_474_483_484) {
            board = MCU_BOARD_NUCLEO_G474RE;
        }
        else if (dev_id == MCU_DEV_ID_STM32H742_743_753_750) {
            board = MCU_BOARD_NUCLEO_H743ZI2;
        }
    }

    MCU mcu;
    mcu_init(&mcu, board);
    UART uart;
    uart_init_for_board(&uart, &mcu);
    Log log;
    log_init(&log, &uart);

    info(&log, "Starting MCU errors test");

    Error_Handler();
    assert_param(1);
    assert_param(0);
    // Must cast __FILE__ or else it produces the warning "pointer targets in
    // passing argument 1 of 'assert_failed' differ in signedness
    // [-Wpointer-sign]"
    assert_failed((uint8_t*) __FILE__, __LINE__);

    // Intentionally call a HAL function with invalid input so it fails
    // assert_param(), which calls assert_failed()
    UART_HandleTypeDef uart_handle;
    uart_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uart_handle.Instance = NULL;	// Not a valid UART/USART or LPUART
    HAL_UART_Init(&uart_handle);

    // Make sure it logs an error if you try to use malloc()
    info(&log, "Attempting malloc()");
    uint8_t* dyn_array = malloc(100);
    info(&log, "Dynamic array addr: 0x%lx", dyn_array);

    info(&log, "Almost done MCU errors test");
    info(&log, "Will try to force a hard fault, "
            "which infinite loops in the fault handler");
    // Force a hard fault by reading from an invalid memory address
    uint32_t* ptr = (uint32_t*) 0x20000;
    uint32_t value = *ptr;	// This line should fail
    info(&log, "0x%lx", value);	// Should never reach this line

    while (1) {}

    return 0;
}
