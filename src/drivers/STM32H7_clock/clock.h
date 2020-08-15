/*
 * clock.h
 *
 *  Created on: Aug. 9, 2020
 *      Author: brytni
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "../../../Startup/STM32H7/stm32h7_startup.h"

void clock_init(void);
void SystemClock_Config(void);
void GPIOClock_Config(void);

#endif /* SYS_CLK_H_ */
