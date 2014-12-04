################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template_fb/d4dlcdhw_template_fb.c" \

C_SRCS += \
../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template_fb/d4dlcdhw_template_fb.c \

OBJS += \
./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template_fb/d4dlcdhw_template_fb.obj \

C_DEPS += \
./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template_fb/d4dlcdhw_template_fb.d \

OBJS_QUOTED += \
"./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template_fb/d4dlcdhw_template_fb.obj" \

OBJS_OS_FORMAT += \
./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template_fb/d4dlcdhw_template_fb.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template_fb/d4dlcdhw_template_fb.obj: ../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template_fb/d4dlcdhw_template_fb.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template_fb/d4dlcdhw_template_fb.args" -o "Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template_fb/d4dlcdhw_template_fb.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template_fb/%.d: ../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template_fb/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


