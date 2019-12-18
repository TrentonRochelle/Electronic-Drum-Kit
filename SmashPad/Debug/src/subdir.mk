################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/SmashPad.c \
../src/bsp.c \
../src/cr_startup_lpc407x_8x.c \
../src/crp.c \
../src/fonts.c \
../src/lcd.c \
../src/statemachine.c \
../src/sysinit.c 

OBJS += \
./src/SmashPad.o \
./src/bsp.o \
./src/cr_startup_lpc407x_8x.o \
./src/crp.o \
./src/fonts.o \
./src/lcd.o \
./src/statemachine.o \
./src/sysinit.o 

C_DEPS += \
./src/SmashPad.d \
./src/bsp.d \
./src/cr_startup_lpc407x_8x.d \
./src/crp.d \
./src/fonts.d \
./src/lcd.d \
./src/statemachine.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -D__MULTICORE_NONE -DDEBUG -D__CODE_RED -DCORE_M4 -D__USE_LPCOPEN -D__LPC407X_8X__ -I"C:\Users\Trenton Rochelle\Documents\LPCXpresso_7.3.0_186\workspace\SmashPad\inc" -I"C:\Users\Trenton Rochelle\Documents\LPCXpresso_7.3.0_186\workspace\lpc_board_ea_devkit_4088\inc" -I"C:\Users\Trenton Rochelle\Documents\LPCXpresso_7.3.0_186\workspace\lpc_chip_40xx\inc" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


