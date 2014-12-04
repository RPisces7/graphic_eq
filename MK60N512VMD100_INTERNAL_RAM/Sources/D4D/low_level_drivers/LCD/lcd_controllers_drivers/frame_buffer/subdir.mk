################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/frame_buffer/d4dlcd_frame_buffer.c" \

C_SRCS += \
../Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/frame_buffer/d4dlcd_frame_buffer.c \

OBJS += \
./Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/frame_buffer/d4dlcd_frame_buffer.obj \

C_DEPS += \
./Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/frame_buffer/d4dlcd_frame_buffer.d \

OBJS_QUOTED += \
"./Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/frame_buffer/d4dlcd_frame_buffer.obj" \

OBJS_OS_FORMAT += \
./Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/frame_buffer/d4dlcd_frame_buffer.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/frame_buffer/d4dlcd_frame_buffer.obj: ../Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/frame_buffer/d4dlcd_frame_buffer.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/frame_buffer/d4dlcd_frame_buffer.args" -o "Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/frame_buffer/d4dlcd_frame_buffer.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/frame_buffer/%.d: ../Sources/D4D/low_level_drivers/LCD/lcd_controllers_drivers/frame_buffer/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


