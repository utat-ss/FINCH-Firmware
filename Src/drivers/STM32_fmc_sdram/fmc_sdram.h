/*
 * fmc_sdram.h
 *
 *  Created on: Jan. 4, 2021
 *      Author: Tobias
 */

#ifndef DRIVERS_STM32_FMC_SDRAM_FMC_SDRAM_H_
#define DRIVERS_STM32_FMC_SDRAM_FMC_SDRAM_H_

#ifdef STM32H743xx
#include <stm32h7xx_hal.h>
#include <stm32h7_startup.h>
#include <drivers/STM32H7_clock/clock.h>
#endif

#ifdef STM32G474xx
#include <stm32g4xx_hal.h>
#include <stm32g4_startup.h>
#include <drivers/STM32G4_clock/clock.h>
#endif

typedef struct {
  SDRAM_HandleTypeDef hsdram;
  uint32_t bank_num;
  uint32_t mem_data_width;
  uint32_t num_row_bits;
  uint32_t num_col_bits;
  uint32_t base_address;

}FMC_SDRAM;

void fmc_sdram_init(FMC_SDRAM* sdmram_handler, uint32_t bank_num, uint32_t bank_num, uint32_t mem_data_width);


#endif /* DRIVERS_STM32_FMC_SDRAM_FMC_SDRAM_H_ */
