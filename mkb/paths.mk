#---------------------------
# User defined (in userset.mk)
#---------------------------
SDK_PATH ?= ../RTL0B_SDK/
ifneq ($(shell uname), Linux)
GCC_PATH ?=~/gcc-arm-none-eabi-7-2017-q4-major/bin/
PYTHON ?= python
COM_PORT ?= /dev/ttyS2
else
GCC_PATH ?= d:/MCU/GNU_Tools_ARM_Embedded/7.2017-q4-major/bin/
OPENOCD_PATH ?= d:/MCU/OpenOCD/bin/
PYTHON ?= c:/Python27/python.exe
COM_PORT ?= COM2
endif
FLASHER_TYPE ?= UART
#---------------------------
# Default
#---------------------------
# Compilation tools
CROSS_COMPILE = $(GCC_PATH)arm-none-eabi-
AR = $(CROSS_COMPILE)ar
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
NM = $(CROSS_COMPILE)nm
LD = $(CROSS_COMPILE)gcc
GDB = $(CROSS_COMPILE)gdb
SIZE = $(CROSS_COMPILE)size
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

# TARGET dirs
TARGET ?= build
OBJ_DIR ?= $(TARGET)/obj
BIN_DIR ?= $(TARGET)/bin
ELFFILE ?= $(OBJ_DIR)/$(TARGET).axf

# openocd tools
OPENOCD ?= $(OPENOCD_PATH)openocd
