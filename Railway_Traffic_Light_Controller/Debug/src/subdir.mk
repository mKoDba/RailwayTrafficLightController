################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/controller.c \
../src/i2c_comms.c \
../src/main.c \
../src/serial_comms.c 

OBJS += \
./src/controller.o \
./src/i2c_comms.o \
./src/main.o \
./src/serial_comms.o 

C_DEPS += \
./src/controller.d \
./src/i2c_comms.d \
./src/main.d \
./src/serial_comms.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


