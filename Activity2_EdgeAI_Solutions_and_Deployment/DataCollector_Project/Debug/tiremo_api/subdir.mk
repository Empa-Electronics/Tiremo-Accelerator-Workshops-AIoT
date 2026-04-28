################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tiremo_api/tiremo_api.c 

OBJS += \
./tiremo_api/tiremo_api.o 

C_DEPS += \
./tiremo_api/tiremo_api.d 


# Each subdirectory must supply rules for building sources it contributes
tiremo_api/%.o tiremo_api/%.su tiremo_api/%.cyclo: ../tiremo_api/%.c tiremo_api/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U575xx -c -I../Core/Inc -I../X-CUBE-ISPU/Target -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/ism330is -I"/Users/mertyilmaz/CODEWORKS/WORKSHOPS/Accelerator-Workshops-University/Activity2_NEAIS_for_EdgeAI_Solutions_and_Deployment/Project_DataCollector/DataCollector_Project/tiremo_api" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-tiremo_api

clean-tiremo_api:
	-$(RM) ./tiremo_api/tiremo_api.cyclo ./tiremo_api/tiremo_api.d ./tiremo_api/tiremo_api.o ./tiremo_api/tiremo_api.su

.PHONY: clean-tiremo_api

