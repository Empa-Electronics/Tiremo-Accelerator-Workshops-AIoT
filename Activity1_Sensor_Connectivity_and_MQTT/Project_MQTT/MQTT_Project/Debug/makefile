################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32U5xx_HAL_Driver/Src/subdir.mk
-include Drivers/BSP/Components/ism330is/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := WORKSHOP_MQTT
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
WORKSHOP_MQTT.elf \

MAP_FILES += \
WORKSHOP_MQTT.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
WORKSHOP_MQTT.list \

OBJCOPY_BIN += \
WORKSHOP_MQTT.bin \


# All Target
all: main-build

# Main-build Target
main-build: WORKSHOP_MQTT.elf secondary-outputs

# Tool invocations
WORKSHOP_MQTT.elf WORKSHOP_MQTT.map: $(OBJS) $(USER_OBJS) C:\Users\sinan\Desktop\EmpaWork\Empa\ Work\Tiremo-Accelerator-Workshops-AIoT_YTU\Activity1_Sensor_Connectivity_and_MQTT\Project_MQTT\MQTT_Project\STM32U575CGTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "WORKSHOP_MQTT.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m33 -T"C:\Users\sinan\Desktop\EmpaWork\Empa Work\Tiremo-Accelerator-Workshops-AIoT_YTU\Activity1_Sensor_Connectivity_and_MQTT\Project_MQTT\MQTT_Project\STM32U575CGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="WORKSHOP_MQTT.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

WORKSHOP_MQTT.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "WORKSHOP_MQTT.list"
	@echo 'Finished building: $@'
	@echo ' '

WORKSHOP_MQTT.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "WORKSHOP_MQTT.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) WORKSHOP_MQTT.bin WORKSHOP_MQTT.elf WORKSHOP_MQTT.list WORKSHOP_MQTT.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
