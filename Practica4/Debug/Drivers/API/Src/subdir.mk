################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/Src/API_debounce.c \
../Drivers/API/Src/API_delay.c 

OBJS += \
./Drivers/API/Src/API_debounce.o \
./Drivers/API/Src/API_delay.o 

C_DEPS += \
./Drivers/API/Src/API_debounce.d \
./Drivers/API/Src/API_delay.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/Src/%.o: ../Drivers/API/Src/%.c Drivers/API/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Inc -I"/home/lautaro/CESE/1_Bimestre/PDM/Repositorios/PdM_workspace/Practica2/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/lautaro/CESE/1_Bimestre/PDM/Repositorios/PdM_workspace/Practica2/Drivers/CMSIS/Include" -I"/home/lautaro/CESE/1_Bimestre/PDM/Repositorios/PdM_workspace/Practica2/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/lautaro/CESE/1_Bimestre/PDM/Repositorios/PdM_workspace/Practica2/Drivers/BSP/STM32F4xx_Nucleo_144" -I"/home/lautaro/CESE/1_Bimestre/PDM/Repositorios/PdM_workspace/Practica2/Drivers/Core/Inc" -I"/home/lautaro/CESE/1_Bimestre/PDM/Repositorios/PdM_workspace/Practica2/Drivers/API/Inc" -I"/home/lautaro/CESE/1_Bimestre/PDM/Repositorios/PdM_workspace/Practica2/Drivers/API/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

