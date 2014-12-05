################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template/d4dlcdhw_template.c" \

C_SRCS += \
../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template/d4dlcdhw_template.c \

OBJS += \
./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template/d4dlcdhw_template.obj \

C_DEPS += \
./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template/d4dlcdhw_template.d \

OBJS_QUOTED += \
"./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template/d4dlcdhw_template.obj" \

OBJS_OS_FORMAT += \
./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template/d4dlcdhw_template.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template/d4dlcdhw_template.obj: ../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template/d4dlcdhw_template.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template/d4dlcdhw_template.args" -o "Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template/d4dlcdhw_template.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template/%.d: ../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


