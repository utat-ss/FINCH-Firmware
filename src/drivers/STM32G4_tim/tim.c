/*
 * tim.c
 *
 *  Created on: Jul 22, 2020
 *      Author: Cameron Rodriguez
 *
 * Overview: Wrapper code for the HAL code for timer functions on the STM32G4 MCU
 */

#include "tim.h"

/* Populate the handle for a timer
 * @param TIM_HandleTypeDef handle - the handle to be populated
 * @param TIM_TypeDef* instance - the register for the timer
 * @param uint32_t prescaler - prescaler value
 * @param uint32_t time_counter_mode - counting mode (TIM_COUNTERMODE[_UP/_DOWN/_CENTERALIGNED1/_CENTERALIGNED2/_CENTERALIGNED3])
 * @param uint32_t period - value to reset to (counting down) or to reset at (counting up)
 * @param uint32_t clock_divison - value to divide clock by (TIM_CLOCKDIVISION[_DIV1/_DIV2/_DIV4])
 * @param uint32_t repitition_counter - value for repetition counter (default 0)
 * @param uint32_t autoreload_preload - disable/enable auto-reload preload (TIM_AUTORELOAD_PRELOAD[_DISABLE/_ENABLE])
 * @return TIM_HandleTypeDef* - the populated handle
 */
TIM_HandleTypeDef* tim_populate_handle(TIM_HandleTypeDef* handle, TIM_TypeDef* instance, uint32_t prescaler, uint32_t time_counter_mode, uint32_t period,
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

/* Initialize a specific timer function
 * @param TIM_HandleTypeDef* handle - the handle for the timer
 * @param uint16_t function - the timer function to be enabled (see tim.h for potential values)
 * @param uint32_t onepulse_mode - select OnePulse mode (TIM_OPMODE[_SINGLE/_REPETITIVE])\
 * @param TIM_Encoder_InitTypeDef* sconfig - Encoder interface config handle
 * @return uint8_t - successful/failed initialization
 */
uint8_t tim_init(TIM_HandleTypeDef* handle, uint16_t function, uint32_t onepulse_mode, TIM_Encoder_InitTypeDef* sConfig){
	if(function==TIM_Base){
		HAL_TIM_Base_Init(handle);
	}else if(function==TIM_IC){
		HAL_TIM_IC_Init(handle);
	}else if(function==TIM_OC){
		HAL_TIM_OC_Init(handle);
	}else if(function==TIM_PWM){
		HAL_TIM_PWM_Init(handle);
	}else if(function==TIM_OnePulse){
		HAL_TIM_OnePulse_Init(handle, onepulse_mode);
	}else if(function==TIM_Encoder){
		HAL_TIM_Encoder_Init(handle, sConfig);
	}else{
		return 0;
	}

	return 1;
}

/* DEinitialize a specific timer function
 * @param TIM_HandleTypeDef* handle - the handle for the timer
 * @param uint16_t function - the timer function to be disabled (see tim.h for potential values)
 * @return uint8_t - successful/failed deinitialization
 */
uint8_t tim_deinit(TIM_HandleTypeDef* handle, uint16_t function){
	if(function==TIM_Base){
			HAL_TIM_Base_DeInit(handle);
		}else if(function==TIM_IC){
			HAL_TIM_IC_DeInit(handle);
		}else if(function==TIM_OC){
			HAL_TIM_OC_DeInit(handle);
		}else if(function==TIM_PWM){
			HAL_TIM_PWM_DeInit(handle);
		}else if(function==TIM_OnePulse){
			HAL_TIM_OnePulse_DeInit(handle);
		}else if(function==TIM_Encoder){
			HAL_TIM_Encoder_DeInit(handle);
		}else{
			return 0;
		}

		return 1;
}

/* Starts a timer peripheral, without DMA or interrupts
 * @param TIM_HandleTypeDef* handle - the handle for the timer
 * @param uint16_t function - the function of the timer
 * @param channel - the timer channel to use
 * @return uint8_t - successful/failed start
 */

uint8_t tim_start(TIM_HandleTypeDef* handle, uint16_t function, uint32_t channel){
	if(function==TIM_Base){
		HAL_TIM_Base_Start(handle);
	}else if(function==TIM_IC){
		HAL_TIM_IC_Start(handle, channel);
	}else if(function==TIM_OC){
		HAL_TIM_OC_Start(handle, channel);
	}else if(function==TIM_PWM){
		HAL_TIM_PWM_Start(handle, channel);
	}else if(function==TIM_OnePulse){
		HAL_TIM_OnePulse_Start(handle, channel);
	}else if(function==TIM_Encoder){
		HAL_TIM_Encoder_Start(handle, channel);
	}else{
		return 0;
	}

	return 1;
}

/* Stops a timer peripheral, without DMA or interrupts
 * @param TIM_HandleTypeDef* handle - the handle for the timer
 * @param uint16_t function - the function of the timer
 * @param channel - the timer channel to use
 * @return uint8_t - successful/failed stop
 */

uint8_t tim_stop(TIM_HandleTypeDef* handle, uint16_t function, uint32_t channel){
	if(function==TIM_Base){
		HAL_TIM_Base_Stop(handle);
	}else if(function==TIM_IC){
		HAL_TIM_IC_Stop(handle, channel);
	}else if(function==TIM_OC){
		HAL_TIM_OC_Stop(handle, channel);
	}else if(function==TIM_PWM){
		HAL_TIM_PWM_Stop(handle, channel);
	}else if(function==TIM_OnePulse){
		HAL_TIM_OnePulse_Stop(handle, channel);
	}else if(function==TIM_Encoder){
		HAL_TIM_Encoder_Stop(handle, channel);
	}else{
		return 0;
	}

	return 1;
}
