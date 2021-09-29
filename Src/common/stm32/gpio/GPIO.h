/*
 * GPIO.h
 *
 *  Created on: Jul. 8, 2020
 *      Author: brytni
 *
 *  This file provides a single header file to include in application code to
 *  include all of the GPIO header files
 *
 *  Overview:
 *  Wrapper code for the GPIO pins on the STM32 MCU
 *
 *  By default, the output pins speed is set to GPIO_SPEED_FREQ_LOW (reduces
 *  noise and power consumption); The max frequency is 8MHz
 *
 *  Standard GPIO outputs are always set to GPIO_SPEED_FREQ_LOW because they are
 *  not toggled at high frequencies
 *  GPIO speed is not applicable for inputs
 *  Alternate function GPIOs must have their speed specified by the application
 *  code because they may be toggled at higher frequencies than what
 *  GPIO_SPEED_FREQ_LOW supports
 *
 *  For example, can see for the H743 MCU in its datasheet (DS12110 Rev 8),
 *  Section 7.3.15 I/O port characteristics, Table 160 Output timing
 *  characteristics (HSLV OFF), the maximum frequency for each GPIO speed under
 *  various conditions
 *  Assuming VDD=3.3V for the H743 MCU, maximum frequencies are (worst case):
 *  - low speed (0b00) - 12MHz
 *  - medium speed (0b01) - 60MHz
 *  - high speed (0b10) - 85MHz
 *  - very high speed (0b11) - 100MHz
 *
 *  Alternate function values can be seen in the following datasheets for the
 *  following MCUs:
 *  STM32H7 - Table 10 pg 89: https://www.mouser.ca/datasheet/2/389/dm00387108-1799185.pdf
 *  STM32G4 - Table 13 pg 74: https://www.st.com/resource/en/datasheet/stm32g474ve.pdf
 *
 *  Don't need to save the GPIO_InitTypeDef struct in our GPIO structs because
 *  it is only used for HAL_GPIO_Init()
 *  All other HAL GPIO functions only require the GPIO_TypeDef and the pin
 */

#ifndef COMMON_STM32_GPIO_GPIO_H_
#define COMMON_STM32_GPIO_GPIO_H_

#include <common/stm32/gpio/GPIOAltFunc.h>
#include <common/stm32/gpio/GPIOInput.h>
#include <common/stm32/gpio/GPIOITInput.h>
#include <common/stm32/gpio/GPIOOutput.h>

#endif /* COMMON_STM32_GPIO_GPIO_H_ */
