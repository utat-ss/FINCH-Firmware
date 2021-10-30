/*
Timer.c

Date created: 2021-10-19
Last modified: 2021-10-19
Author: Cameron Rodriguez

Wrapper for the HAL timer (TIM) drivers. Timers to be implemeneted
  * Basic timer with interrupt
    * Autoreload toggle
    * Update toggle (raises flag after number of loops completed)
    * Countup toggle
  * Timer with callback function

Not currently supported
  * output compare (OC) - modify signal when counter == catch/capture register
  * pulse width modulation (PWM)
  * OnePulse mode (variant of Oc)
  * input capture (IC) - latch counter value after a signal is recieved
  * DMA burst mode

Still need to determine which timers will be enabled
 */

#include <common/stm32/timer/Timer.h>

// Initializes the timer struct, and optionally enables interrupts.
Timer timer_init(MCU* mcu, uint32_t clk_frequency, uint32_t prescaler,
    uint32_t period, uint8_t it_enabled) {
    Timer timer_struct;

    // Setup timer handle, default to TIM5
    TIM_Base_InitTypeDef base_timer = {
        .Prescaler = prescaler,
        .Period = period,
        .CounterMode = TIM_COUNTERMODE_DOWN,
        .ClockDivision = TIM_CLOCKDIVISION_DIV1,
        .RepetitionCounter = 0,
        .AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE
    };
    timer_struct.handle = {.Instance = TIM5, .Init = base_timer};
    timer_struct.interrupts_enabled = it_enabled;
    timer_struct.callback_enabled = 0;

	HAL_TIM_Base_MspInit(TIM5); // Default to TIM5
    
    return timer_struct;
}

/* Add additional customizations to the timer driver

TIM_TypeDef* timer_reg: specific timer register to use, use NULL to make no
                        change
uint8_t count_up: run timer in upcounter mode (default countdown)
uint8_t disable_autoreload: disable automatic reloading of timer once limit is 
                            reached
uint8_t repetitions: number of timer loops to execute before update interrupt is
                     raised (default 0)
*/
void timer_customize(Timer* timer_struct, TIM_TypeDef* timer_reg, uint8_t count_up, 
    uint8_t disable_autoreload, uint8_t repetitions) {
    
    if (timer_reg != NULL) {
        timer_struct->handle.Instance = timer_reg;
    }
    if (count_up) {
        timer_struct->Init.CounterMode = TIM_COUNTERMODE_UP;
    }
    if (disable_autoreload) {
        timer_struct->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    }
    if (repetitions != 0 {
        timer_struct->Init.RepetitionCounter = repetitions;
    }
}

uint8_t timer_setup_callback() {

}

// Initializes the timer, must be called before starting the timer
HAL_StatusTypeDef timer_init(Timer* timer_struct) {
    return HAL_TIM_Base_Init(&(timer_struct->handle));
}

// Deinitializes the timer after it has been stopped
HAL_StatusTypeDef timer_deinit(Timer* timer_struct) {
    return HAL_TIM_Base_DeInit(&(timer_struct->handle));
}

// Starts the timer (in interrupt mode if specified during timer setup)
HAL_StatusTypeDef timer_start(Timer* timer_struct) {    
    if(timer_struct->interrupts_enabled) {
        return HAL_TIM_Base_Start_IT(&(timer_struct->handle));
    } else {
        return HAL_TIM_Base_Start(&(timer_struct->handle));
    }
}

// Stops the timer (in interrupt mode if specified during timer setup)
HAL_StatusTypeDef timer_stop(Timer* timer_struct) {
    if(timer_struct->interrupts_enabled) {
        return HAL_TIM_Base_Stop_IT(&(timer_struct->handle));
    } else {
        return HAL_TIM_Base_Stop(&(timer_struct->handle));
    }
}

