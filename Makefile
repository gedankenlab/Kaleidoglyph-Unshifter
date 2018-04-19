# This stub makefile for a Kaleidoglyph plugin pulls in 
# all targets from the Kaleidoglyph-Plugin library

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
SKETCHBOOK_DIR ?= $(HOME)/Documents/Arduino/
else
SKETCHBOOK_DIR ?= $(HOME)/Arduino
endif

BOARD_HARDWARE_PATH ?= $(SKETCHBOOK_DIR)/hardware
KALEIDOGLYPH_PLUGIN_MAKEFILE_DIR ?= keyboardio/avr/build-tools/makefiles/
include $(BOARD_HARDWARE_PATH)/$(KALEIDOGLYPH_PLUGIN_MAKEFILE_DIR)/rules.mk
