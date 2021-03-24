################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32wb55rgvx.s 

OBJS += \
./Core/Startup/startup_stm32wb55rgvx.o 

S_DEPS += \
./Core/Startup/startup_stm32wb55rgvx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/startup_stm32wb55rgvx.o: ../Core/Startup/startup_stm32wb55rgvx.s
	arm-none-eabi-gcc -mcpu=cortex-m4 -c -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -x assembler-with-cpp -MMD -MP -MF"Core/Startup/startup_stm32wb55rgvx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

