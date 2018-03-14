include userset.mk
include $(GCCMK_PATH)paths.mk
include project.mk

IMAGE2_OTA1=ota1.bin 
IMAGE2_OTA2=ota2.bin

INCFLAGS = $(patsubst %,-I%,$(patsubst sdk/%,$(SDK_PATH)%,$(INCLUDES)))

LIBFLAGS = $(addprefix -L,$(patsubst sdk/%,$(SDK_PATH)%,$(PATHLIBS))) $(addprefix -l,$(LIBS))

LFLAGS += -Wl,-Map=$(OBJ_DIR)/$(TARGET).map 

CFLAGS += $(INCFLAGS)

SRC_O = $(patsubst %.c,%.o,$(patsubst sdk/%,$(SDK_PATH)%,$(ADD_SRC_C))) $(patsubst %.c,%.o,$(patsubst sdk/%,$(SDK_PATH)%,$(SRC_C)))
DRAM_O = $(patsubst %.c,%.o,$(patsubst sdk/%,$(SDK_PATH)%,$(DRAM_C)))
BOOT_O = $(patsubst %.c,%.o,$(patsubst sdk/%,$(SDK_PATH)%,$(BOOT_C)))

SRC_C_LIST = $(patsubst sdk/%,$(SDK_PATH)%,$(ADD_SRC_C)) $(patsubst sdk/%,$(SDK_PATH)%,$(SRC_C)) $(patsubst sdk/%,$(SDK_PATH)%,$(DRAM_C)) $(patsubst sdk/%,$(SDK_PATH)%,$(BOOT_C))
OBJ_LIST = $(addprefix $(OBJ_DIR)/,$(patsubst %.c,%.o,$(SRC_C_LIST)))
DEPENDENCY_LIST = $(patsubst %.c,$(OBJ_DIR)/%.d,$(SRC_C_LIST))

TARGET ?= build
OBJ_DIR ?= $(TARGET)/obj
BIN_DIR ?= $(TARGET)/bin
ELFFILE ?= $(OBJ_DIR)/$(TARGET).axf

all: prerequirement application
mp: prerequirement application

.PHONY: build_info
build_info:
	@echo \#define UTS_VERSION \"`date +%Y/%m/%d-%T`\" > .ver
	@echo \#define RTL8195AFW_COMPILE_TIME \"`date +%Y/%m/%d-%T`\" >> .ver
	@echo \#define RTL8195AFW_COMPILE_DATE \"`date +%Y%m%d`\" >> .ver
	@echo \#define RTL8195AFW_COMPILE_BY \"`id -u -n`\" >> .ver
	@echo \#define RTL8195AFW_COMPILE_HOST \"`$(HOSTNAME_APP)`\" >> .ver
	@if [ -x /bin/dnsdomainname ]; then \
		echo \#define RTL8195AFW_COMPILE_DOMAIN \"`dnsdomainname`\"; \
	elif [ -x /bin/domainname ]; then \
		echo \#define RTL8195AFW_COMPILE_DOMAIN \"`domainname`\"; \
	else \
		echo \#define RTL8195AFW_COMPILE_DOMAIN ; \
	fi >> .ver
	@echo \#define RTL195AFW_COMPILER \"gcc `$(CC) $(CFLAGS) -dumpversion | tr --delete '\r'`\" >> .ver
	@mv -f .ver project/inc/$@.h
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)
	@cp $(patsubst sdk/%,$(SDK_PATH)%,$(BOOT_ALL)) $(BIN_DIR)/boot_all.bin
	@chmod 777 $(BIN_DIR)/boot_all.bin
	@$(OBJCOPY) -I binary -O elf32-littlearm -B arm $(BIN_DIR)/boot_all.bin $(OBJ_DIR)/boot_all.o 

.PHONY:	application 
application: build_info $(SRC_O) $(DRAM_O) $(BOOT_O)
	@echo "==========================================================="
	@echo "Link ($(TARGET))"
#	@echo "==========================================================="
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)
	@$(file > $(OBJ_DIR)/obj_list.lst,$(OBJ_LIST))
	@$(LD) $(LFLAGS) -o $(ELFFILE) @$(OBJ_DIR)/obj_list.lst $(LIBFLAGS) -T$(patsubst sdk/%,$(SDK_PATH)%,$(LDFILE))
#	@$(OBJDUMP) -D -S $(ELFFILE) > $(OBJ_DIR)/$(TARGET).asm
	@$(OBJDUMP) -d $(ELFFILE) > $(OBJ_DIR)/$(TARGET).asm
	@echo ===========================================================
	@echo Image manipulating
	@echo ===========================================================
	@$(NM) $(OBJ_DIR)/$(TARGET).axf | sort > $(OBJ_DIR)/$(TARGET).nmap
	@$(PYTHON) $(GCCMK_PATH)rtlimage.py -o $(BIN_DIR)/ -a $(OBJ_DIR)/$(TARGET).axf
	@mv $(BIN_DIR)/ota.bin $(BIN_DIR)/ota$(OTA_IDX).bin

.PHONY:	prerequirement
#.NOTPARALLEL: prerequirement
prerequirement:
	@echo "==========================================================="
	@echo "Compile ($(TARGET))"
#	@echo "==========================================================="
	@mkdir -p $(OBJ_DIR)

$(SRC_O): %.o : %.c
	@echo $<
	@mkdir -p $(OBJ_DIR)/$(dir $@)
	@$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $(OBJ_DIR)/$@
	@$(CC) -MM $(CFLAGS) $(INCFLAGS) $< -MT $@ -MF $(OBJ_DIR)/$(patsubst %.o,%.d,$@)

$(DRAM_O): %.o : %.c
	@echo $<
	@mkdir -p $(OBJ_DIR)/$(dir $@)
	@$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $(OBJ_DIR)/$@
	@$(OBJCOPY) --prefix-alloc-sections .sdram $(OBJ_DIR)/$@
	@$(CC) -MM $(CFLAGS) $(INCFLAGS) $< -MT $@ -MF $(OBJ_DIR)/$(patsubst %.o,%.d,$@)

$(BOOT_O): %.o : %.c
	@echo $<
	@mkdir -p $(OBJ_DIR)/$(dir $@)
	@$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $(OBJ_DIR)/$@
	@$(OBJCOPY) --prefix-alloc-sections .boot $(OBJ_DIR)/$@
	@$(CC) -MM $(CFLAGS) $(INCFLAGS) $< -MT $@ -MF $(OBJ_DIR)/$(patsubst %.o,%.d,$@)
	
-include $(DEPENDENCY_LIST)

VPATH:=$(OBJ_DIR) $(SDK_PATH)

#.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(OBJ_DIR)/$(SDK_PATH) 

ifeq ($(FLASHER_TYPE), UART)
	
.PHONY: flash_ota1
flash_ota1: 
	@$(PYTHON)	$(GCCMK_PATH)rtltool.py -p $(COM_PORT) wf 0x0B000 $(BIN_DIR)/$(IMAGE2_OTA1)

.PHONY: flash_ota2
flash_ota2:
	@$(PYTHON)	$(GCCMK_PATH)rtltool.py -p $(COM_PORT) wf 0x80000 $(BIN_DIR)/$(IMAGE2_OTA2)	

.PHONY: flash_boot
flash_boot:
	@$(PYTHON)	$(GCCMK_PATH)rtltool.py -p $(COM_PORT) wf 0x00000 $(BIN_DIR)/boot_all.bin

.PHONY: flash_sys
flash_sys:
	@$(PYTHON)	$(GCCMK_PATH)rtltool.py -p $(COM_PORT) wf 0x09000 $(GCCMK_PATH)0x9000.bin

.PHONY: run_ram
run_ram:
	@$(PYTHON)	$(GCCMK_PATH)rtltool.py -p $(COM_PORT) wm 0x10002000 $(BIN_DIR)/ram_1.r.bin
	
.PHONY: go_rom_monitor	
go_rom_monitor:
	@$(PYTHON)	$(GCCMK_PATH)rtltool.py -p $(COM_PORT) gm

.PHONY: flash_read
flash_read:
	@$(PYTHON)	$(GCCMK_PATH)rtltool.py -p $(COM_PORT) rf 0x00000 0x100000 $(BIN_DIR)/flash.bin
else
ifeq ($(FLASHER_TYPE),Jlink)

else
ifeq ($(FLASHER_TYPE),cmsis-dap)

endif
endif
endif	