/*
 * mcu.c
 *
 *  Created on: Jul. 24, 2021
 *      Author: bruno
 */

#include <common/stm32/mcu/mcu.h>

void mcu_init(MCU *mcu) {
	HAL_Init();
	SystemClock_Config();
	GPIOClock_Config();
}
