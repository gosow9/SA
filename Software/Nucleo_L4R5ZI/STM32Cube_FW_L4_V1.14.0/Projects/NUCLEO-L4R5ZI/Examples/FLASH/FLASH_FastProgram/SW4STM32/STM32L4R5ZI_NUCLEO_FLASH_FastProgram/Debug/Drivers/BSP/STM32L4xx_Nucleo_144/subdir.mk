################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Drivers/BSP/STM32L4xx_Nucleo_144/stm32l4xx_nucleo_144.c 

OBJS += \
./Drivers/BSP/STM32L4xx_Nucleo_144/stm32l4xx_nucleo_144.o 

C_DEPS += \
./Drivers/BSP/STM32L4xx_Nucleo_144/stm32l4xx_nucleo_144.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32L4xx_Nucleo_144/stm32l4xx_nucleo_144.o: C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Drivers/BSP/STM32L4xx_Nucleo_144/stm32l4xx_nucleo_144.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L4R5xx -DUSE_HAL_DRIVER -DUSE_STM32L4XX_NUCLEO_144 -c -I../../../Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../../../Drivers/BSP/STM32L4xx_Nucleo_144 -I../../../../../../../../Drivers/CMSIS/Include -Os -ffunction-sections -Wall -Wno-pointer-to-int-cast -fstack-usage -MMD -MP -MF"Drivers/BSP/STM32L4xx_Nucleo_144/stm32l4xx_nucleo_144.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

