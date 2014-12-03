################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Project_Settings/Startup_Code/kinetis_sysinit.c" \

C_SRCS += \
../Project_Settings/Startup_Code/kinetis_sysinit.c \

OBJS += \
./Project_Settings/Startup_Code/kinetis_sysinit.obj \

C_DEPS += \
./Project_Settings/Startup_Code/kinetis_sysinit.d \

OBJS_QUOTED += \
"./Project_Settings/Startup_Code/kinetis_sysinit.obj" \

OBJS_OS_FORMAT += \
./Project_Settings/Startup_Code/kinetis_sysinit.obj \


# Each subdirectory must supply rules for building sources it contributes
Project_Settings/Startup_Code/kinetis_sysinit.obj: ../Project_Settings/Startup_Code/kinetis_sysinit.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Project_Settings/Startup_Code/kinetis_sysinit.args" -o "Project_Settings/Startup_Code/kinetis_sysinit.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Project_Settings/Startup_Code/%.d: ../Project_Settings/Startup_Code/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


