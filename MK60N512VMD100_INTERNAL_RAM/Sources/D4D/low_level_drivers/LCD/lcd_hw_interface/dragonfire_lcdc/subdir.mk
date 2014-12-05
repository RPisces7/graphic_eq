################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/dragonfire_lcdc/d4dlcdhw_dragonfire_lcdc.c" \

C_SRCS += \
../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/dragonfire_lcdc/d4dlcdhw_dragonfire_lcdc.c \

OBJS += \
./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/dragonfire_lcdc/d4dlcdhw_dragonfire_lcdc.obj \

C_DEPS += \
./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/dragonfire_lcdc/d4dlcdhw_dragonfire_lcdc.d \

OBJS_QUOTED += \
"./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/dragonfire_lcdc/d4dlcdhw_dragonfire_lcdc.obj" \

OBJS_OS_FORMAT += \
./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/dragonfire_lcdc/d4dlcdhw_dragonfire_lcdc.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/dragonfire_lcdc/d4dlcdhw_dragonfire_lcdc.obj: ../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/dragonfire_lcdc/d4dlcdhw_dragonfire_lcdc.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/dragonfire_lcdc/d4dlcdhw_dragonfire_lcdc.args" -o "Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/dragonfire_lcdc/d4dlcdhw_dragonfire_lcdc.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/dragonfire_lcdc/%.d: ../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/dragonfire_lcdc/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


