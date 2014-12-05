################################################################################
# Automatically-generated file. Do not edit!
################################################################################

first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := "$(Gnu_Make_Install_DirEnv)/rm" -f

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Sources/subdir.mk
-include Sources/D4D/low_level_drivers/touch_screen/touch_screen_hw_interface/template/subdir.mk
-include Sources/D4D/low_level_drivers/touch_screen/touch_screen_hw_interface/s12_adc_12b/subdir.mk
-include Sources/D4D/low_level_drivers/touch_screen/touch_screen_hw_interface/s08_adc_12b/subdir.mk
-include Sources/D4D/low_level_drivers/touch_screen/touch_screen_hw_interface/mcf52259_adc_12b/subdir.mk
-include Sources/D4D/low_level_drivers/touch_screen/touch_screen_hw_interface/kinetis_adc_12b/subdir.mk
-include Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/tsc2046/subdir.mk
-include Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/template/subdir.mk
-include Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/resistive_mqx/subdir.mk
-include Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/resistive/subdir.mk
-include Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/mcf52277_asp/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template_fb/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/template/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/spi_swc_8bit/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/spi_sw_16bit/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/spi_8bit/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/spi_16bit/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/s12_spi_16b/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/mqx_spi/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/mqx_mpc5125_diu/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/mqx_flexbus/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/kinetis_spi_bm/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/gpio_8080_byte_8bit/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/gpio_8080_8bit/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/gpio_6800_8bit/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/flexbus_8b/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/flexbus_16b/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/dragonfire_lcdc/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/common_drivers/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/template/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/ssd1926/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/ssd1289/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/ls020/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/lgdp4531/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/fsa506/subdir.mk
-include Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/frame_buffer/subdir.mk
-include Sources/D4D/graphic_objects/subdir.mk
-include Sources/D4D/common_files/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include Project_Settings/Linker_Files/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
graphic_eq.afx \

EXECUTABLES_QUOTED += \
"graphic_eq.afx" \

EXECUTABLES_OS_FORMAT += \
graphic_eq.afx \


# All Target
all: graphic_eq.afx

# Tool invocations
graphic_eq.afx: $(OBJS) $(LCF_BUILD_DEPENCENCY) $(USER_OBJS) $(LCF_SRCS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"$(ARM_ToolsDirEnv)/mwldarm" @@"graphic_eq.args" -o "graphic_eq.afx"  $(USER_OBJS_QUOTED)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CXX_DEPS_QUOTED)$(OBJS)$(CXX_DEPS_OS_FORMAT)$(C_DEPS)$(CC_DEPS_OS_FORMAT)$(C_UPPER_DEPS_OS_FORMAT)$(EXECUTABLES_OS_FORMAT)$(CXX_DEPS)$(C++_DEPS)$(C_UPPER_DEPS_QUOTED)$(CC_DEPS)$(CPP_DEPS_OS_FORMAT)$(OBJS_QUOTED)$(CC_DEPS_QUOTED)$(CPP_DEPS_QUOTED)$(C++_DEPS_QUOTED)$(C_DEPS_OS_FORMAT)$(EXECUTABLES_QUOTED)$(CPP_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(OBJS_OS_FORMAT)$(C++_DEPS_OS_FORMAT)$(C_DEPS_QUOTED) graphic_eq.afx
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
