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
# the MCU model (currently can be "G431", "G474", or "H743")
# e.g. make ... MCU=G431
# e.g. make ... MCU=G474
# e.g. make ... MCU=H743

# Default build type
BUILD = Debug
# Base folder name for CMake configuration and builds
BUILD_DIR_BASE = Build
# Folder for CMake configuration and builds
BUILD_DIR = $(BUILD_DIR_BASE)-$(MCU)

# Strip leading dot and slash in the test path if there is one
# Strip trailing slash in the test path if there is one
# Use both forward slash (/) and backward slash (\) for Windows compatibility
# Need to use a simply expanded variable (:=) instead of a recursively expanded
# variable (=) or else TEST_TARGET would be recursively defined in terms of
# itself
TEST_TARGET := $(TEST)
TEST_TARGET := $(TEST_TARGET:.%=%)
# Must be a single forward slash (/) to work
TEST_TARGET := $(TEST_TARGET:/%=%)
# Must be a double backslash (\\) to work (not sure why)
TEST_TARGET := $(TEST_TARGET:\\%=%)
# Must be a single forward slash (/) to work
TEST_TARGET := $(TEST_TARGET:%/=%)
# Must be a single backslash (\) to work (not sure why this is different from the prefix \)
TEST_TARGET := $(TEST_TARGET:%\=%)
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

# If DETECT=1, detect how many MCUs of each model are connected
# Setting DETECT=1 causes this to execute the `st-info --probe` command, which
# resets all MCUs connected to the computer, causing each MCU to restart its
# current program
ifeq ($(DETECT),1)
	ifeq ($(WINDOWS),1)
		G431_COUNT = $(shell powershell "(st-info --probe | select-string -pattern 'G431').length")
		G474_COUNT = $(shell powershell "(st-info --probe | select-string -pattern 'G474').length")
		H743_COUNT = $(shell powershell "(st-info --probe | select-string -pattern 'H7').length")
		MCU_COUNT = $(shell powershell $(G431_COUNT) + $(G474_COUNT) + $(H743_COUNT))
	else
		G431_COUNT = $(shell st-info --probe | grep -c G431)
		G474_COUNT = $(shell st-info --probe | grep -c G474)
		H743_COUNT = $(shell st-info --probe | grep -c H7)
		MCU_COUNT = $(shell expr $(G431_COUNT) + $(G474_COUNT) + $(H743_COUNT))
	endif
endif

# Automatically detect the MCU model and set the MCU variable if there is only
# one MCU connected
# This only overwrites the MCU variable if it was not set manually
ifeq ($(MCU_COUNT),1)
	ifeq ($(G431_COUNT),1)
		MCU = G431
	endif
	ifeq ($(G474_COUNT),1)
		MCU = G474
	endif
	ifeq ($(H743_COUNT),1)
		MCU = H743
	endif
endif

# Serial argument (if specified)
# i.e. which MCU to upload to
ifneq ($(SERIAL),)
	SERIAL_ARG = --serial $(SERIAL)
endif

# Default value for FILE variable (for `make download`)
ifeq ($(FILE),)
	FILE = downloaded_fw.bin
endif




# -----------------------------------------------------------------------------

# Compile all test programs for all MCU models
# This command is useful to run before committing or merging code, to at least
# make sure all programs still compile correctly
.PHONY: all
all:
	make all_mcu MCU=G431
	make all_mcu MCU=G474
	make all_mcu MCU=H743

# Compile all test programs for one MCU model
.PHONY: all_mcu
all_mcu: $(BUILD_DIR)
	cd $(BUILD_DIR) && \
	cmake --build . -- -j 1 && \
	cd ..

# Compile one test program for one MCU model
.PHONY: compile
compile: $(BUILD_DIR)
ifeq ($(TEST),)
	@echo "ERROR: Parameter TEST must be defined"
	exit 1
endif
	cd $(BUILD_DIR) && \
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
	@echo "ERROR: Parameter MCU must be defined, or set DETECT=1 if one MCU is connected"
	exit 1
endif
ifeq ($(WINDOWS),1)
	powershell New-Item -Path $(BUILD_DIR) -ItemType Directory -Force
else
	mkdir -p $(BUILD_DIR)
endif
	cd $(BUILD_DIR) && \
	cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE=$(BUILD) -DMCU=$(MCU) .. && \
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
	@echo "ERROR: Parameter MCU must be defined, or set DETECT=1 if one MCU is connected"
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
	@echo ""
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

# Download (read) current firmware from MCU
# BYTES defines the number of bytes to read, as either a decimal value or a hex
# value, e.g. `make download BYTES=0x1000` or `make download BYTES=4096` to read
# 4096 bytes)
# FILE defaults to downloaded_fw.bin, but can be manually specified
.PHONY: download
download:
ifeq ($(BYTES),)
	@echo "ERROR: Parameter BYTES must be defined"
	exit 1
endif
	st-flash $(SERIAL_ARG) read $(FILE) 0x8000000 $(BYTES)

# Erase current firmware on MCU
.PHONY: erase
erase:
	st-flash $(SERIAL_ARG) erase




# Print variable values for debugging
.PHONY: variables
variables:
	@echo "G431_COUNT: $(G431_COUNT)"
	@echo "G474_COUNT: $(G474_COUNT)"
	@echo "H743_COUNT: $(H743_COUNT)"
	@echo "MCU_COUNT: $(MCU_COUNT)"
	@echo "MCU: $(MCU)"