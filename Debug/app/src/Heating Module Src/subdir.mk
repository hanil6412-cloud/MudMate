################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/src/Heating\ Module\ Src/fan.c \
../app/src/Heating\ Module\ Src/fan_speed.c \
../app/src/Heating\ Module\ Src/pid.c \
../app/src/Heating\ Module\ Src/pid_control.c \
../app/src/Heating\ Module\ Src/pwm.c \
../app/src/Heating\ Module\ Src/temp_sensor.c 

OBJS += \
./app/src/Heating\ Module\ Src/fan.o \
./app/src/Heating\ Module\ Src/fan_speed.o \
./app/src/Heating\ Module\ Src/pid.o \
./app/src/Heating\ Module\ Src/pid_control.o \
./app/src/Heating\ Module\ Src/pwm.o \
./app/src/Heating\ Module\ Src/temp_sensor.o 

C_DEPS += \
./app/src/Heating\ Module\ Src/fan.d \
./app/src/Heating\ Module\ Src/fan_speed.d \
./app/src/Heating\ Module\ Src/pid.d \
./app/src/Heating\ Module\ Src/pid_control.d \
./app/src/Heating\ Module\ Src/pwm.d \
./app/src/Heating\ Module\ Src/temp_sensor.d 


# Each subdirectory must supply rules for building sources it contributes
app/src/Heating\ Module\ Src/fan.o: ../app/src/Heating\ Module\ Src/fan.c app/src/Heating\ Module\ Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/develop/stm32/MugMate/app/include" -I"C:/develop/stm32/MugMate/app" -I"C:/develop/stm32/MugMate/app/include/Heating Module Hed" -I"C:/develop/stm32/MugMate/app/include/Touch Module Hed" -I"C:/develop/stm32/MugMate/app/src/Heating Module Src" -I"C:/develop/stm32/MugMate/app/src/Touch Module Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"app/src/Heating Module Src/fan.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
app/src/Heating\ Module\ Src/fan_speed.o: ../app/src/Heating\ Module\ Src/fan_speed.c app/src/Heating\ Module\ Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/develop/stm32/MugMate/app/include" -I"C:/develop/stm32/MugMate/app" -I"C:/develop/stm32/MugMate/app/include/Heating Module Hed" -I"C:/develop/stm32/MugMate/app/include/Touch Module Hed" -I"C:/develop/stm32/MugMate/app/src/Heating Module Src" -I"C:/develop/stm32/MugMate/app/src/Touch Module Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"app/src/Heating Module Src/fan_speed.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
app/src/Heating\ Module\ Src/pid.o: ../app/src/Heating\ Module\ Src/pid.c app/src/Heating\ Module\ Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/develop/stm32/MugMate/app/include" -I"C:/develop/stm32/MugMate/app" -I"C:/develop/stm32/MugMate/app/include/Heating Module Hed" -I"C:/develop/stm32/MugMate/app/include/Touch Module Hed" -I"C:/develop/stm32/MugMate/app/src/Heating Module Src" -I"C:/develop/stm32/MugMate/app/src/Touch Module Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"app/src/Heating Module Src/pid.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
app/src/Heating\ Module\ Src/pid_control.o: ../app/src/Heating\ Module\ Src/pid_control.c app/src/Heating\ Module\ Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/develop/stm32/MugMate/app/include" -I"C:/develop/stm32/MugMate/app" -I"C:/develop/stm32/MugMate/app/include/Heating Module Hed" -I"C:/develop/stm32/MugMate/app/include/Touch Module Hed" -I"C:/develop/stm32/MugMate/app/src/Heating Module Src" -I"C:/develop/stm32/MugMate/app/src/Touch Module Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"app/src/Heating Module Src/pid_control.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
app/src/Heating\ Module\ Src/pwm.o: ../app/src/Heating\ Module\ Src/pwm.c app/src/Heating\ Module\ Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/develop/stm32/MugMate/app/include" -I"C:/develop/stm32/MugMate/app" -I"C:/develop/stm32/MugMate/app/include/Heating Module Hed" -I"C:/develop/stm32/MugMate/app/include/Touch Module Hed" -I"C:/develop/stm32/MugMate/app/src/Heating Module Src" -I"C:/develop/stm32/MugMate/app/src/Touch Module Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"app/src/Heating Module Src/pwm.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
app/src/Heating\ Module\ Src/temp_sensor.o: ../app/src/Heating\ Module\ Src/temp_sensor.c app/src/Heating\ Module\ Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/develop/stm32/MugMate/app/include" -I"C:/develop/stm32/MugMate/app" -I"C:/develop/stm32/MugMate/app/include/Heating Module Hed" -I"C:/develop/stm32/MugMate/app/include/Touch Module Hed" -I"C:/develop/stm32/MugMate/app/src/Heating Module Src" -I"C:/develop/stm32/MugMate/app/src/Touch Module Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"app/src/Heating Module Src/temp_sensor.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app-2f-src-2f-Heating-20-Module-20-Src

clean-app-2f-src-2f-Heating-20-Module-20-Src:
	-$(RM) ./app/src/Heating\ Module\ Src/fan.cyclo ./app/src/Heating\ Module\ Src/fan.d ./app/src/Heating\ Module\ Src/fan.o ./app/src/Heating\ Module\ Src/fan.su ./app/src/Heating\ Module\ Src/fan_speed.cyclo ./app/src/Heating\ Module\ Src/fan_speed.d ./app/src/Heating\ Module\ Src/fan_speed.o ./app/src/Heating\ Module\ Src/fan_speed.su ./app/src/Heating\ Module\ Src/pid.cyclo ./app/src/Heating\ Module\ Src/pid.d ./app/src/Heating\ Module\ Src/pid.o ./app/src/Heating\ Module\ Src/pid.su ./app/src/Heating\ Module\ Src/pid_control.cyclo ./app/src/Heating\ Module\ Src/pid_control.d ./app/src/Heating\ Module\ Src/pid_control.o ./app/src/Heating\ Module\ Src/pid_control.su ./app/src/Heating\ Module\ Src/pwm.cyclo ./app/src/Heating\ Module\ Src/pwm.d ./app/src/Heating\ Module\ Src/pwm.o ./app/src/Heating\ Module\ Src/pwm.su ./app/src/Heating\ Module\ Src/temp_sensor.cyclo ./app/src/Heating\ Module\ Src/temp_sensor.d ./app/src/Heating\ Module\ Src/temp_sensor.o ./app/src/Heating\ Module\ Src/temp_sensor.su

.PHONY: clean-app-2f-src-2f-Heating-20-Module-20-Src

