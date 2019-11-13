################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Projects/NUCLEO-L4R5ZI/Examples/DMA/DMA_FLASHToRAM/SW4STM32/startup_stm32l4r5xx.s 

OBJS += \
./Example/SW4STM32/startup_stm32l4r5xx.o 


# Each subdirectory must supply rules for building sources it contributes
Example/SW4STM32/startup_stm32l4r5xx.o: C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Projects/NUCLEO-L4R5ZI/Examples/DMA/DMA_FLASHToRAM/SW4STM32/startup_stm32l4r5xx.s
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -x assembler-with-cpp --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

