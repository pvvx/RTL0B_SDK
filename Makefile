include userset.mk

all: ram_all

mp: ram_all_mp

flash: flash_boot flash_sys flash_ota1 flash_ota2

.PHONY: ram_all
ram_all:
	@$(MAKE) -f $(GCCMK_PATH)sdkbuild.mk

.PHONY: ram_all_mp
ram_all_mp:
	@$(MAKE) -f $(GCCMK_PATH)sdkbuild.mk mp

.PHONY: flash_ota1
flash_ota1:
	@$(MAKE) -f $(GCCMK_PATH)sdkbuild.mk flash_ota1

.PHONY: flash_ota2
flash_ota2:
	@$(MAKE) -f $(GCCMK_PATH)sdkbuild.mk flash_ota2
	
.PHONY: flash_boot
flash_boot:
	@$(MAKE) -f $(GCCMK_PATH)sdkbuild.mk flash_boot

.PHONY: flash_sys
flash_sys:
	@$(MAKE) -f $(GCCMK_PATH)sdkbuild.mk flash_sys

.PHONY: run_ram
run_ram:
	@$(MAKE) -f $(GCCMK_PATH)sdkbuild.mk run_ram

.PHONY: go_rom_monitor	
go_rom_monitor:
	@$(MAKE) -f $(GCCMK_PATH)sdkbuild.mk go_rom_monitor
		
.PHONY: clean
clean:
	@$(MAKE) -f $(GCCMK_PATH)sdkbuild.mk clean

	
