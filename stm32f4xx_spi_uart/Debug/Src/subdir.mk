################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/ad5641.c \
../Src/adf4159.c \
../Src/bgt24mtr11.c \
../Src/bitDecode.c \
../Src/fsk_crc16.c \
../Src/main.c \
../Src/microsec_delay.c \
../Src/rs232_menu.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/system_stm32f4xx.c \
../Src/uartPrint.c 

OBJS += \
./Src/ad5641.o \
./Src/adf4159.o \
./Src/bgt24mtr11.o \
./Src/bitDecode.o \
./Src/fsk_crc16.o \
./Src/main.o \
./Src/microsec_delay.o \
./Src/rs232_menu.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/system_stm32f4xx.o \
./Src/uartPrint.o 

C_DEPS += \
./Src/ad5641.d \
./Src/adf4159.d \
./Src/bgt24mtr11.d \
./Src/bitDecode.d \
./Src/fsk_crc16.d \
./Src/main.d \
./Src/microsec_delay.d \
./Src/rs232_menu.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/system_stm32f4xx.d \
./Src/uartPrint.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F446xx -I"C:/Users/min_bao/Documents/Workspace/stm32f4xx_spi_uart/Inc" -I"C:/Users/min_bao/Documents/Workspace/stm32f4xx_spi_uart/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/min_bao/Documents/Workspace/stm32f4xx_spi_uart/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/min_bao/Documents/Workspace/stm32f4xx_spi_uart/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/min_bao/Documents/Workspace/stm32f4xx_spi_uart/Drivers/CMSIS/Include" -I"C:/Users/min_bao/Documents/Workspace/stm32f4xx_spi_uart/Inc"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


