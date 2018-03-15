#=============================================
# Flasher Config
#=============================================
ifneq ($(shell uname), Linux)
COM_PORT ?= COM6
else
COM_PORT = /dev/ttyS6
#sudo chmod 666 $(COM_PORT) !
endif
FLASHER_TYPE = UART
#=============================================
# User defined
#=============================================
CHIP_SERIES = b
#SDK_RAM = 1
OTA_IDX ?= 1
#=============================================
SDK_PATH = ./
#GCC_PATH set in PATH?
ifneq ($(shell uname), Linux)
GCC_PATH ?= d:/MCU/GNU_Tools_ARM_Embedded/7.2017-q4-major/bin/
OPENOCD_PATH ?= d:/MCU/OpenOCD/bin/
PYTHON ?= c:/Python27/python.exe
else
GCC_PATH ?=~/gcc-arm-none-eabi-7-2017-q4-major/bin/
PYTHON ?= python
endif
#---------------------------------------------
GCCMK_PATH = $(SDK_PATH)mk$(CHIP_SERIES)/

