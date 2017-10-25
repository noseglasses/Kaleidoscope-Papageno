# This stub makefile for a Kaleidoscope plugin pulls in 
# all targets from the Kaleidoscope-Plugin library

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
SKETCHBOOK_DIR ?= $(HOME)/Documents/Arduino/
else
SKETCHBOOK_DIR ?= $(HOME)/Arduino
endif

BOARD_HARDWARE_PATH ?= $(SKETCHBOOK_DIR)/hardware
KALEIDOSCOPE_PLUGIN_MAKEFILE_DIR ?= keyboardio/avr/build-tools/makefiles/
include $(BOARD_HARDWARE_PATH)/$(KALEIDOSCOPE_PLUGIN_MAKEFILE_DIR)/rules.mk

papageno:
	cd $(BOARD_HARDWARE_PATH)/keyboardio/avr/libraries/Kaleidoscope-Papageno/3rd-party/Papageno
	mkdir -p build/atmega32u4
	cd build/atmega32u4
	cmake \
		-DCMAKE_TOOLCHAIN_FILE=$PWD/../../cmake/toolchains/Toolchain-avr-gcc.cmake \
		-DPAPAGENO_PLATFORM=atmega32u4 \
		../..
	make
