################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c \
C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c \
C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c \
C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c \
C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c \
C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c \
C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c \
C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c 

OBJS += \
./Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal.o \
./Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_cortex.o \
./Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_flash.o \
./Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_flash_ex.o \
./Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_gpio.o \
./Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_pwr.o \
./Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_pwr_ex.o \
./Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_rcc.o 

C_DEPS += \
./Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal.d \
./Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_cortex.d \
./Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_flash.d \
./Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_flash_ex.d \
./Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_gpio.d \
./Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_pwr.d \
./Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_pwr_ex.d \
./Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_rcc.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal.o: C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L4R5xx -DUSE_HAL_DRIVER -DUSE_STM32L4XX_NUCLEO_144 -c -I../../../Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../../../Drivers/BSP/STM32L4xx_Nucleo_144 -I../../../../../../../../Drivers/CMSIS/Include -Os -ffunction-sections -Wall -Wno-pointer-to-int-cast -fstack-usage -MMD -MP -MF"Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_cortex.o: C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L4R5xx -DUSE_HAL_DRIVER -DUSE_STM32L4XX_NUCLEO_144 -c -I../../../Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../../../Drivers/BSP/STM32L4xx_Nucleo_144 -I../../../../../../../../Drivers/CMSIS/Include -Os -ffunction-sections -Wall -Wno-pointer-to-int-cast -fstack-usage -MMD -MP -MF"Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_cortex.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_flash.o: C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L4R5xx -DUSE_HAL_DRIVER -DUSE_STM32L4XX_NUCLEO_144 -c -I../../../Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../../../Drivers/BSP/STM32L4xx_Nucleo_144 -I../../../../../../../../Drivers/CMSIS/Include -Os -ffunction-sections -Wall -Wno-pointer-to-int-cast -fstack-usage -MMD -MP -MF"Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_flash.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_flash_ex.o: C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L4R5xx -DUSE_HAL_DRIVER -DUSE_STM32L4XX_NUCLEO_144 -c -I../../../Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../../../Drivers/BSP/STM32L4xx_Nucleo_144 -I../../../../../../../../Drivers/CMSIS/Include -Os -ffunction-sections -Wall -Wno-pointer-to-int-cast -fstack-usage -MMD -MP -MF"Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_flash_ex.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_gpio.o: C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L4R5xx -DUSE_HAL_DRIVER -DUSE_STM32L4XX_NUCLEO_144 -c -I../../../Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../../../Drivers/BSP/STM32L4xx_Nucleo_144 -I../../../../../../../../Drivers/CMSIS/Include -Os -ffunction-sections -Wall -Wno-pointer-to-int-cast -fstack-usage -MMD -MP -MF"Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_gpio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_pwr.o: C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L4R5xx -DUSE_HAL_DRIVER -DUSE_STM32L4XX_NUCLEO_144 -c -I../../../Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../../../Drivers/BSP/STM32L4xx_Nucleo_144 -I../../../../../../../../Drivers/CMSIS/Include -Os -ffunction-sections -Wall -Wno-pointer-to-int-cast -fstack-usage -MMD -MP -MF"Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_pwr.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_pwr_ex.o: C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L4R5xx -DUSE_HAL_DRIVER -DUSE_STM32L4XX_NUCLEO_144 -c -I../../../Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../../../Drivers/BSP/STM32L4xx_Nucleo_144 -I../../../../../../../../Drivers/CMSIS/Include -Os -ffunction-sections -Wall -Wno-pointer-to-int-cast -fstack-usage -MMD -MP -MF"Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_pwr_ex.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_rcc.o: C:/HSR/SA/Software/Nucleo_L4R5ZI/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L4R5xx -DUSE_HAL_DRIVER -DUSE_STM32L4XX_NUCLEO_144 -c -I../../../Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../../../Drivers/BSP/STM32L4xx_Nucleo_144 -I../../../../../../../../Drivers/CMSIS/Include -Os -ffunction-sections -Wall -Wno-pointer-to-int-cast -fstack-usage -MMD -MP -MF"Drivers/STM32L4xx_HAL_Driver/stm32l4xx_hal_rcc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

