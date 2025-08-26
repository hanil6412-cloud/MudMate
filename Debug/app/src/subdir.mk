################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/src/uart.c \
../app/src/utils.c 

OBJS += \
./app/src/uart.o \
./app/src/utils.o 

C_DEPS += \
./app/src/uart.d \
./app/src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
app/src/%.o app/src/%.su app/src/%.cyclo: ../app/src/%.c app/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/develop/stm32/MugMate/app/include" -I"C:/develop/stm32/MugMate/app" -I"C:/develop/stm32/MugMate/app/include/Heating Module Hed" -I"C:/develop/stm32/MugMate/app/include/Touch Module Hed" -I"C:/develop/stm32/MugMate/app/src/Heating Module Src" -I"C:/develop/stm32/MugMate/app/src/Touch Module Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app-2f-src

clean-app-2f-src:
	-$(RM) ./app/src/uart.cyclo ./app/src/uart.d ./app/src/uart.o ./app/src/uart.su ./app/src/utils.cyclo ./app/src/utils.d ./app/src/utils.o ./app/src/utils.su

.PHONY: clean-app-2f-src

