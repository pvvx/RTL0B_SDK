#=============================================
# User defined
#=============================================
COM_PORTNUM ?= 6
#SDK_RAM = 1
OTA_IDX ?= 1
SDK_PATH = ./
#---------------------------------------------
ifneq ($(shell uname), Linux)
#=============================================
# if Windows MinGW/MSYS
COM_PORT ?= COM$(COM_PORTNUM)
GCC_PATH ?= d:/MCU/GNU_Tools_ARM_Embedded/7.2017-q4-major/bin/
OPENOCD_PATH ?= d:/MCU/OpenOCD/bin/
PYTHON ?= c:/Python27/python.exe
else
#=============================================
# if WSL
COM_PORT = /dev/ttyS$(COM_PORTNUM)
GCC_PATH ?=~/gcc-arm-none-eabi-7-2017-q4-major/bin/
PYTHON ?= python
USE_WSL = 1
endif
#---------------------------------------------
FLASHER_TYPE = UART
CHIP_SERIES = b
GCCMK_PATH = $(SDK_PATH)mk$(CHIP_SERIES)/

