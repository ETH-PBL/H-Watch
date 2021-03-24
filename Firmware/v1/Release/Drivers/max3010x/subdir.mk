################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/max3010x/heartrate.c \
../Drivers/max3010x/max3010x.c \
../Drivers/max3010x/spo2_algorithm.c 

OBJS += \
./Drivers/max3010x/heartrate.o \
./Drivers/max3010x/max3010x.o \
./Drivers/max3010x/spo2_algorithm.o 

C_DEPS += \
./Drivers/max3010x/heartrate.d \
./Drivers/max3010x/max3010x.d \
./Drivers/max3010x/spo2_algorithm.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/max3010x/heartrate.o: ../Drivers/max3010x/heartrate.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/max3010x/heartrate.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/max3010x/max3010x.o: ../Drivers/max3010x/max3010x.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/max3010x/max3010x.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/max3010x/spo2_algorithm.o: ../Drivers/max3010x/spo2_algorithm.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/max3010x/spo2_algorithm.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

