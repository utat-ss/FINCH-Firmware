/*
 * nvic.c
 *
 *  Created on: Nov 23, 2020
 *      Author: Cameron Rodriguez
 */

#include <drivers/STM32_nvic/nvic.h>

// NVIC reset & priority functions

/* Performs a software reset ("reboot"). */
void nvic_system_reset(){
	HAL_NVIC_SystemReset();
}

/* Returns the number of bits dedicated to priority preemption grouping for interrupts.
 * For more information, see comments for nvic_set_priority_grouping.
 *
 * @return uint32_t - the number of bits dedicated to preemption grouping
 */
uint32_t nvic_get_priority_grouping(){
	return HAL_NVIC_GetPriorityGrouping();
}

/* This function sets the priority preemption grouping for interrupts, and should be setup before any interrupts.
 *
 * There are 4 bits (16 levels) of subpriority available, to determine the order to process interrupts in. Lower numbers have higher priority.
 * Up to 4 bits can be dedicated to preemption grouping, at the cost of less subpriority levels.
 *
 * Interrupts with lower preemption group numbers can interrupt those with higher numbers, even if they're currently being handled.
 * Members of the same preemption group cannot interrupt each other, and one-by-one by timing, and priority.
 *
 * @param uint32_t grouping_bits - the number of bits to dedicate to preemption priority. (USE: NVIC_PRIORITYGROUP[_1/_2/_3/_4])
 */
void nvic_set_priority_grouping(uint32_t grouping_bits){
	if(grouping_bits <= 7 && grouping_bits >= 3){
		HAL_NVIC_SetPriorityGrouping(grouping_bits);
	}
}

/* Internal function to validate if group and subpriority inputs are valid based on current setup of priority groups, and available bits
 *
 * @param uint32_t group - the preemption group
 * @param uint32_t subpriority - the subpriority level
 * @return uint8_t - 1 if
 */
uint8_t nvic_validate_priority_inputs(uint32_t group, uint32_t subpriority){
	uint32_t total_groups = nvic_get_priority_grouping(); // 7 - total groups =  number of preemption bits

	if(group >> (7 - total_groups) != 0 || group < 0){ // Too high, low group for current setup
		return 0;
	}
	if(subpriority >> (total_groups - 3) != 0 || subpriority < 0){ // Too high, low subpriority
		return 0;
	}
	return 1;
}

/* Returns priority groups and subpriority levels of an interrupt.
 *
 * @param IRQn_Type interrupt - the interrupt channel to check (ex. TIM4_IRQn; these can be found in /Drivers/CMSIS/Device/ST/STM32xxxx/stm32xxxxxx.h)
 * @param uint32_t *group - pointer to hold the preemption group of the interrupt
 * @param uint32_t *subpriority - pointer to hold the subpriority of the interrupt
 */
void nvic_get_priority(IRQn_Type interrupt, uint32_t *group, uint32_t *subpriority){
	HAL_NVIC_GetPriority(interrupt, nvic_get_priority_grouping(), group, subpriority);
}

/* Sets the priority of an interrupt, if the inputs are valid
 *
 * @param IRQn_Type interrupt - the interrupt channel to set the priority for
 * @param uint32_t group - the preemption group to set the interrupt to
 * @param uint32_t subpriority - the subpriority level to set the interrupt to
 */
void nvic_set_priority(IRQn_Type interrupt, uint32_t group, uint32_t subpriority){
	if(nvic_validate_priority_inputs(group, subpriority)){ // Error checking
		HAL_NVIC_SetPriority(interrupt, group, subpriority);
	}
}

// Interrupt (IRQ) operation functions

/* Returns whether a given interrupt is active or not.
 *
 * @param IRQn_type interrupt -  the interrupt to check
 * @return uint32_t - the status of the interrupt (1 if active, 0 otherwise)
 */
uint32_t nvic_get_active_interrupt(IRQn_Type interrupt){
	return HAL_NVIC_GetActive(interrupt);
}

/* Enables the given interrupt channel.
 *
 * @param IRQn_Type interrupt - the interrupt channel to enable
 */
void nvic_enable_interrupt(IRQn_Type interrupt){
	HAL_NVIC_EnableIRQ(interrupt);
}

/* Disables the given interrupt channel.
 *
 * @param IRQn_Type interrupt - the interrupt channel to disable
 */
void nvic_disable_interrupt(IRQn_Type interrupt){
	HAL_NVIC_DisableIRQ(interrupt);
}

/* Check if an interrupt is pending on the channel
 *
 * @param IRQn_Type interrupt - the interrupt channel to check
 * @return uint32_t - if there is a pending interrupt (1 if pending, 0 otherwise)
 */
uint32_t nvic_get_pending_interrupt(IRQn_Type interrupt){
	return HAL_NVIC_GetPendingIRQ(interrupt);
}

/* Manually set an interrupt channel as pending.
 *
 * @param IRQn_Type interrupt - the interrupt to set as pending
 */
void nvic_set_pending_interrupt(IRQn_Type interrupt){
	HAL_NVIC_SetPendingIRQ(interrupt);
}

/* Clear a pending interrupt from a given channel.
 *
 * @param IQRn_Type interrupt - the channel to clear the interrupt from
 */
void nvic_clear_pending_interrupt(IRQn_Type interrupt){
	HAL_NVIC_SetPendingIRQ(interrupt);
}
