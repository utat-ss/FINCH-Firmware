/*
 * mcu.h
 *
 *  Created on: Jul. 24, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_MCU_MCU_H_
#define COMMON_STM32_MCU_MCU_H_

#include <common/stm32/mcu/hal.h>
#include <common/stm32/mcu/init.h>

// DEV_ID values
typedef enum {
	// Category 2 - STM32G431, STM32G441 (AES) - RM0440 Rev 3 p.2087, p.73
	MCU_DEV_ID_STM32G431_441 = 0x468,
	// Category 3 - STM32G471, STM32G473, STM32G474, STM32G483, STM32G484 - RM0440 Rev 3 p.2087, p.73
	MCU_DEV_ID_STM32G471_473_474_483_484 = 0x469,
	// Same DEV_ID for STM32H742, STM32H743/753, and STM32H750 - RM0433 Rev 7 p.3189
	MCU_DEV_ID_STM32H742_743_753_750 = 0x450,
} MCUDevID;

// Can't define an enum for REV_ID because the numbering is different and
// overlapping between different MCU models

// The specific model of MCU this is
typedef enum {
	// Unknown or unspecified
	MCU_MODEL_NONE,
	MCU_MODEL_STM32G474,
	MCU_MODEL_STM32H743,
} MCUModel;

// The board/PCB this MCU is on
typedef enum {
	// Unknown or unspecified
	MCU_BOARD_NONE,
	// G474 dev kit
	MCU_BOARD_NUCLEO_G474RE,
	// H743 dev kit
	MCU_BOARD_NUCLEO_H743ZI2,
} MCUBoard;

// Unique device identifier for MCU
// It is 96 bits long, so need three 32-bit words to store it
typedef struct {
	// First word
	uint32_t w0;
	// Second word
	uint32_t w1;
	// Third word
	uint32_t w2;
} MCUUID;

typedef struct {
	// Board and model are useful pieces of information to be used in code
	MCUBoard board;
	MCUModel model;
} MCU;

void mcu_init(MCU *mcu, MCUBoard board);
MCUDevID mcu_get_dev_id();
uint32_t mcu_get_rev_id();
MCUUID mcu_get_uid();
MCUBoard mcu_get_board();
MCUModel mcu_get_model_for_board(MCUBoard board);
MCUModel mcu_get_model();

#endif /* COMMON_STM32_MCU_MCU_H_ */
