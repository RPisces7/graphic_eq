################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/s12_spi_16b/d4dlcdhw_s12_spi_16b.c" \

C_SRCS += \
../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/s12_spi_16b/d4dlcdhw_s12_spi_16b.c \

OBJS += \
./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/s12_spi_16b/d4dlcdhw_s12_spi_16b.obj \

C_DEPS += \
./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/s12_spi_16b/d4dlcdhw_s12_spi_16b.d \

OBJS_QUOTED += \
"./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/s12_spi_16b/d4dlcdhw_s12_spi_16b.obj" \

OBJS_OS_FORMAT += \
./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/s12_spi_16b/d4dlcdhw_s12_spi_16b.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/s12_spi_16b/d4dlcdhw_s12_spi_16b.obj: ../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/s12_spi_16b/d4dlcdhw_s12_spi_16b.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/s12_spi_16b/d4dlcdhw_s12_spi_16b.args" -o "Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/s12_spi_16b/d4dlcdhw_s12_spi_16b.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/s12_spi_16b/%.d: ../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/s12_spi_16b/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


