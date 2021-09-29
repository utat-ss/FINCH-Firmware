/*
 * MCU.c
 *
 *  Created on: Jul. 24, 2021
 *      Author: bruno
 * 
 * General-purpose MCU initialization wrapper. Also provides functionality for
 * programmatically determining properties of the MCU (e.g. device, revision,
 * unique ID, board).
 */

#include <common/stm32/mcu/Init.h>
#include <common/stm32/mcu/MCU.h>

// Pointer to "default" MCU struct
// Try not to use this, but it can be used in contexts where you do not have a
// pointer to the MCU struct
// (should only have one MCU struct initialized throughout the program lifetime)
MCU* g_mcu_def = NULL;

/*
 * @param board - can be specified manually, as MCU_BOARD_NONE, or automatically
 *                with the result from mcu_get_board()
 */
void mcu_init(MCU* mcu, MCUBoard board) {
    MCUModel model = mcu_get_model_for_board(board);

    // In the future, could customize this initialization per board/model
    HAL_Init();
    SystemClock_Config();
    GPIOClock_Config();

    mcu->board = board;
    mcu->model = model;

    // Save global default MCU pointer
    if (g_mcu_def == NULL) {
        g_mcu_def = mcu;
    }
}

MCUDevID mcu_get_dev_id() {
    return HAL_GetDEVID();
}

uint32_t mcu_get_rev_id() {
    return HAL_GetREVID();
}

MCUUID mcu_get_uid() {
    MCUUID uid;

    // The G4 HAL doesn't have the three functions to get UID
    // It defines UID_BASE though, so copy the implementation from the H7 HAL
#if defined(STM32G4)
    uid.w0 = READ_REG(*((uint32_t *)(UID_BASE + 0U)));
    uid.w1 = READ_REG(*((uint32_t *)(UID_BASE + 4U)));
    uid.w2 = READ_REG(*((uint32_t *)(UID_BASE + 8U)));
#elif defined(STM32H7)
    uid.w0 = HAL_GetUIDw0();
    uid.w1 = HAL_GetUIDw1();
    uid.w2 = HAL_GetUIDw2();
#endif

    return uid;
}

/*
 * Reads the UID (unique identifier) of your MCU and determines which board
 * (PCB) it is on.
 */
MCUBoard mcu_get_board() {
    MCUUID uid = mcu_get_uid();

    // This code will be frequently updated to add the UIDs of new MCUs

    // To find out the UID of your MCU, run this code and set a breakpoint here
    // (after mcu_get_uid() is called), then look at the value of `uid` (three
    // 32-bit words) in the debugger

    // To add the UID of your MCU for automatic board discovery, add a new
    // `else if` block with a UID check that returns the enum for your board
    // It is possible/common to have multiple UIDs that correspond to the same
    // board, e.g. for multiple copies of the same custom PCB or dev kit

    if (uid.w0 == 0x3f0040 && uid.w1 == 0x31395114 && uid.w2 == 0x35393336) {
        return MCU_BOARD_NUCLEO_H743ZI2;
    }

    return MCU_BOARD_NONE;
}

MCUModel mcu_get_model_for_board(MCUBoard board) {
    if (board == MCU_BOARD_NUCLEO_G474RE) {
        return MCU_MODEL_STM32G474;
    } else if (board == MCU_BOARD_NUCLEO_H743ZI2) {
        return MCU_MODEL_STM32H743;
    }

    return MCU_MODEL_NONE;
}
