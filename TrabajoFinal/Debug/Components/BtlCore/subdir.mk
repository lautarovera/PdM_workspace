################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Components/BtlCore/BtlCore.c 

OBJS += \
./Components/BtlCore/BtlCore.o 

C_DEPS += \
./Components/BtlCore/BtlCore.d 


# Each subdirectory must supply rules for building sources it contributes
Components/BtlCore/%.o: ../Components/BtlCore/%.c Components/BtlCore/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Inc -I"/home/lautaro/CESE/1_Bimestre/PDM/Repositorios/PdM_workspace/TrabajoFinal/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/lautaro/CESE/1_Bimestre/PDM/Repositorios/PdM_workspace/TrabajoFinal/Drivers/CMSIS/Include" -I"/home/lautaro/CESE/1_Bimestre/PDM/Repositorios/PdM_workspace/TrabajoFinal/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/lautaro/CESE/1_Bimestre/PDM/Repositorios/PdM_workspace/TrabajoFinal/Drivers/BSP/STM32F4xx_Nucleo_144" -I"/home/lautaro/CESE/1_Bimestre/PDM/Repositorios/PdM_workspace/TrabajoFinal/Drivers/Core/Inc" -I"/home/lautaro/CESE/1_Bimestre/PDM/Repositorios/PdM_workspace/TrabajoFinal/Drivers/API/Inc" -I"/home/lautaro/CESE/1_Bimestre/PDM/Repositorios/PdM_workspace/TrabajoFinal/Drivers/API/Src" -I"/home/lautaro/CESE/1_Bimestre/PDM/Repositorios/PdM_workspace/TrabajoFinal/Components/BtlCore" -I"/home/lautaro/CESE/1_Bimestre/PDM/Repositorios/PdM_workspace/TrabajoFinal/Components/CmdMng" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

