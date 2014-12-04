################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/D4D/graphic_objects/d4d_button.c" \
"../Sources/D4D/graphic_objects/d4d_check_box.c" \
"../Sources/D4D/graphic_objects/d4d_console.c" \
"../Sources/D4D/graphic_objects/d4d_gauge.c" \
"../Sources/D4D/graphic_objects/d4d_graph.c" \
"../Sources/D4D/graphic_objects/d4d_icon.c" \
"../Sources/D4D/graphic_objects/d4d_label.c" \
"../Sources/D4D/graphic_objects/d4d_menu.c" \
"../Sources/D4D/graphic_objects/d4d_picture.c" \
"../Sources/D4D/graphic_objects/d4d_progress_bar.c" \
"../Sources/D4D/graphic_objects/d4d_scroll_bar.c" \
"../Sources/D4D/graphic_objects/d4d_slider.c" \
"../Sources/D4D/graphic_objects/d4d_text_box.c" \

C_SRCS += \
../Sources/D4D/graphic_objects/d4d_button.c \
../Sources/D4D/graphic_objects/d4d_check_box.c \
../Sources/D4D/graphic_objects/d4d_console.c \
../Sources/D4D/graphic_objects/d4d_gauge.c \
../Sources/D4D/graphic_objects/d4d_graph.c \
../Sources/D4D/graphic_objects/d4d_icon.c \
../Sources/D4D/graphic_objects/d4d_label.c \
../Sources/D4D/graphic_objects/d4d_menu.c \
../Sources/D4D/graphic_objects/d4d_picture.c \
../Sources/D4D/graphic_objects/d4d_progress_bar.c \
../Sources/D4D/graphic_objects/d4d_scroll_bar.c \
../Sources/D4D/graphic_objects/d4d_slider.c \
../Sources/D4D/graphic_objects/d4d_text_box.c \

OBJS += \
./Sources/D4D/graphic_objects/d4d_button.obj \
./Sources/D4D/graphic_objects/d4d_check_box.obj \
./Sources/D4D/graphic_objects/d4d_console.obj \
./Sources/D4D/graphic_objects/d4d_gauge.obj \
./Sources/D4D/graphic_objects/d4d_graph.obj \
./Sources/D4D/graphic_objects/d4d_icon.obj \
./Sources/D4D/graphic_objects/d4d_label.obj \
./Sources/D4D/graphic_objects/d4d_menu.obj \
./Sources/D4D/graphic_objects/d4d_picture.obj \
./Sources/D4D/graphic_objects/d4d_progress_bar.obj \
./Sources/D4D/graphic_objects/d4d_scroll_bar.obj \
./Sources/D4D/graphic_objects/d4d_slider.obj \
./Sources/D4D/graphic_objects/d4d_text_box.obj \

C_DEPS += \
./Sources/D4D/graphic_objects/d4d_button.d \
./Sources/D4D/graphic_objects/d4d_check_box.d \
./Sources/D4D/graphic_objects/d4d_console.d \
./Sources/D4D/graphic_objects/d4d_gauge.d \
./Sources/D4D/graphic_objects/d4d_graph.d \
./Sources/D4D/graphic_objects/d4d_icon.d \
./Sources/D4D/graphic_objects/d4d_label.d \
./Sources/D4D/graphic_objects/d4d_menu.d \
./Sources/D4D/graphic_objects/d4d_picture.d \
./Sources/D4D/graphic_objects/d4d_progress_bar.d \
./Sources/D4D/graphic_objects/d4d_scroll_bar.d \
./Sources/D4D/graphic_objects/d4d_slider.d \
./Sources/D4D/graphic_objects/d4d_text_box.d \

OBJS_QUOTED += \
"./Sources/D4D/graphic_objects/d4d_button.obj" \
"./Sources/D4D/graphic_objects/d4d_check_box.obj" \
"./Sources/D4D/graphic_objects/d4d_console.obj" \
"./Sources/D4D/graphic_objects/d4d_gauge.obj" \
"./Sources/D4D/graphic_objects/d4d_graph.obj" \
"./Sources/D4D/graphic_objects/d4d_icon.obj" \
"./Sources/D4D/graphic_objects/d4d_label.obj" \
"./Sources/D4D/graphic_objects/d4d_menu.obj" \
"./Sources/D4D/graphic_objects/d4d_picture.obj" \
"./Sources/D4D/graphic_objects/d4d_progress_bar.obj" \
"./Sources/D4D/graphic_objects/d4d_scroll_bar.obj" \
"./Sources/D4D/graphic_objects/d4d_slider.obj" \
"./Sources/D4D/graphic_objects/d4d_text_box.obj" \

OBJS_OS_FORMAT += \
./Sources/D4D/graphic_objects/d4d_button.obj \
./Sources/D4D/graphic_objects/d4d_check_box.obj \
./Sources/D4D/graphic_objects/d4d_console.obj \
./Sources/D4D/graphic_objects/d4d_gauge.obj \
./Sources/D4D/graphic_objects/d4d_graph.obj \
./Sources/D4D/graphic_objects/d4d_icon.obj \
./Sources/D4D/graphic_objects/d4d_label.obj \
./Sources/D4D/graphic_objects/d4d_menu.obj \
./Sources/D4D/graphic_objects/d4d_picture.obj \
./Sources/D4D/graphic_objects/d4d_progress_bar.obj \
./Sources/D4D/graphic_objects/d4d_scroll_bar.obj \
./Sources/D4D/graphic_objects/d4d_slider.obj \
./Sources/D4D/graphic_objects/d4d_text_box.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/D4D/graphic_objects/d4d_button.obj: ../Sources/D4D/graphic_objects/d4d_button.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/graphic_objects/d4d_button.args" -o "Sources/D4D/graphic_objects/d4d_button.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/graphic_objects/%.d: ../Sources/D4D/graphic_objects/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/D4D/graphic_objects/d4d_check_box.obj: ../Sources/D4D/graphic_objects/d4d_check_box.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/graphic_objects/d4d_check_box.args" -o "Sources/D4D/graphic_objects/d4d_check_box.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/graphic_objects/d4d_console.obj: ../Sources/D4D/graphic_objects/d4d_console.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/graphic_objects/d4d_console.args" -o "Sources/D4D/graphic_objects/d4d_console.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/graphic_objects/d4d_gauge.obj: ../Sources/D4D/graphic_objects/d4d_gauge.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/graphic_objects/d4d_gauge.args" -o "Sources/D4D/graphic_objects/d4d_gauge.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/graphic_objects/d4d_graph.obj: ../Sources/D4D/graphic_objects/d4d_graph.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/graphic_objects/d4d_graph.args" -o "Sources/D4D/graphic_objects/d4d_graph.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/graphic_objects/d4d_icon.obj: ../Sources/D4D/graphic_objects/d4d_icon.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/graphic_objects/d4d_icon.args" -o "Sources/D4D/graphic_objects/d4d_icon.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/graphic_objects/d4d_label.obj: ../Sources/D4D/graphic_objects/d4d_label.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/graphic_objects/d4d_label.args" -o "Sources/D4D/graphic_objects/d4d_label.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/graphic_objects/d4d_menu.obj: ../Sources/D4D/graphic_objects/d4d_menu.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/graphic_objects/d4d_menu.args" -o "Sources/D4D/graphic_objects/d4d_menu.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/graphic_objects/d4d_picture.obj: ../Sources/D4D/graphic_objects/d4d_picture.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/graphic_objects/d4d_picture.args" -o "Sources/D4D/graphic_objects/d4d_picture.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/graphic_objects/d4d_progress_bar.obj: ../Sources/D4D/graphic_objects/d4d_progress_bar.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/graphic_objects/d4d_progress_bar.args" -o "Sources/D4D/graphic_objects/d4d_progress_bar.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/graphic_objects/d4d_scroll_bar.obj: ../Sources/D4D/graphic_objects/d4d_scroll_bar.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/graphic_objects/d4d_scroll_bar.args" -o "Sources/D4D/graphic_objects/d4d_scroll_bar.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/graphic_objects/d4d_slider.obj: ../Sources/D4D/graphic_objects/d4d_slider.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/graphic_objects/d4d_slider.args" -o "Sources/D4D/graphic_objects/d4d_slider.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/graphic_objects/d4d_text_box.obj: ../Sources/D4D/graphic_objects/d4d_text_box.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/graphic_objects/d4d_text_box.args" -o "Sources/D4D/graphic_objects/d4d_text_box.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


