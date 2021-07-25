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

typedef struct {
	// TODO
} MCU;

void mcu_init(MCU *mcu);

#endif /* COMMON_STM32_MCU_MCU_H_ */
