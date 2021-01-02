# This file is adapted from https://dev.to/younup/cmake-on-stm32-the-beginning-3766

# Define the minimum version of CMake required
# As of 2021-01-02 the latest version is 3.19.2, but use 3.16.3 as the minimum
# because that is the version used for Ubuntu 20.04 LTS
cmake_minimum_required(VERSION 3.16.3)
# Define the entire project name
project(Firmware
        DESCRIPTION "FINCH Firmware"
)

# Make sure MCU series parameter is valid
if ((${MCU_SERIES} MATCHES "G4") OR (${MCU_SERIES} MATCHES "H7"))
else()
        message(FATAL_ERROR "Invalid MCU series: must be G4 or H7")
endif()

# Enable using C and Assembly source files
enable_language(C ASM)
# Use the C11 standard to match the CubeIDE project
set(CMAKE_C_STANDARD 11)
# Enforce that we must use the C11 standard
set(CMAKE_C_STANDARD_REQUIRED ON)
# Don't request compiler-specific extensions
set(CMAKE_C_EXTENSIONS OFF)

# Create a common library (object library) that compiles all common source files
# only once rather than repeating compilation for each test target
# https://stackoverflow.com/questions/29634101/cmake-how-to-define-target-without-linking-compilation-only
# https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#object-libraries
# https://cmake.org/cmake/help/latest/command/add_library.html
set(COMMON_TARGET common)
add_library(${COMMON_TARGET} OBJECT)

# Specify all common source (.c and .s) files
# It seems that it is a bad idea to use globbing to find all .c (or other)
# files, so we will explicitly list all files
# Source: https://stackoverflow.com/questions/2110795/how-to-use-all-c-files-in-a-directory-with-the-cmake-build-system
target_sources(${COMMON_TARGET} PRIVATE
        Src/drivers/STM32_gpio/gpio.c
        Src/drivers/STM32G4_clock/clock.c
        Src/drivers/STM32H7_clock/clock.c
)

# Add common source files for the particular MCU series in use
if (${MCU_SERIES} MATCHES "G4")
        target_sources(${COMMON_TARGET} PRIVATE
                Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.c
                Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.c
                Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.c
                Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.c
                Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.c
                Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.c
                Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.c
                Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.c
                Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.c
                Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.c
                Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.c
                Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.c
                Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.c
                Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.c
                Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.c
                Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.c
                Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.c

                Startup/STM32G4/startup_stm32g474retx.s
                Startup/STM32G4/stm32g4_startup.c
                Startup/STM32G4/stm32g4xx_it.c
                Startup/STM32G4/system_stm32g4xx.c
        )
elseif (${MCU_SERIES} MATCHES "H7")
        target_sources(${COMMON_TARGET} PRIVATE
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

                Startup/STM32H7/startup_stm32h743zitx.s
                Startup/STM32H7/stm32h7_startup.c
                Startup/STM32H7/stm32h7xx_it.c
                Startup/STM32H7/system_stm32h7xx.c
        )
endif()

# Recursively find all test source (.c) files under the Manual_Tests directory
# https://stackoverflow.com/questions/2110795/how-to-use-all-c-files-in-a-directory-with-the-cmake-build-system/2110796
file(GLOB_RECURSE TEST_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "Manual_Tests/*.c")

# Loop through all test source files
foreach(TEST_SOURCE IN LISTS TEST_SOURCES)
        # Get the source file's parent directory path
        get_filename_component(TEST_DIR ${TEST_SOURCE} DIRECTORY)
        # Append the parent directory path to the list of test directories
        list(APPEND TEST_DIRS ${TEST_DIR})
endforeach()
# In case there are multiple .c files in any test directory, remove duplicated
# test directories
list(REMOVE_DUPLICATES TEST_DIRS)

# Loop through all test directories
foreach(TEST_DIR IN LISTS TEST_DIRS)
        # A CMake target can't have slashes in its name, so replace all forward
        # slashes in the test directory path with dashes to create the target
        # name
        # Don't just use the last directory in the path because that could lead
        # to naming conflicts
        string(REPLACE "/" "-" TEST_TARGET ${TEST_DIR})
        # Append the target name to the list of test target names
        list(APPEND TEST_TARGETS ${TEST_TARGET})

        # Recursively find all test source (.c) files under the test directory
        file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${TEST_DIR}/*.c")

        # Add a new executable target for this test
        # Combine object files compiled from the common target with the test
        # sources specifically for this test
        add_executable(${TEST_TARGET} $<TARGET_OBJECTS:${COMMON_TARGET}> ${SOURCES})
endforeach()

# Save common options between compiler and linker so we don't have to repeat them
set(OPTIONS
        -mfloat-abi=hard
        -mthumb
        -specs=nano.specs
)
set(G4_OPTIONS
        -mcpu=cortex-m4
        -mfpu=fpv4-sp-d16
)
set(H7_OPTIONS
        -mcpu=cortex-m7
        -mfpu=fpv5-d16
)

# Loop through all targets (both common and tests since these settings apply to
# both)
foreach(TARGET IN LISTS COMMON_TARGET TEST_TARGETS)
        # Specify all header (.h) file directories
        # Similar to source files, explicitly list all files rather than globbing 
        target_include_directories(${TARGET} PRIVATE
                Drivers/CMSIS/Include
                Src
        )

        # Specify compiler definitions (macros)
        target_compile_definitions(${TARGET} PRIVATE
                -DDEBUG
                -DUSE_HAL_DRIVER
        )

        # Add compiler options
        target_compile_options(${TARGET} PRIVATE
                ${OPTIONS}

                -fdata-sections
                -ffunction-sections
                -fstack-usage

                # Maximum debugging information
                # https://stackoverflow.com/questions/10475040/gcc-g-vs-g3-gdb-flag-what-is-the-difference
                -g3

                # Optimization level
                $<$<CONFIG:Debug>:-Og>

                -Wall
        )

        # Add MCU series-specific settings
        if (${MCU_SERIES} MATCHES "G4")
                target_include_directories(${TARGET} PRIVATE
                        Drivers/CMSIS/Device/ST/STM32G4xx/Include
                        Drivers/STM32G4xx_HAL_Driver/Inc
                        Startup/STM32G4
                )
                target_compile_definitions(${TARGET} PRIVATE
                        -DSTM32G4
                        -DSTM32G474xx
                )
                target_compile_options(${TARGET} PRIVATE
                        ${G4_OPTIONS}
                )
        elseif (${MCU_SERIES} MATCHES "H7")
                target_include_directories(${TARGET} PRIVATE
                        Drivers/CMSIS/Device/ST/STM32H7xx/Include
                        Drivers/STM32H7xx_HAL_Driver/Inc
                        Startup/STM32H7
                )
                target_compile_definitions(${TARGET} PRIVATE
                        -DSTM32H7
                        -DSTM32H743xx
                )
                target_compile_options(${TARGET} PRIVATE
                        ${H7_OPTIONS}
                )
        endif()
endforeach()

# Loop through test targets
# These settings only apply to test targets (because they are linked and produce
# a final binary)
foreach(TARGET IN LISTS TEST_TARGETS)
        # Add linker script and linker options
        target_link_options(${TARGET} PRIVATE
                ${OPTIONS}
                
                -lc
                -lm
                -lnosys

                -static

                # Map file provides linker info, symbols
                -Wl,-Map=${TARGET}.map,--cref
                -Wl,--gc-sections
                # end-group must come after start-group or else we get the
                # linker error: "group ended before it began"
                -Wl,--start-group
                -Wl,--end-group
        )

        # Add MCU series-specific settings
        if (${MCU_SERIES} MATCHES "G4")
                target_link_options(${TARGET} PRIVATE
                        ${G4_OPTIONS}
                        -T${CMAKE_SOURCE_DIR}/Projects/STM32G474RETX/STM32G474RETX_FLASH.ld
                )
        elseif (${MCU_SERIES} MATCHES "H7")
                target_link_options(${TARGET} PRIVATE
                        ${H7_OPTIONS}
                        -T${CMAKE_SOURCE_DIR}/Projects/STM32H743ZITX/STM32H743ZITX_FLASH.ld
                )
        endif()

        # Print executable size
        add_custom_command(TARGET ${TARGET}
                POST_BUILD
                COMMAND arm-none-eabi-size ${TARGET}
        )

        # Create hex file
        add_custom_command(TARGET ${TARGET}
                POST_BUILD
                COMMAND arm-none-eabi-objcopy -O ihex ${TARGET} ${TARGET}.hex
                COMMAND arm-none-eabi-objcopy -O binary ${TARGET} ${TARGET}.bin
        )
endforeach()
