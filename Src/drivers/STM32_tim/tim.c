/*
 * tim.c
 *
 *  Created on: Jul 22, 2020
 *      Author: Cameron Rodriguez
 *
 * Overview: Wrapper code for the HAL code for timer functions on the STM32G4 MCU
 *
 * When certain inputs are permitted for a parameter, the potential inputs are shown as follows: (USE: NAME_OF_INPUT[_SWAP/_THESE])
 * --> This can receive either NAME_OF_INPUT_SWAP or NAME_OF_INPUT_THESE
 */

#include "tim.h"

/*Initializes a timer handle, counting up by default.
 * @param TIM_TypeDef *instance - the timer to use
 * @param uint32_t prescaler - the prescaler value to load
 * @param uint32_t period - the timer period
 * @return Tim_HandleTypeDef - the timer handle
 */
TIM_HandleTypeDef tim_init_handle(TIM_TypeDef *instance, uint32_t prescaler, uint32_t period){
	TIM_HandleTypeDef handle = {.Instance = instance};
	handle.Init.Prescaler = prescaler;
	handle.Init.Period = period;
	handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	handle.Init.RepetitionCounter = 0;
	return handle;
}

/* Configure a basic struct for a timer with Base feature.
 * @param TIM_TypeDef *instance - the register where the specific timer is location (ex. TIM2)
 * @param TimMode mode - the mode the timer runs in (normal, interrupt, or DMA) and associated values
 * @param uint32_t prescaler - used in the prescaler calculation to divide the clock
 * @param uint32_t period - value to reset to or at; part of the prescaler calculation
 * @return TimFunc - the TimFunc timer struct
 */
TimFunc tim_config_base(TIM_TypeDef *instance, TimMode mode, uint32_t prescaler, uint32_t period){
	TimFunc base_timer;
	TIM_HandleTypeDef handle = tim_init_handle(instance, prescaler, period);

	base_timer.function = TIM_Base;
	base_timer.mode = mode;
	base_timer.handle = handle;

	return base_timer;
}

/* Configure a basic struct for a timer with the Input Capture feature.
 * @param TIM_TypeDef *instance - the register where the specific timer is location (ex. TIM2)
 * @param TimMode mode - the mode the timer runs in (normal, interrupt, or DMA) and associated values
 * @param uint32_t prescaler - used in the prescaler calculation to divide the clock
 * @param uint32_t period - value to reset to or at; part of the prescaler calculation
 * @param uint32_t ic_polarity - specify the input's polarity (USE: TIM_INPUTCHANNELPOLARITY[_RISING/_FALLING/_BOTHEDGE])
 * @param uint32_t ic_selection - specify how IC registers are connected to timer input channels (USE: TIM_IC_SELECTION[_DIRECTTI/_INDIRECTTI/_TRC)
 * --> Direct (the default) uses TIM Input 1 (TI1) to IC1, TI2->IC2, TI3->IC3, TI4->IC4; Indirect uses TI2->IC1, TI1->IC2, TI4->IC3, TI3->IC4; TRC connects to timer interconnect
 * @param uint32_t ic_prescaler - capture every X events (USE: TIM_ICPSC[_DIV1/_DIV2/_DIV4/_DIV8])
 * @param uint32_t ic_filter - specify the IC filter, between 0x0 and 0xF
 * @return TimFunc - the TimFunc timer struct
 */
TimFunc tim_config_ic(TIM_TypeDef *instance, TimMode mode, uint32_t prescaler, uint32_t period, uint32_t ic_polarity,
		uint32_t ic_selection, uint32_t ic_prescaler, uint32_t ic_filter){
	TimFunc ic_timer;
	TIM_HandleTypeDef handle = tim_init_handle(instance, prescaler, period);
	TIM_IC_InitTypeDef ic_config = {.ICPolarity = ic_polarity, .ICSelection = ic_selection, .ICPrescaler = ic_prescaler, .ICFilter = ic_filter};


	ic_timer.function = TIM_IC;
	ic_timer.mode = mode;
	ic_timer.handle = handle;
	ic_timer.ic_cfg = ic_config;

	return ic_timer;
}

/* Configure a basic struct for a timer with the Output Compare or PWM feature.
 * @param uint8_t function - whether to use a OC or PWM timer
 * @param TIM_TypeDef *instance - the register where the specific timer is location (ex. TIM2)
 * @param TimMode mode - the mode the timer runs in (normal, interrupt, or DMA) and associated values
 * @param uint32_t prescaler - used in the prescaler calculation to divide the clock
 * @param uint32_t period - value to reset to or at; part of the prescaler calculation
 * @param uint32_t mode - specify the timer mode, some options shown (USE: TIM_OCMODE[_TIMING/_ACTIVE/_INACTIVE/_TOGGLE/_FORCED_ACTIVE/_FORCED_INACTIVE] for OC,
 * 							 TIM_OCMODE[_PWM1/_PWM2/_ASSYMETRIC_PWM1/_ASSYMETRIC_PWM1/_COMBINED_PWM1/_COMBINED_PWM2] for PWM)
 * --> Timing - generates a timing base, no external effect ; Active/Inactive - set channel on match; Toggle - toggles on match, Forced Active/Inactive - remain active/inactive
 * --> PWM1 is active before hitting CCR value, PWM2 is active after hitting CCR value; Assymetric allows for a phase shift; Combined lets PWMs combine delays and phase shifts@param uint32_t oc_pulse - value loaded into the Capture Compare Register using the OC Pulse equation (max. 0xFFFF, 0xFFEF with dithering)
 * @param uint32_t pulse - value loaded into the Capture Compare Register using the OC Pulse equation (max. 0xFFFF, 0xFFEF with dithering)
 * @param uint32_t polarity - specify the output's polarity (USE: TIM_OCPOLARITY[_HIGH/_LOW])
 * @return TimFunc - the TimFunc timer struct
 */
TimFunc tim_config_oc_pwm(uint8_t function, TIM_TypeDef *instance, TimMode tim_mode, uint32_t prescaler, uint32_t period, uint32_t mode, uint32_t pulse,
		uint32_t polarity){
	TimFunc timer;
	TIM_HandleTypeDef handle = tim_init_handle(instance, prescaler, period);
	TIM_OC_InitTypeDef config = {.OCMode = mode, .Pulse = pulse, .OCPolarity = polarity};

	timer.function = function;
	timer.mode = tim_mode;
	timer.handle = handle;
	timer.oc_pwm_cfg = config;

	return timer;
}

/* Configure a basic struct for a timer with the OnePulse feature.
 * @param TIM_TypeDef *instance - the register where the specific timer is location (ex. TIM2)
 * @param TimMode mode - the mode the timer runs in (normal, interrupt, or DMA) and associated values
 * @param uint32_t prescaler - used in the prescaler calculation to divide the clock
 * @param uint32_t period - value to reset to or at; part of the prescaler calculation
 * @param uint32_t oc_mode - specify the timer mode, some options shown (USE: TIM_OCMODE[_TIMING/_ACTIVE/_INACTIVE/_TOGGLE/_FORCED_ACTIVE/_FORCED_INACTIVE])
 * --> Timing - generates a timing base, no external effect ; Active/Inactive - set channel on match; Toggle - toggles on match, Forced Active/Inactive - remain active/inactive
 * @param uint32_t oc_pulse - value loaded into the Capture Compare Register using the OC Pulse equation (max. 0xFFFF, 0xFFEF with dithering)
 * @param uint32_t oc_polarity - specify the output's polarity (USE: TIM_OCPOLARITY[_HIGH/_LOW])
 * @param uint32_t ic_polarity - specify the input's polarity (USE: TIM_INPUTCHANNELPOLARITY[_RISING/_FALLING/_BOTHEDGE])
 * @param uint32_t ic_selection - specify how IC registers are connected to timer input channels (USE: TIM_IC_SELECTION[_DIRECTTI/_INDIRECTTI/_TRC)
 * --> Direct (the default) uses TIM Input 1 (TI1) to IC1, TI2->IC2, TI3->IC3, TI4->IC4; Indirect uses TI2->IC1, TI1->IC2, TI4->IC3, TI3->IC4; TRC connects to timer interconnect
 * @param uint32_t ic_filter - specify the IC filter, between 0x0 and 0xF
 * @return TimFunc - the TimFunc timer struct
 */
TimFunc tim_config_onepulse(TIM_TypeDef *instance, TimMode mode, uint32_t prescaler, uint32_t period, uint32_t oc_mode, uint32_t oc_pulse,
		uint32_t oc_polarity, uint32_t ic_polarity, uint32_t ic_selection, uint32_t ic_filter){
	TimFunc op_timer;
	TIM_HandleTypeDef handle = tim_init_handle(instance, prescaler, period);
	TIM_OnePulse_InitTypeDef op_config = {.OCMode = oc_mode, .Pulse = oc_pulse, .OCPolarity = oc_polarity, .ICPolarity = ic_polarity,
		.ICSelection = ic_selection, .ICFilter = ic_filter};

	op_timer.function = TIM_OnePulse;
	op_timer.mode = mode;
	op_timer.handle = handle;
	op_timer.onepulse_cfg = op_config;

	return op_timer;
}

/* Configure the timer to detect update events
 *
 * NOTE: If this option is selected, the timer MUST be run in interrupt mode
 * @param TimFunc *timer - the timer struct
 * @param uint16_t period_repetitions - number of periods to count before an update event is triggered (uses flag TIM_FLAG_UPDATE)
 * @return uint8_t - whether the repetitions were successfully implemented
 */
void tim_config_repetition(TimFunc *timer, uint16_t period_repetitions){
	// Configure struct
	timer->mode.mode = TIM_Mode_IT;
	timer->handle.Init.RepetitionCounter = period_repetitions - 1;
	timer->handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

	// Enable flag
	tim_enable_IT_flag(&(timer->handle), TIM_FLAG_UPDATE);
}

// Timer operation functions

/* Start the timer with the given mode and options
 *
 * @param TimFunc *timer - the timer struct
 * @return HAL_StatusTypeDef - confirmation of successful timer start (good start is HAL_OK or 0x00U)
 */
HAL_StatusTypeDef tim_start(TimFunc *timer){
	HAL_StatusTypeDef successful = HAL_OK, channel_successful = HAL_OK;

	// Split by function
	if(timer->function==TIM_Base) {
		HAL_TIM_Base_MspInit(&(timer->handle));
		tim_init_clock(timer);
		successful = HAL_TIM_Base_Init(&(timer->handle));

		if(successful != HAL_OK){ // Error check
			// Cancel start
		} else if(timer->mode.mode==TIM_Mode_None){
			successful = HAL_TIM_Base_Start(&(timer->handle));
		} else if (timer->mode.mode==TIM_Mode_IT){
			successful = HAL_TIM_Base_Start_IT(&(timer->handle));
		} else if (timer->mode.mode==TIM_Mode_DMA){
			successful = HAL_TIM_Base_Start_DMA(&(timer->handle), timer->mode.src_addr, timer->mode.length);
		}
	}else if(timer->function==TIM_IC) {
		HAL_TIM_IC_MspInit(&(timer->handle));
		tim_init_clock(timer);
		channel_successful = HAL_TIM_IC_ConfigChannel(&(timer->handle), &(timer->ic_cfg), timer->mode.channel);
		successful = HAL_TIM_IC_Init(&(timer->handle));

		if(successful != HAL_OK || channel_successful != HAL_OK){ // Error checks
			successful = channel_successful; // Cancel start
		} else if(timer->mode.mode==TIM_Mode_None){
			successful = HAL_TIM_IC_Start(&(timer->handle), timer->mode.channel);
		} else if (timer->mode.mode==TIM_Mode_IT){
			successful = HAL_TIM_IC_Start_IT(&(timer->handle), timer->mode.channel);
		} else if (timer->mode.mode==TIM_Mode_DMA){
			successful = HAL_TIM_IC_Start_DMA(&(timer->handle), timer->mode.channel, timer->mode.src_addr, timer->mode.length);
		}
	}else if(timer->function==TIM_OC) {
		HAL_TIM_OC_MspInit(&(timer->handle));
		tim_init_clock(timer);
		channel_successful = HAL_TIM_OC_ConfigChannel(&(timer->handle), &(timer->oc_pwm_cfg), timer->mode.channel);
		successful = HAL_TIM_OC_Init(&(timer->handle));

		if(successful != HAL_OK || channel_successful != HAL_OK){
			successful = channel_successful; // Cancel start
		} else if(timer->mode.mode==TIM_Mode_None){
			successful = HAL_TIM_OC_Start(&(timer->handle), timer->mode.channel);
		} else if (timer->mode.mode==TIM_Mode_IT){
			successful = HAL_TIM_OC_Start_IT(&(timer->handle), timer->mode.channel);
		} else if (timer->mode.mode==TIM_Mode_DMA){
			successful = HAL_TIM_OC_Start_DMA(&(timer->handle), timer->mode.channel, timer->mode.src_addr, timer->mode.length);
		}
	}else if(timer->function==TIM_PWM) {
		HAL_TIM_PWM_MspInit(&(timer->handle));
		tim_init_clock(timer);
		channel_successful = HAL_TIM_PWM_ConfigChannel(&(timer->handle), &(timer->oc_pwm_cfg), timer->mode.channel);
		successful = HAL_TIM_PWM_Init(&(timer->handle));

		if(successful != HAL_OK || channel_successful != HAL_OK){
			successful = channel_successful; // Cancel start
		} else if(timer->mode.mode==TIM_Mode_None){
			successful = HAL_TIM_PWM_Start(&(timer->handle), timer->mode.channel);
		} else if (timer->mode.mode==TIM_Mode_IT){
			successful = HAL_TIM_PWM_Start_IT(&(timer->handle), timer->mode.channel);
		} else if (timer->mode.mode==TIM_Mode_DMA){
			successful = HAL_TIM_PWM_Start_DMA(&(timer->handle), timer->mode.channel, timer->mode.src_addr, timer->mode.length);
		}
	}else if(timer->function==TIM_OnePulse) {
		HAL_TIM_OnePulse_MspInit(&(timer->handle));
		tim_init_clock(timer);
		channel_successful = HAL_TIM_OnePulse_ConfigChannel(&(timer->handle), &(timer->onepulse_cfg), timer->mode.output_channel, timer->mode.input_channel);
		successful = HAL_TIM_OnePulse_Init(&(timer->handle), timer->mode.onepulse_mode);

		if(successful != HAL_OK || channel_successful != HAL_OK){
			successful = channel_successful; // Cancel start
		} else if(timer->mode.mode==TIM_Mode_None){
			successful = HAL_TIM_OnePulse_Start(&(timer->handle), timer->mode.channel);
		} else if (timer->mode.mode==TIM_Mode_IT){
			successful = HAL_TIM_OnePulse_Start_IT(&(timer->handle), timer->mode.channel);
		}
	}else if(timer->function==TIM_Encoder) {
		HAL_TIM_Encoder_MspInit(&(timer->handle));
		tim_init_clock(timer);
		successful = HAL_TIM_Encoder_Init(&(timer->handle), &(timer->encoder_cfg));

		if(successful != HAL_OK){
			// Cancel start
		} else if(timer->mode.mode==TIM_Mode_None){
			HAL_TIM_Encoder_Start(&(timer->handle), timer->mode.channel);
		} else if (timer->mode.mode==TIM_Mode_IT){
			HAL_TIM_Encoder_Start_IT(&(timer->handle), timer->mode.channel);
		} else if (timer->mode.mode==TIM_Mode_DMA){
			HAL_TIM_Encoder_Start_DMA(&(timer->handle), timer->mode.channel, timer->mode.src_addr, timer->mode.src_addr2, timer->mode.length);
		}
	}else{
		// Error handler
	}

	return successful;
}

/* Starts the appropriate timer clock and interrupt handler, and runs clock config if present.
 *
 * @param TimFunc *timer - the timer struct
 */
void tim_init_clock(TimFunc *timer){
	if(timer->handle.Instance==TIM2){
		__HAL_RCC_TIM2_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}else if(timer->handle.Instance==TIM3){
		__HAL_RCC_TIM3_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM3_IRQn);
	}else if(timer->handle.Instance==TIM4){
		__HAL_RCC_TIM4_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM4_IRQn);
	}else if(timer->handle.Instance==TIM5){
		__HAL_RCC_TIM5_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM5_IRQn);
	}else if(timer->handle.Instance==TIM6){
		__HAL_RCC_TIM6_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
	}

	// MCU-specific timers
#ifdef STM32G474xx
	if (timer->handle.Instance==TIM1){
		__HAL_RCC_TIM1_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
	}else if(timer->handle.Instance==TIM7){
		__HAL_RCC_TIM7_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM7_DAC_IRQn);
	}else if(timer->handle.Instance==TIM15){
		__HAL_RCC_TIM15_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
	}else if(timer->handle.Instance==TIM16){
		__HAL_RCC_TIM16_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
	}else if(timer->handle.Instance==TIM17){
		__HAL_RCC_TIM17_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM17_IRQn);
	}else if(timer->handle.Instance==TIM20){
		__HAL_RCC_TIM20_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM20_UP_IRQn);
	}
#elif defined(STM32H743xx)
	if(timer->handle.Instance==TIM1){
		__HAL_RCC_TIM1_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
	}else if(timer->handle.Instance==TIM7){
		__HAL_RCC_TIM7_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM7_IRQn);
	}else if(timer->handle.Instance==TIM12){
		__HAL_RCC_TIM12_CLK_ENABLE();
	}else if(timer->handle.Instance==TIM13){
		__HAL_RCC_TIM13_CLK_ENABLE();
	}else if(timer->handle.Instance==TIM14){
		__HAL_RCC_TIM14_CLK_ENABLE();
	}else if(timer->handle.Instance==TIM15){
		__HAL_RCC_TIM15_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM15_IRQn);
	}else if(timer->handle.Instance==TIM16){
		__HAL_RCC_TIM16_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM16_IRQn);
	}else if(timer->handle.Instance==TIM17){
		__HAL_RCC_TIM17_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(TIM17_IRQn);
	}
#endif

	if(&(timer->clk_cfg) != NULL){
		tim_config_clock(&(timer->handle), &(timer->clk_cfg));
	}
}

/* Stops a given timer
 *
 * @param TimFunc *timer - the timer struct
 * @return HAL_StatusTypeDef - if the operation inputs were valid (good stop is HAL_OK or 0x00U)
 */
HAL_StatusTypeDef tim_stop(TimFunc *timer){
	HAL_StatusTypeDef successful = HAL_ERROR; // Default to error state

	if(timer->function==TIM_Base){
		if(timer->mode.mode==TIM_Mode_None){
			successful = HAL_TIM_Base_Stop(&(timer->handle));
		}else if(timer->mode.mode==TIM_Mode_IT){
			successful = HAL_TIM_Base_Stop_IT(&(timer->handle));
		}else if(timer->mode.mode==TIM_Mode_DMA){
			successful = HAL_TIM_Base_Stop_DMA(&(timer->handle));
		}else{
			successful = HAL_ERROR;
		}
	}else if(timer->function==TIM_IC){
		if(timer->mode.mode==TIM_Mode_None){
			successful = HAL_TIM_IC_Stop(&(timer->handle), timer->mode.channel);
		}else if(timer->mode.mode==TIM_Mode_IT){
			successful = HAL_TIM_IC_Stop_IT(&(timer->handle), timer->mode.channel);
		}else if(timer->mode.mode==TIM_Mode_DMA){
			successful = HAL_TIM_IC_Stop_DMA(&(timer->handle), timer->mode.channel);
		}else{
			successful = HAL_ERROR;
		}
	}else if(timer->function==TIM_OC){
		if(timer->mode.mode==TIM_Mode_None){
			successful = HAL_TIM_OC_Stop(&(timer->handle), timer->mode.channel);
		}else if(timer->mode.mode==TIM_Mode_IT){
			successful = HAL_TIM_OC_Stop_IT(&(timer->handle), timer->mode.channel);
		}else if(timer->mode.mode==TIM_Mode_DMA){
			successful = HAL_TIM_OC_Stop_DMA(&(timer->handle), timer->mode.channel);
		}
	}else if(timer->function==TIM_PWM){
		if(timer->mode.mode==TIM_Mode_None){
			successful = HAL_TIM_PWM_Stop(&(timer->handle), timer->mode.channel);
		}else if(timer->mode.mode==TIM_Mode_IT){
			successful = HAL_TIM_PWM_Stop_IT(&(timer->handle), timer->mode.channel);
		}else if(timer->mode.mode==TIM_Mode_DMA){
			successful = HAL_TIM_PWM_Stop_DMA(&(timer->handle), timer->mode.channel);
		}
	}else if(timer->function==TIM_OnePulse){
		if(timer->mode.mode==TIM_Mode_None){
			successful = HAL_TIM_OnePulse_Stop(&(timer->handle), timer->mode.channel);
		}else if(timer->mode.mode==TIM_Mode_IT){
			successful = HAL_TIM_OnePulse_Stop_IT(&(timer->handle), timer->mode.channel);
		}else{
			successful = HAL_ERROR;
		}
	}else if(timer->function==TIM_Encoder){
		if(timer->mode.mode==TIM_Mode_None){
			successful = HAL_TIM_Encoder_Stop(&(timer->handle), timer->mode.channel);
		}else if(timer->mode.mode==TIM_Mode_IT){
			successful = HAL_TIM_Encoder_Stop_IT(&(timer->handle), timer->mode.channel);
		}else if(timer->mode.mode==TIM_Mode_DMA){
			successful = HAL_TIM_Encoder_Stop_DMA(&(timer->handle), timer->mode.channel);
		}
	}else{
		// Error handler
	}

	return successful;
}

/* Deinitializes a given timer
 *
 * @param TimFunc *timer - the timer struct
 * @return HAL_StatusTypeDef - if the operation inputs were valid (good deinit is HAL_OK or 0x00U)
 */
HAL_StatusTypeDef tim_deinit(TimFunc *timer){
	HAL_StatusTypeDef status = HAL_OK;
	if(timer->function==TIM_Base){
		status = HAL_TIM_Base_DeInit(&(timer->handle));
	}else if(timer->function==TIM_IC){
		status = HAL_TIM_IC_DeInit(&(timer->handle));
	}else if(timer->function==TIM_OC){
		status = HAL_TIM_OC_DeInit(&(timer->handle));
	}else if(timer->function==TIM_PWM){
		status = HAL_TIM_PWM_DeInit(&(timer->handle));
	}else if(timer->function==TIM_OnePulse){
		status = HAL_TIM_OnePulse_DeInit(&(timer->handle));
	}else if(timer->function==TIM_Encoder){
		status = HAL_TIM_Encoder_DeInit(&(timer->handle));
	}else{
		status = HAL_ERROR;
	}

	return status;
}

/* Checks if a flag has been raised, clears the flag, and returns a result
 *
 * @param TimFunc* timer - the timer struct
 * @param uint16_t flag - the flag to check
 * @return uint8_t - 1 if the flag was raised, 0 otherwise
 */
uint8_t tim_check_flag(TimFunc* timer, uint8_t flag){
	if(tim_get_IT_flag(&(timer->handle), flag)){
		tim_clear_IT_flag(&(timer->handle), flag);
		return 1;
	}else{
		return 0;
	}
	return 0;
}

// Quickstart timer

TimFunc qs_timer;
void (*qs_func)() = NULL;

/* Function for TIM4 interrupt handler *
void TIM4_IRQHandler(){
	if(tim_get_IT_flag(&(qs_timer.handle), TIM_IT_UPDATE) != RESET){
		tim_clear_IT_flag(&(qs_timer.handle), TIM_IT_UPDATE);
		qs_func();
	}
} */

/* Creates a Base timer on TIM4 that executes a void function at every update event.
 *
 * @param uint32_t clk_frequency - frequency at which the clock is operating
 * @param uint32_t target_frequency - the frequency at which the function should execute (min. 1/65536 of clk_frequency)
 * @param void (*f)() - the void function to execute
 * @return TimFunc - the loaded timer struct
 */
TimFunc tim_quickstart_void_function(uint32_t clk_frequency, uint32_t target_frequency, void (*function)()){
	TimMode mode = {.mode = TIM_Mode_IT};
	uint32_t prescaler = 0, period = 0;
	qs_func = function;

	// Set timer frequency
	if (clk_frequency / target_frequency > 65536){
		qs_timer.handle.Instance = NULL;
		return qs_timer;
	} else if (clk_frequency / target_frequency > 256) {
		prescaler = 255;
		period = (clk_frequency / target_frequency / (prescaler+1)) - 1;
		qs_timer = tim_config_base(TIM4, mode, prescaler, period);
	} else {
		prescaler = (clk_frequency / target_frequency) - 1;
		period = (clk_frequency / target_frequency / (prescaler+1)) - 1;
		qs_timer = tim_config_base(TIM4, mode, prescaler, period);
	}

	tim_start(&qs_timer);

	return qs_timer;
}
