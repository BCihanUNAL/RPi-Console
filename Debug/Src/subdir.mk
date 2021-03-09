################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/client_connection.c \
../Src/main.c \
../Src/server_connection.c 

OBJS += \
./Src/client_connection.o \
./Src/main.o \
./Src/server_connection.o 

C_DEPS += \
./Src/client_connection.d \
./Src/main.d \
./Src/server_connection.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-linux-gnueabihf-gcc -mcpu=cortex-a53 -march=armv8-a -mlittle-endian -mfloat-abi=hard -mfpu=vfpv4 -munaligned-access -pthread -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


