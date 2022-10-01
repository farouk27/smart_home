################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../adc.c \
../buzzer.c \
../gpio.c \
../keypad.c \
../lcd.c \
../servo.c \
../smartHome.c \
../timer0.c \
../timer2.c \
../uart.c 

OBJS += \
./adc.o \
./buzzer.o \
./gpio.o \
./keypad.o \
./lcd.o \
./servo.o \
./smartHome.o \
./timer0.o \
./timer2.o \
./uart.o 

C_DEPS += \
./adc.d \
./buzzer.d \
./gpio.d \
./keypad.d \
./lcd.d \
./servo.d \
./smartHome.d \
./timer0.d \
./timer2.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


