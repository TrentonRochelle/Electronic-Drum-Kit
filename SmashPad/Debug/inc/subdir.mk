################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../inc/qepn.c \
../inc/qfn.c \
../inc/qfn_init.c 

OBJS += \
./inc/qepn.o \
./inc/qfn.o \
./inc/qfn_init.o 

C_DEPS += \
./inc/qepn.d \
./inc/qfn.d \
./inc/qfn_init.d 


# Each subdirectory must supply rules for building sources it contributes
inc/%.o: ../inc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -D__MULTICORE_NONE -DDEBUG -D__CODE_RED -DCORE_M4 -D__USE_LPCOPEN -D__LPC407X_8X__ -I"C:\Users\Trenton Rochelle\Documents\LPCXpresso_7.3.0_186\workspace\SmashPad\inc" -I"C:\Users\Trenton Rochelle\Documents\LPCXpresso_7.3.0_186\workspace\lpc_board_ea_devkit_4088\inc" -I"C:\Users\Trenton Rochelle\Documents\LPCXpresso_7.3.0_186\workspace\lpc_chip_40xx\inc" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


