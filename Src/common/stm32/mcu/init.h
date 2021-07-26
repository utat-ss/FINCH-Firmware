/*
 * init.h
 *
 *  Created on: Aug. 9, 2020
 *      Author: brytni
 */

#ifndef COMMON_STM32_MCU_INIT_H_
#define COMMON_STM32_MCU_INIT_H_

#include <common/stm32/mcu/errors.h>
#include <common/stm32/mcu/hal.h>

void SystemClock_Config(void);
void GPIOClock_Config(void);

#endif /* COMMON_STM32_MCU_INIT_H_ */
