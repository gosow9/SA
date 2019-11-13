################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Example/User/subdir.mk
-include Example/SW4STM32/subdir.mk
-include Drivers/STM32L4xx_HAL_Driver/subdir.mk
-include Drivers/CMSIS/subdir.mk
-include Drivers/BSP/STM32L4xx_Nucleo_144/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
STM32L4R5ZI_NUCLEO_DMA_FLASHToRAM.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
STM32L4R5ZI_NUCLEO_DMA_FLASHToRAM.list \

OBJCOPY_BIN += \
STM32L4R5ZI_NUCLEO_DMA_FLASHToRAM.bin \


# All Target
all: STM32L4R5ZI_NUCLEO_DMA_FLASHToRAM.elf secondary-outputs

# Tool invocations
STM32L4R5ZI_NUCLEO_DMA_FLASHToRAM.elf: $(OBJS) $(USER_OBJS) ../STM32L4R5ZITx_FLASH.ld
	arm-none-eabi-gcc -o "STM32L4R5ZI_NUCLEO_DMA_FLASHToRAM.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"../STM32L4R5ZITx_FLASH.ld" --specs=nosys.specs -Wl,-Map="STM32L4R5ZI_NUCLEO_DMA_FLASHToRAM.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

STM32L4R5ZI_NUCLEO_DMA_FLASHToRAM.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "STM32L4R5ZI_NUCLEO_DMA_FLASHToRAM.list"
	@echo 'Finished building: $@'
	@echo ' '

STM32L4R5ZI_NUCLEO_DMA_FLASHToRAM.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "STM32L4R5ZI_NUCLEO_DMA_FLASHToRAM.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
