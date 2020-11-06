/* Interrupt handlers for all implemented timers.
 *
 * Created on: Oct 31, 2020
 *     Author: Cameron Rodriguez
 *
 * Overview: allows callbacks to be setup whenever an update interrupt occurs.
 */

#include "tim.h"
void tim_run_callback(TIM_TypeDef *timer, uint8_t func_location);

// Array of timers. Timers available: 1-7, 12-14 (H7 only), 15-17, 20 (G4 only)
volatile void (*tim_callback_array[14])() = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

/* Interrupt handlers
 *
 * Comment out these handlers if another version is needed. DO NOT MODIFY THEM.
 */

void TIM2_IRQHandler(){
	if((TIM2->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM2, 1);
	}
}

void TIM3_IRQHandler(){
	if((TIM3->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM3, 2);
	}
}

void TIM4_IRQHandler(){
	if((TIM4->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM4, 3);
	}
}

void TIM5_IRQHandler(){
	if((TIM5->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM5, 4);
	}
}

void TIM6_IRQHandler(){
	if((TIM6->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM6, 5);
	}
}

#ifdef STM32G474xx

void TIM1_UP_TIM16_IRQHandler(){
	if((TIM1->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM1, 0);
	}else if((TIM16->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM16, 11);
	}
}

void TIM7_DAC_IRQHandler(){
	if((TIM7->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM7, 6);
	}
}

void TIM1_BRK_TIM15_IRQHandler(){
	if((TIM15->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM15, 10);
	}
}

void TIM1_TRG_COM_TIM17_IRQHandler(){
	if((TIM17->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM17, 12);
	}
}

void TIM20_UP_IRQHandler(){
	if((TIM20->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM20, 13);
	}
}

#elif defined(STM32H743xx)

void TIM1_UP_IRQHandler(){
	if((TIM1->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM1, 0);
	}
}

void TIM7_IRQHandler(){
	if((TIM7->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM7, 6);
	}
}

void TIM8_BRK_TIM12_IRQHandler(){
	if((TIM12->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM12, 7);
	}
}

void TIM8_UP_TIM13_IRQHandler(){
	if((TIM13->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM13, 8);
	}
}

void TIM8_TRG_COM_TIM14_IRQHandler(){
	if((TIM14->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM14, 9);
	}
}

void TIM15_IRQHandler(){
	if((TIM15->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM15, 10);
	}
}

void TIM16_IRQHandler(){
	if((TIM16->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM16, 11);
	}
}

void TIM17_IRQHandler(){
	if((TIM17->SR &TIM_IT_UPDATE) == TIM_IT_UPDATE){
		tim_run_callback(TIM17, 12);
	}
}

#endif

/* This will load a provided callback function into the update interrupt handler of the given timer
 *
 * @param TimFunc timer - the provided timer
 * @param void (*function)() - the void callback function with no arguments
 * @return TimFunc - the original timer, or
 */
TimFunc tim_setup_callback(TimFunc timer, void (*function)()){
	void (*callback_func)() = function;
	// Array of functions, called by each handler
	if(timer.handle.Instance==TIM1){
		tim_callback_array[0] = callback_func;
	}else if(timer.handle.Instance==TIM2){
		tim_callback_array[1] = callback_func;
	}else if(timer.handle.Instance==TIM3){
		tim_callback_array[2] = callback_func;
	}else if(timer.handle.Instance==TIM4){
		tim_callback_array[3] = callback_func;
	}else if(timer.handle.Instance==TIM5){
		tim_callback_array[4] = callback_func;
	}else if(timer.handle.Instance==TIM6){
		tim_callback_array[5] = callback_func;
	}else if(timer.handle.Instance==TIM7){
		tim_callback_array[6] = callback_func;
	}else if(timer.handle.Instance==TIM15){
		tim_callback_array[10] = callback_func;
	}else if(timer.handle.Instance==TIM16){
		tim_callback_array[11] = callback_func;
	}else if(timer.handle.Instance==TIM17){
		tim_callback_array[12] = callback_func;
	}

#ifdef STM32G474xx
	if(timer.handle.Instance==TIM20){
		tim_callback_array[13] = callback_func;
	}
#elif defined(STM32H743xx)
	if(timer.handle.Instance==TIM12){
		tim_callback_array[7] = callback_func;
	}else if(timer.handle.Instance==TIM13){
		tim_callback_array[8] = callback_func;
	}else if(timer.handle.Instance==TIM14){
		tim_callback_array[9] = callback_func;
	}
#endif
	return timer;
}

/* This function runs the callback function of a given timer
 *
 * @param TIM_TypeDef *timer - the timer instance
 * @param uint8_t func_location - location of the timer's callback function
 */
void tim_run_callback(TIM_TypeDef *timer, uint8_t func_location){
	timer->SR = ~TIM_IT_UPDATE; // Clear flag
	if(func_location>=0 && func_location<14){
		(*tim_callback_array[func_location])();// Run the callback function
	}
}
