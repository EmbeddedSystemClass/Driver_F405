################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../Src/startup_stm32f405xx.asm 

C_SRCS += \
../Src/LPS25HB_Driver.c \
../Src/MotionFX_Manager.c \
../Src/can.c \
../Src/dma.c \
../Src/gpio.c \
../Src/i2c.c \
../Src/main.c \
../Src/mcp23017.c \
../Src/rtc.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/system_stm32f4xx.c \
../Src/tim.c \
../Src/usb_device.c \
../Src/usbd_cdc_if.c \
../Src/usbd_conf.c \
../Src/usbd_desc.c 

OBJS += \
./Src/LPS25HB_Driver.o \
./Src/MotionFX_Manager.o \
./Src/can.o \
./Src/dma.o \
./Src/gpio.o \
./Src/i2c.o \
./Src/main.o \
./Src/mcp23017.o \
./Src/rtc.o \
./Src/startup_stm32f405xx.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/system_stm32f4xx.o \
./Src/tim.o \
./Src/usb_device.o \
./Src/usbd_cdc_if.o \
./Src/usbd_conf.o \
./Src/usbd_desc.o 

ASM_DEPS += \
./Src/startup_stm32f405xx.d 

C_DEPS += \
./Src/LPS25HB_Driver.d \
./Src/MotionFX_Manager.d \
./Src/can.d \
./Src/dma.d \
./Src/gpio.d \
./Src/i2c.d \
./Src/main.d \
./Src/mcp23017.d \
./Src/rtc.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/system_stm32f4xx.d \
./Src/tim.d \
./Src/usb_device.d \
./Src/usbd_cdc_if.d \
./Src/usbd_conf.d \
./Src/usbd_desc.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F405xx -DARM_MATH_CM4 -D__FPU_PRESENT=1U -I"C:/Users/Fabio/Documents/Develop/SRM32/workspace/F4_BSP/Inc" -I"C:/Users/Fabio/Documents/Develop/SRM32/workspace/F4_BSP/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Fabio/Documents/Develop/SRM32/workspace/F4_BSP/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Fabio/Documents/Develop/SRM32/workspace/F4_BSP/Drivers/CMSIS/Include" -I"C:/Users/Fabio/Documents/Develop/SRM32/workspace/F4_BSP/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Fabio/Documents/Develop/SRM32/workspace/F4_BSP/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Users/Fabio/Documents/Develop/SRM32/workspace/F4_BSP/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Users/Fabio/Documents/Develop/SRM32/workspace/F4_BSP/" -std=c11 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/%.o: ../Src/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


