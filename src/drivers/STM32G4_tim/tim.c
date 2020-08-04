/*
 * tim.c
 *
 *  Created on: Jul 22, 2020
 *      Author: Cameron Rodriguez
 *
 * Overview: Wrapper code for the HAL code for timer functions on the STM32G4 MCU
 */

#include "tim.h"

/* Placeholder initialization function - populate the handle for a timer
 * @param TIM_HandleTypeDef handle - the handle to be populated
 * @param TIM_TypeDef* instance - the register for the timer
 * @param uint32_t prescaler - prescaler value to divide clock by
 * @param uint32_t time_counter_mode - counting mode (TIM_COUNTERMODE[_UP/_DOWN/_CENTERALIGNED1/_CENTERALIGNED2/_CENTERALIGNED3])
 * @param uint32_t period - value to reset to (counting down) or to reset at (counting up)
 * @param uint32_t clock_divison - value to divide clock by (TIM_CLOCKDIVISION[_DIV1/_DIV2/_DIV4])
 * @param uint32_t repitition_counter - value for repetition counter (default 0)
 * @param uint32_t autoreload_preload - disable/enable auto-reload preload (TIM_AUTORELOAD_PRELOAD[_DISABLE/_ENABLE])
 * @return the populated handle
 */
TIM_HandleTypeDef* tim_init(TIM_HandleTypeDef* handle, TIM_TypeDef* instance, uint32_t prescaler, uint32_t time_counter_mode, uint32_t period,
		uint32_t clock_division, uint32_t repitition_counter, uint32_t autoreload_preload){
	handle->Instance = instance;
	handle->Init.Prescaler = prescaler;
	handle->Init.CounterMode = time_counter_mode;
	handle->Init.Period = period;
	handle->Init.ClockDivision = clock_division;
	handle->Init.RepetitionCounter = repitition_counter;
	handle->Init.AutoReloadPreload = autoreload_preload;

	return handle;
}
