################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/tsc2046/d4dtch_TSC2046.c" \

C_SRCS += \
../Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/tsc2046/d4dtch_TSC2046.c \

OBJS += \
./Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/tsc2046/d4dtch_TSC2046.obj \

C_DEPS += \
./Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/tsc2046/d4dtch_TSC2046.d \

OBJS_QUOTED += \
"./Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/tsc2046/d4dtch_TSC2046.obj" \

OBJS_OS_FORMAT += \
./Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/tsc2046/d4dtch_TSC2046.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/tsc2046/d4dtch_TSC2046.obj: ../Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/tsc2046/d4dtch_TSC2046.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/tsc2046/d4dtch_TSC2046.args" -o "Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/tsc2046/d4dtch_TSC2046.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/tsc2046/%.d: ../Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/tsc2046/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


