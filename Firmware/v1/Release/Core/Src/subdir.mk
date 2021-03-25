################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Display.c \
../Core/Src/Int_Sensors.c \
../Core/Src/NB_BC95_task.c \
../Core/Src/app_freertos.c \
../Core/Src/dma.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/main.c \
../Core/Src/rtc.c \
../Core/Src/spi.c \
../Core/Src/stm32wbxx_hal_msp.c \
../Core/Src/stm32wbxx_hal_timebase_tim.c \
../Core/Src/stm32wbxx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32wbxx.c \
../Core/Src/time_op_F.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/Display.o \
./Core/Src/Int_Sensors.o \
./Core/Src/NB_BC95_task.o \
./Core/Src/app_freertos.o \
./Core/Src/dma.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/main.o \
./Core/Src/rtc.o \
./Core/Src/spi.o \
./Core/Src/stm32wbxx_hal_msp.o \
./Core/Src/stm32wbxx_hal_timebase_tim.o \
./Core/Src/stm32wbxx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32wbxx.o \
./Core/Src/time_op_F.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/Display.d \
./Core/Src/Int_Sensors.d \
./Core/Src/NB_BC95_task.d \
./Core/Src/app_freertos.d \
./Core/Src/dma.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/main.d \
./Core/Src/rtc.d \
./Core/Src/spi.d \
./Core/Src/stm32wbxx_hal_msp.d \
./Core/Src/stm32wbxx_hal_timebase_tim.d \
./Core/Src/stm32wbxx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32wbxx.d \
./Core/Src/time_op_F.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Display.o: ../Core/Src/Display.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/Display.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Int_Sensors.o: ../Core/Src/Int_Sensors.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/Int_Sensors.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/NB_BC95_task.o: ../Core/Src/NB_BC95_task.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/NB_BC95_task.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/app_freertos.o: ../Core/Src/app_freertos.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/app_freertos.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/dma.o: ../Core/Src/dma.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/dma.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/gpio.o: ../Core/Src/gpio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/gpio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/i2c.o: ../Core/Src/i2c.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/i2c.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/main.o: ../Core/Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/rtc.o: ../Core/Src/rtc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/rtc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/spi.o: ../Core/Src/spi.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/spi.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32wbxx_hal_msp.o: ../Core/Src/stm32wbxx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32wbxx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32wbxx_hal_timebase_tim.o: ../Core/Src/stm32wbxx_hal_timebase_tim.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32wbxx_hal_timebase_tim.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32wbxx_it.o: ../Core/Src/stm32wbxx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32wbxx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/syscalls.o: ../Core/Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/syscalls.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/sysmem.o: ../Core/Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/system_stm32wbxx.o: ../Core/Src/system_stm32wbxx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/system_stm32wbxx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/time_op_F.o: ../Core/Src/time_op_F.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/time_op_F.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/usart.o: ../Core/Src/usart.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lps22hb_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm303agr_STdC" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/lsm6dsm_STdC/driver" -I"C:/Users/schuluka/STM32CubeIDE/workspace_1.4.0/GSM_watch/smartwatch/Firmware/ETH_v1/v1/Drivers/max3010x" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/usart.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
