################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/resistive/d4dtch_resistive.c" \

C_SRCS += \
../Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/resistive/d4dtch_resistive.c \

OBJS += \
./Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/resistive/d4dtch_resistive.obj \

C_DEPS += \
./Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/resistive/d4dtch_resistive.d \

OBJS_QUOTED += \
"./Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/resistive/d4dtch_resistive.obj" \

OBJS_OS_FORMAT += \
./Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/resistive/d4dtch_resistive.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/resistive/d4dtch_resistive.obj: ../Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/resistive/d4dtch_resistive.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/resistive/d4dtch_resistive.args" -o "Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/resistive/d4dtch_resistive.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/resistive/%.d: ../Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/resistive/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


