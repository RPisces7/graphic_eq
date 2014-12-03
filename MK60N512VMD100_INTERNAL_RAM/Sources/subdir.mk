################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/adc.c" \
"../Sources/dac.c" \
"../Sources/ee_printf.c" \
"../Sources/fcvt.c" \
"../Sources/filters.c" \
"../Sources/gpio.c" \
"../Sources/io.c" \
"../Sources/isr.c" \
"../Sources/lptmr.c" \
"../Sources/main.c" \
"../Sources/mcg.c" \
"../Sources/printf.c" \
"../Sources/timers.c" \
"../Sources/uart.c" \

C_SRCS += \
../Sources/adc.c \
../Sources/dac.c \
../Sources/ee_printf.c \
../Sources/fcvt.c \
../Sources/filters.c \
../Sources/gpio.c \
../Sources/io.c \
../Sources/isr.c \
../Sources/lptmr.c \
../Sources/main.c \
../Sources/mcg.c \
../Sources/printf.c \
../Sources/timers.c \
../Sources/uart.c \

OBJS += \
./Sources/adc.obj \
./Sources/dac.obj \
./Sources/ee_printf.obj \
./Sources/fcvt.obj \
./Sources/filters.obj \
./Sources/gpio.obj \
./Sources/io.obj \
./Sources/isr.obj \
./Sources/lptmr.obj \
./Sources/main.obj \
./Sources/mcg.obj \
./Sources/printf.obj \
./Sources/timers.obj \
./Sources/uart.obj \

C_DEPS += \
./Sources/adc.d \
./Sources/dac.d \
./Sources/ee_printf.d \
./Sources/fcvt.d \
./Sources/filters.d \
./Sources/gpio.d \
./Sources/io.d \
./Sources/isr.d \
./Sources/lptmr.d \
./Sources/main.d \
./Sources/mcg.d \
./Sources/printf.d \
./Sources/timers.d \
./Sources/uart.d \

OBJS_QUOTED += \
"./Sources/adc.obj" \
"./Sources/dac.obj" \
"./Sources/ee_printf.obj" \
"./Sources/fcvt.obj" \
"./Sources/filters.obj" \
"./Sources/gpio.obj" \
"./Sources/io.obj" \
"./Sources/isr.obj" \
"./Sources/lptmr.obj" \
"./Sources/main.obj" \
"./Sources/mcg.obj" \
"./Sources/printf.obj" \
"./Sources/timers.obj" \
"./Sources/uart.obj" \

OBJS_OS_FORMAT += \
./Sources/adc.obj \
./Sources/dac.obj \
./Sources/ee_printf.obj \
./Sources/fcvt.obj \
./Sources/filters.obj \
./Sources/gpio.obj \
./Sources/io.obj \
./Sources/isr.obj \
./Sources/lptmr.obj \
./Sources/main.obj \
./Sources/mcg.obj \
./Sources/printf.obj \
./Sources/timers.obj \
./Sources/uart.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/adc.obj: ../Sources/adc.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/adc.args" -o "Sources/adc.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/%.d: ../Sources/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/dac.obj: ../Sources/dac.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/dac.args" -o "Sources/dac.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/ee_printf.obj: ../Sources/ee_printf.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/ee_printf.args" -o "Sources/ee_printf.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/fcvt.obj: ../Sources/fcvt.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/fcvt.args" -o "Sources/fcvt.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/filters.obj: ../Sources/filters.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/filters.args" -o "Sources/filters.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/gpio.obj: ../Sources/gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/gpio.args" -o "Sources/gpio.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/io.obj: ../Sources/io.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/io.args" -o "Sources/io.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/isr.obj: ../Sources/isr.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/isr.args" -o "Sources/isr.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/lptmr.obj: ../Sources/lptmr.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/lptmr.args" -o "Sources/lptmr.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.obj: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/main.args" -o "Sources/main.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/mcg.obj: ../Sources/mcg.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/mcg.args" -o "Sources/mcg.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/printf.obj: ../Sources/printf.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/printf.args" -o "Sources/printf.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/timers.obj: ../Sources/timers.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/timers.args" -o "Sources/timers.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/uart.obj: ../Sources/uart.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/uart.args" -o "Sources/uart.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


