/*
 * tim.h
 *
 *  Created on: Jul 22, 2020
 *      Author: camro
 */

#ifndef DRIVERS_STM32G4_TIM_TIM_H_
#define DRIVERS_STM32G4_TIM_TIM_H_

#include "../../Core/Inc/main.h"

// Constants for timer functions and modes in init/start/stop functions
enum tim_func_constants {TIM_Base, TIM_IC, TIM_OC, TIM_PWM, TIM_OnePulse, TIM_Encoder};

TIM_HandleTypeDef* tim_populate_handle(TIM_HandleTypeDef* handle, TIM_TypeDef* instance, uint32_t prescaler, uint32_t time_counter_mode, uint32_t period,
		uint32_t clock_division, uint32_t repitition_counter, uint32_t autoreload_preload);
uint8_t tim_init(TIM_HandleTypeDef* handle, uint16_t function, uint32_t onepulse_mode, TIM_Encoder_InitTypeDef* sConfig);
uint8_t tim_deinit(TIM_HandleTypeDef* handle, uint16_t function);
uint8_t tim_start(TIM_HandleTypeDef* handle, uint16_t function, uint32_t channel);
uint8_t tim_stop(TIM_HandleTypeDef* handle, uint16_t function, uint32_t channel);

#endif /* DRIVERS_STM32G4_TIM_TIM_H_ */
