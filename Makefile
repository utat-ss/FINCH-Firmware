# It may seem strange to have a Makefile to call CMake, which in turn calls Make
# This Makefile is here to provide simpler commands to execute which are more
# intuitive to remember and use
# It also allows commands to be run from the root of the repository without
# having to cd into the build folder
# It also makes it easier to specify paths to manual tests without having to add
# .. to go up a directory
# It uses default CMake configuration parameters (such as the toolchain file and
# build type) which can be overriden if desired

# Note that support for the STM32H7 MCU series was only added in v1.7.0 (April
# 2021) of the STLink software package
# You can check the version of your installed STLink software with
# `st-info --version`
# https://github.com/stlink-org/stlink/pull/1033
# https://github.com/stlink-org/stlink/releases

# If you are using version v1.6.1 or earlier, the output of `st-info --probe`
# looks something like this:
#     Found 1 stlink programmers
#      serial:     303035373030333633343338353131303334333133393339
#      hla-serial: "\x30\x30\x35\x37\x30\x30\x33\x36\x33\x34\x33\x38\x35\x31\x31\x30\x33\x34\x33\x31\x33\x39\x33\x39"
#      flash:      0 (pagesize: 0)
#      sram:       0
#      chipid:     0x0000
#      descr:      unknown device

# Commands for st-flash and multiple connected programmers:
# https://github.com/stlink-org/stlink/blob/develop/doc/man/st-flash.md

# For some commands, must assign the MCU variable when calling Make to specify
# the MCU series (currently can be "G4" or "H7")
# e.g. make ... MCU=G4
# e.g. make ... MCU=H7

# Default build type
BUILD = Debug
# Base folder name for CMake configuration and builds
BUILD_DIR_BASE = Build
# Folder for CMake configuration and builds
BUILD_DIR = $(BUILD_DIR_BASE)-$(MCU)

# Strip trailing slash in the test path if there is one
# Use both forward slash (/) and backward slash (\) for Windows compatibility
# Need to use a simply expanded variable (:=) instead of a recursively expanded
# variable (=) or else TEST_TARGET would be recursively defined in terms of
# itself
TEST_TARGET := $(TEST)
TEST_TARGET := $(patsubst %/,%,$(TEST_TARGET))
TEST_TARGET := $(patsubst %\,%,$(TEST_TARGET))
# Replace every slash (/ or \) with a dash (-) - this is necessary to match our
# CMake configuration with how it names targets since a target name cannot
# contain a slash
# https://www.gnu.org/software/make/manual/html_node/Flavors.html
TEST_TARGET := $(subst /,-,$(TEST_TARGET))
TEST_TARGET := $(subst \,-,$(TEST_TARGET))

# Detect operating system
ifeq ($(OS),Windows_NT)
	WINDOWS = 1
endif

# Automatically detect the MCU model and set the MCU variable if there is only
# one MCU connected
G4_COUNT = $(shell st-info --probe | grep -c G4)
H7_COUNT = $(shell st-info --probe | grep -c H7)
MCU_COUNT = $(shell expr $(G4_COUNT) + $(H7_COUNT))
# This only overwrites the MCU variable if it was not set manually
ifeq ($(MCU_COUNT),1)
	ifeq ($(G4_COUNT),1)
		MCU = G4
	endif
	ifeq ($(H7_COUNT),1)
		MCU = H7
	endif
endif

# Serial argument (if specified)
# i.e. which MCU to upload to
ifneq ($(SERIAL),)
	SERIAL_ARG = --serial $(SERIAL)
endif




# -----------------------------------------------------------------------------

# Compile all test programs for all MCU models
# This command is useful to run before committing or merging code, to at least
# make sure all programs still compile correctly
.PHONY: all
all:
	make all_mcu MCU=G4
	make all_mcu MCU=H7

# Compile all test programs for one MCU model
.PHONY: all_mcu
all_mcu: $(BUILD_DIR)
	@cd $(BUILD_DIR) && \
	cmake --build . -- -j 1 && \
	cd ..

# Compile one test program for one MCU model
.PHONY: compile
compile: $(BUILD_DIR)
ifeq ($(TEST),)
	@echo "ERROR: Parameter TEST must be defined"
	exit 1
endif
	@cd $(BUILD_DIR) && \
	cmake --build . --target $(TEST_TARGET) -- -j 1 && \
	cd ..

# Create the build directory and convert the CMake configuration to Makefiles
# Want to use the -p switch for mkdir so it doesn't give an error if the
# directory already exists
# mkdir is emulated by New-Item on Windows, but the -p switch doesn't work so we
# call New-Item directly instead
# Based on https://docs.microsoft.com/en-us/powershell/module/microsoft.powershell.management/new-item?view=powershell-7.1 (Example 8)
$(BUILD_DIR):
ifeq ($(MCU),)
	@echo "ERROR: Parameter MCU must be defined"
	exit 1
endif
ifeq ($(WINDOWS),1)
	powershell New-Item -Path $(BUILD_DIR) -ItemType Directory -Force
else
	mkdir -p $(BUILD_DIR)
endif
	@cd $(BUILD_DIR) && \
	cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE=$(BUILD) -DMCU_SERIES=$(MCU) .. && \
	cd ..

# Remove the build directories for all MCU models
.PHONY: clean
clean:
ifeq ($(WINDOWS),1)
	powershell Remove-Item -Path $(BUILD_DIR_BASE)-* -Recurse -ErrorAction Ignore
else
	rm -rf $(BUILD_DIR_BASE)-*
endif

# Remove the build directory for one MCU model
# This forces CMake to regenerate the Makefiles next time a program is compiled
# Similar to the $(BUILD_DIR) target, rm is emulated by Remove-Item on Windows
# The -f switch does not work, so call Remove-Item directly instead
.PHONY: clean_mcu
clean_mcu:
ifeq ($(MCU),)
	@echo "ERROR: Parameter MCU must be defined"
	exit 1
endif
ifeq ($(WINDOWS),1)
	powershell Remove-Item -Path $(BUILD_DIR) -Recurse -ErrorAction Ignore
else
	rm -rf $(BUILD_DIR)
endif




# List info about the available MCUs and serial ports
.PHONY: info
info:
	@echo "MCUs (STLink programmers):"
	st-info --probe
	@echo
	@echo "Serial ports:"
ifeq ($(WINDOWS),1)
	powershell "[System.IO.Ports.SerialPort]::getportnames()"
else
	ls /dev/cu.usbmodem*
endif

# Compile and upload one test program to MCU
# Need the --reset option to reset the MCU (to start executing the program)
# after writing the program to memory, or else the program doesn't start running
# automatically and you need to press the hardware reset button to start it
.PHONY: upload
upload: compile
	st-flash --reset $(SERIAL_ARG) write $(BUILD_DIR)/$(TEST_TARGET).bin 0x8000000

# Download (read) current firmware on MCU
# BYTES defines the number of bytes to read (as a hex value, e.g.
# `make download BYTES=0x1000` to read 4096 bytes)
.PHONY: download
download:
ifeq ($(BYTES),)
	@echo "ERROR: Parameter BYTES must be defined"
	exit 1
endif
	st-flash $(SERIAL_ARG) read downloaded_fw.bin 0x8000000 $(BYTES)

# Erase current firmware on MCU
.PHONY: erase
erase:
	st-flash $(SERIAL_ARG) erase




# Print variable values for debugging
.PHONY: variables
variables:
	@echo "G4_COUNT: $(G4_COUNT)"
	@echo "H7_COUNT: $(H7_COUNT)"
	@echo "MCU_COUNT: $(MCU_COUNT)"
	@echo "MCU: $(MCU)"
