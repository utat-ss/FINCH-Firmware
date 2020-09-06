/*
 * clock.h
 *
 *  Created on: Aug. 9, 2020
 *      Author: brytni
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#ifdef STM32G474xx
#include <stm32g4xx_hal.h>
#endif

#ifdef STM32H743xx
#include <stm32h7xx_hal.h>
#endif

void clock_init(void);
void SystemClock_Config(void);
void GPIOClock_Config(void);

#endif /* SYS_CLK_H_ */
