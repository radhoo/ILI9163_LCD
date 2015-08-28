################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include spi/subdir.mk
-include ili9163/subdir.mk
-include digitalpin/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
lcd_ili9163.lss \

FLASH_IMAGE += \
lcd_ili9163.hex \

EEPROM_IMAGE += \
lcd_ili9163.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: lcd_ili9163.elf secondary-outputs

# Tool invocations
lcd_ili9163.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C++ Linker'
	avr-g++ -Wl,-Map,lcd_ili9163.map,--cref -mrelax -Wl,--gc-sections -Wl,-u,vfprintf -lprintf_flt -lm -Llibprintf_flt.a -Llibm.a -mmcu=atmega128a -o "lcd_ili9163.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

lcd_ili9163.lss: lcd_ili9163.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S lcd_ili9163.elf  >"lcd_ili9163.lss"
	@echo 'Finished building: $@'
	@echo ' '

lcd_ili9163.hex: lcd_ili9163.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex lcd_ili9163.elf  "lcd_ili9163.hex"
	@echo 'Finished building: $@'
	@echo ' '

lcd_ili9163.eep: lcd_ili9163.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex lcd_ili9163.elf  "lcd_ili9163.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: lcd_ili9163.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega128a lcd_ili9163.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(S_DEPS)$(LSS)$(C_DEPS)$(FLASH_IMAGE)$(CC_DEPS)$(ELFS)$(C++_DEPS)$(OBJS)$(CXX_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(SIZEDUMMY)$(S_UPPER_DEPS)$(CPP_DEPS) lcd_ili9163.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
