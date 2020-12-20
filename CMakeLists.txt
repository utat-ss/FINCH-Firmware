# This file is adapted from https://dev.to/younup/cmake-on-stm32-the-beginning-3766

# Define the minimum version of CMake required to compile
# (preferably the latest version so we are all consistent)
cmake_minimum_required(VERSION 3.19.2)
# Define the entire project name
project(Firmware
        DESCRIPTION "FINCH Firmware"
)

# Enable using C and Assembly source files
enable_language(C ASM)
# Use the C11 standard to match the CubeIDE project
set(CMAKE_C_STANDARD 11)
# Enforce that we must use the C11 standard
set(CMAKE_C_STANDARD_REQUIRED ON)
# Don't request compiler-specific extensions
set(CMAKE_C_EXTENSIONS OFF)

set(EXECUTABLE STM32_gpio.out)
add_executable(${EXECUTABLE})

# Specify all source (.c and .s) files
# It seems that it is a bad idea to use globbing to find all .c (or other)
# files, so we will explicitly list all files
# Source: https://stackoverflow.com/questions/2110795/how-to-use-all-c-files-in-a-directory-with-the-cmake-build-system
target_sources(${EXECUTABLE} PRIVATE
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth_ex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd_ex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
        Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usb.c

        Manual_Tests/STM32_gpio/gpio_test.c

        Src/drivers/STM32H7_clock/clock.c
        Src/drivers/STM32_gpio/gpio.c

        Startup/STM32H7/startup_stm32h743zitx.s
        Startup/STM32H7/stm32h7_startup.c
        Startup/STM32H7/stm32h7xx_it.c
        Startup/STM32H7/system_stm32h7xx.c
)

# Specify all header (.h) file directories
# Similar to source files, explicitly list all files rather than globbing 
target_include_directories(${EXECUTABLE} PRIVATE
        Drivers/CMSIS/Device/ST/STM32H7xx/Include
        Drivers/CMSIS/Include
        Drivers/STM32H7xx_HAL_Driver/Inc
        Src
        Startup/STM32H7
)

# Specify compiler definitions (macros)
target_compile_definitions(${EXECUTABLE} PRIVATE
        -DSTM32H743xx
        -DUSE_HAL_DRIVER
)

# Add compiler options
target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard

        -fdata-sections
        -ffunction-sections

        -Wall

        $<$<CONFIG:Debug>:-Og>
)

# Add linker script and linker options
target_link_options(${EXECUTABLE} PRIVATE
        -T${CMAKE_SOURCE_DIR}/Projects/STM32H743ZITX/STM32H743ZITX_FLASH.ld
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -specs=nano.specs
        -lc
        -lm
        -lnosys
        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections
)

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-size ${EXECUTABLE}
)

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
)
