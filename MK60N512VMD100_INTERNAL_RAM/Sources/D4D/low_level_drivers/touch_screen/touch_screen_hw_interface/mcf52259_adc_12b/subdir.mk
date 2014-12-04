################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/D4D/low_level_drivers/touch_screen/touch_screen_hw_interface/mcf52259_adc_12b/d4dtchhw_mcf52259_adc.c" \

C_SRCS += \
../Sources/D4D/low_level_drivers/touch_screen/touch_screen_hw_interface/mcf52259_adc_12b/d4dtchhw_mcf52259_adc.c \

OBJS += \
./Sources/D4D/low_level_drivers/touch_screen/touch_screen_hw_interface/mcf52259_adc_12b/d4dtchhw_mcf52259_adc.obj \

C_DEPS += \
./Sources/D4D/low_level_drivers/touch_screen/touch_screen_hw_interface/mcf52259_adc_12b/d4dtchhw_mcf52259_adc.d \

OBJS_QUOTED += \
"./Sources/D4D/low_level_drivers/touch_screen/touch_screen_hw_interface/mcf52259_adc_12b/d4dtchhw_mcf52259_adc.obj" \

OBJS_OS_FORMAT += \
./Sources/D4D/low_level_drivers/touch_screen/touch_screen_hw_interface/mcf52259_adc_12b/d4dtchhw_mcf52259_adc.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/D4D/low_level_drivers/touch_screen/touch_screen_hw_interface/mcf52259_adc_12b/d4dtchhw_mcf52259_adc.obj: ../Sources/D4D/low_level_drivers/touch_screen/touch_screen_hw_interface/mcf52259_adc_12b/d4dtchhw_mcf52259_adc.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/low_level_drivers/touch_screen/touch_screen_hw_interface/mcf52259_adc_12b/d4dtchhw_mcf52259_adc.args" -o "Sources/D4D/low_level_drivers/touch_screen/touch_screen_hw_interface/mcf52259_adc_12b/d4dtchhw_mcf52259_adc.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/low_level_drivers/touch_screen/touch_screen_hw_interface/mcf52259_adc_12b/%.d: ../Sources/D4D/low_level_drivers/touch_screen/touch_screen_hw_interface/mcf52259_adc_12b/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


