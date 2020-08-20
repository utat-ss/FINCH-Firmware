/*
 * tim.h
 *
 *  Created on: Jul 22, 2020
 *      Author: Cameron Rodriguez
 */

#ifndef DRIVERS_STM32_TIM_TIM_H_
#define DRIVERS_STM32_TIM_TIM_H_

#include "../../Core/Inc/main.h"

// Constants for timer functions
enum tim_func_constants {TIM_Base, TIM_IC, TIM_OC, TIM_PWM, TIM_OnePulse, TIM_Encoder};

// Helper calculator functions
uint16_t tim_calc_prescaler(uint32_t tim_clk, uint16_t count_clk);
uint16_t tim_calc_period(uint32_t tim_clk, uint16_t prescaler, uint16_t frequency);

// User-friendly functions for macros, including interrupt flags
uint32_t tim_get_counter(TIM_HandleTypeDef *handle);
void tim_enable_IT_flag(TIM_HandleTypeDef *handle, uint32_t flag);
void tim_disable_IT_flag(TIM_HandleTypeDef *handle, uint32_t flag);
uint8_t tim_get_IT_flag(TIM_HandleTypeDef *handle, uint32_t flag);
uint8_t tim_clear_IT_flag(TIM_HandleTypeDef *handle, uint32_t flag);

// Timer init/start/stop functions
TIM_HandleTypeDef *tim_populate_handle(TIM_HandleTypeDef *handle, TIM_TypeDef *instance, uint32_t prescaler, uint32_t time_counter_mode, uint32_t period,
		uint32_t clock_division, uint32_t repitition_counter, uint32_t autoreload_preload);
uint8_t tim_init(TIM_HandleTypeDef *handle, uint16_t function, uint32_t onepulse_mode, TIM_Encoder_InitTypeDef* sConfig);
uint8_t tim_deinit(TIM_HandleTypeDef *handle, uint16_t function);
uint8_t tim_start(TIM_HandleTypeDef *handle, uint16_t function, uint32_t channel);
uint8_t tim_start_IT(TIM_HandleTypeDef *handle, uint16_t function, uint32_t channel);
uint8_t tim_stop(TIM_HandleTypeDef *handle, uint16_t function, uint32_t channel);
uint8_t tim_stop_IT(TIM_HandleTypeDef *handle, uint16_t function, uint32_t channel);

#endif /* DRIVERS_STM32_TIM_TIM_H_ */
