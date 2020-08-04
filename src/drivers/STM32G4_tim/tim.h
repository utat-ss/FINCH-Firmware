/*
 * tim.h
 *
 *  Created on: Jul 22, 2020
 *      Author: camro
 */

#ifndef DRIVERS_STM32G4_TIM_TIM_H_
#define DRIVERS_STM32G4_TIM_TIM_H_

#include "../../Core/Inc/main.h"

TIM_HandleTypeDef* tim_init(TIM_HandleTypeDef* handle, TIM_TypeDef* instance, uint32_t prescaler, uint32_t time_counter_mode, uint32_t period,
		uint32_t clock_division, uint32_t repitition_counter, uint32_t autoreload_preload);

#endif /* DRIVERS_STM32G4_TIM_TIM_H_ */
