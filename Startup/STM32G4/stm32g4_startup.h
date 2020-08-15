/*
 * stm32h7_startup.h
 *
 *  Header file that connects all of the STM32H7 startup files
 */

#ifndef STARTUP_H_
#define STARTUP_H_

// STM32H7 specific libraries
#include "stm32g4xx_hal.h"
#include "stm32g4xx.h"

// General Libraries
#include <stdint.h>

void Error_Handler(void);

#endif /* STARTUP_H_ */
