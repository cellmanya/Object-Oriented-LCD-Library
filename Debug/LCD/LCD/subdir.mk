################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LCD/LCD/lcd.c 

OBJS += \
./LCD/LCD/lcd.o 

C_DEPS += \
./LCD/LCD/lcd.d 


# Each subdirectory must supply rules for building sources it contributes
LCD/LCD/lcd.o: ../LCD/LCD/lcd.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/CELL/STM32CubeIDE/workspace_1.5.0/LCD/LCD/LCD" -I"C:/Users/CELL/STM32CubeIDE/workspace_1.5.0/LCD/LCD/LCD_GPIO" -I"C:/Users/CELL/STM32CubeIDE/workspace_1.5.0/LCD/LCD/LCD_I2C" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"LCD/LCD/lcd.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

