################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/mcf52277_asp/d4dtch_mcf52277_asp.c" \

C_SRCS += \
../Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/mcf52277_asp/d4dtch_mcf52277_asp.c \

OBJS += \
./Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/mcf52277_asp/d4dtch_mcf52277_asp.obj \

C_DEPS += \
./Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/mcf52277_asp/d4dtch_mcf52277_asp.d \

OBJS_QUOTED += \
"./Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/mcf52277_asp/d4dtch_mcf52277_asp.obj" \

OBJS_OS_FORMAT += \
./Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/mcf52277_asp/d4dtch_mcf52277_asp.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/mcf52277_asp/d4dtch_mcf52277_asp.obj: ../Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/mcf52277_asp/d4dtch_mcf52277_asp.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/mcf52277_asp/d4dtch_mcf52277_asp.args" -o "Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/mcf52277_asp/d4dtch_mcf52277_asp.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/mcf52277_asp/%.d: ../Sources/D4D/low_level_drivers/touch_screen/touch_screen_drivers/mcf52277_asp/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


