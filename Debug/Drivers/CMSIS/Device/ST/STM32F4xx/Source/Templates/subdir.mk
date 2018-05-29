################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c 

OBJS += \
./Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.o 

C_DEPS += \
./Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/%.o: ../Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F405xx -DARM_MATH_CM4 -I"D:/Documents/Develop/STM32/workspace/F4_BSP/Inc" -I"D:/Documents/Develop/STM32/workspace/F4_BSP/Drivers/STM32F4xx_HAL_Driver/Inc" -I"D:/Documents/Develop/STM32/workspace/F4_BSP/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"D:/Documents/Develop/STM32/workspace/F4_BSP/Drivers/CMSIS/Include" -I"D:/Documents/Develop/STM32/workspace/F4_BSP/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"D:/Documents/Develop/STM32/workspace/F4_BSP/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"D:/Documents/Develop/STM32/workspace/F4_BSP/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"D:/Documents/Develop/STM32/workspace/F4_BSP/" -std=c11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


