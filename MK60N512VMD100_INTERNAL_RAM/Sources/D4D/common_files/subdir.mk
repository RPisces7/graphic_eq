################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/D4D/common_files/d4d_base.c" \
"../Sources/D4D/common_files/d4d_bmp.c" \
"../Sources/D4D/common_files/d4d_extsrc.c" \
"../Sources/D4D/common_files/d4d_font.c" \
"../Sources/D4D/common_files/d4d_low.c" \
"../Sources/D4D/common_files/d4d_math.c" \
"../Sources/D4D/common_files/d4d_object.c" \
"../Sources/D4D/common_files/d4d_scheme.c" \
"../Sources/D4D/common_files/d4d_screen.c" \

C_SRCS += \
../Sources/D4D/common_files/d4d_base.c \
../Sources/D4D/common_files/d4d_bmp.c \
../Sources/D4D/common_files/d4d_extsrc.c \
../Sources/D4D/common_files/d4d_font.c \
../Sources/D4D/common_files/d4d_low.c \
../Sources/D4D/common_files/d4d_math.c \
../Sources/D4D/common_files/d4d_object.c \
../Sources/D4D/common_files/d4d_scheme.c \
../Sources/D4D/common_files/d4d_screen.c \

OBJS += \
./Sources/D4D/common_files/d4d_base.obj \
./Sources/D4D/common_files/d4d_bmp.obj \
./Sources/D4D/common_files/d4d_extsrc.obj \
./Sources/D4D/common_files/d4d_font.obj \
./Sources/D4D/common_files/d4d_low.obj \
./Sources/D4D/common_files/d4d_math.obj \
./Sources/D4D/common_files/d4d_object.obj \
./Sources/D4D/common_files/d4d_scheme.obj \
./Sources/D4D/common_files/d4d_screen.obj \

C_DEPS += \
./Sources/D4D/common_files/d4d_base.d \
./Sources/D4D/common_files/d4d_bmp.d \
./Sources/D4D/common_files/d4d_extsrc.d \
./Sources/D4D/common_files/d4d_font.d \
./Sources/D4D/common_files/d4d_low.d \
./Sources/D4D/common_files/d4d_math.d \
./Sources/D4D/common_files/d4d_object.d \
./Sources/D4D/common_files/d4d_scheme.d \
./Sources/D4D/common_files/d4d_screen.d \

OBJS_QUOTED += \
"./Sources/D4D/common_files/d4d_base.obj" \
"./Sources/D4D/common_files/d4d_bmp.obj" \
"./Sources/D4D/common_files/d4d_extsrc.obj" \
"./Sources/D4D/common_files/d4d_font.obj" \
"./Sources/D4D/common_files/d4d_low.obj" \
"./Sources/D4D/common_files/d4d_math.obj" \
"./Sources/D4D/common_files/d4d_object.obj" \
"./Sources/D4D/common_files/d4d_scheme.obj" \
"./Sources/D4D/common_files/d4d_screen.obj" \

OBJS_OS_FORMAT += \
./Sources/D4D/common_files/d4d_base.obj \
./Sources/D4D/common_files/d4d_bmp.obj \
./Sources/D4D/common_files/d4d_extsrc.obj \
./Sources/D4D/common_files/d4d_font.obj \
./Sources/D4D/common_files/d4d_low.obj \
./Sources/D4D/common_files/d4d_math.obj \
./Sources/D4D/common_files/d4d_object.obj \
./Sources/D4D/common_files/d4d_scheme.obj \
./Sources/D4D/common_files/d4d_screen.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/D4D/common_files/d4d_base.obj: ../Sources/D4D/common_files/d4d_base.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/common_files/d4d_base.args" -o "Sources/D4D/common_files/d4d_base.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/common_files/%.d: ../Sources/D4D/common_files/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/D4D/common_files/d4d_bmp.obj: ../Sources/D4D/common_files/d4d_bmp.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/common_files/d4d_bmp.args" -o "Sources/D4D/common_files/d4d_bmp.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/common_files/d4d_extsrc.obj: ../Sources/D4D/common_files/d4d_extsrc.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/common_files/d4d_extsrc.args" -o "Sources/D4D/common_files/d4d_extsrc.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/common_files/d4d_font.obj: ../Sources/D4D/common_files/d4d_font.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/common_files/d4d_font.args" -o "Sources/D4D/common_files/d4d_font.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/common_files/d4d_low.obj: ../Sources/D4D/common_files/d4d_low.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/common_files/d4d_low.args" -o "Sources/D4D/common_files/d4d_low.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/common_files/d4d_math.obj: ../Sources/D4D/common_files/d4d_math.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/common_files/d4d_math.args" -o "Sources/D4D/common_files/d4d_math.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/common_files/d4d_object.obj: ../Sources/D4D/common_files/d4d_object.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/common_files/d4d_object.args" -o "Sources/D4D/common_files/d4d_object.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/common_files/d4d_scheme.obj: ../Sources/D4D/common_files/d4d_scheme.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/common_files/d4d_scheme.args" -o "Sources/D4D/common_files/d4d_scheme.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/common_files/d4d_screen.obj: ../Sources/D4D/common_files/d4d_screen.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/common_files/d4d_screen.args" -o "Sources/D4D/common_files/d4d_screen.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


