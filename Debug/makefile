################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Drivers/LSM9DS1-Driver/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/FastMathFunctions/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/ControllerFunctions/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/CommonTables/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
F4_BSP.hex \

SECONDARY_SIZE += \
F4_BSP.siz \


# All Target
all: F4_BSP.elf secondary-outputs

# Tool invocations
F4_BSP.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -T C:/Users/Fabio/Documents/Develop/SRM32/workspace/F4_BSP/SW4STM32/F4_BSP/STM32F405RGTx_FLASH.ld -Xlinker --gc-sections -L"C:\Users\Fabio\Documents\Develop\SRM32\workspace\F4_BSP\Middlewares\ST\motion_fx" -Wl,-Map,"F4_BSP.map" --specs=nosys.specs --specs=rdimon.specs -o "F4_BSP.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

F4_BSP.hex: F4_BSP.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "F4_BSP.elf"  "F4_BSP.hex"
	@echo 'Finished building: $@'
	@echo ' '

F4_BSP.siz: F4_BSP.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "F4_BSP.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) F4_BSP.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
