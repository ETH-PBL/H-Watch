################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Drivers/max3010x/subdir.mk
-include Drivers/lsm6dsm_STdC/driver/subdir.mk
-include Drivers/lsm303agr_STdC/driver/subdir.mk
-include Drivers/lps22hb_STdC/driver/subdir.mk
-include Drivers/STM32WBxx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
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
EXECUTABLES += \
v1.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
v1.list \

OBJCOPY_BIN += \
v1.bin \


# All Target
all: v1.elf secondary-outputs

# Tool invocations
v1.elf: $(OBJS) $(USER_OBJS) C:\Users\schuluka\STM32CubeIDE\workspace_1.4.0\GSM_watch\smartwatch\Firmware\ETH_v1\v1\STM32WB55RGVX_FLASH.ld
	arm-none-eabi-gcc -o "v1.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\schuluka\STM32CubeIDE\workspace_1.4.0\GSM_watch\smartwatch\Firmware\ETH_v1\v1\STM32WB55RGVX_FLASH.ld" --specs=nosys.specs -Wl,-Map="v1.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

v1.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "v1.list"
	@echo 'Finished building: $@'
	@echo ' '

v1.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "v1.bin"
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
