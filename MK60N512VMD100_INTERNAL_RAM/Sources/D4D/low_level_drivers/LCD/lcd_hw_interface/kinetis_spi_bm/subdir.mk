################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/kinetis_spi_bm/d4dlcdhw_kinetis_spi.c" \

C_SRCS += \
../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/kinetis_spi_bm/d4dlcdhw_kinetis_spi.c \

OBJS += \
./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/kinetis_spi_bm/d4dlcdhw_kinetis_spi.obj \

C_DEPS += \
./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/kinetis_spi_bm/d4dlcdhw_kinetis_spi.d \

OBJS_QUOTED += \
"./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/kinetis_spi_bm/d4dlcdhw_kinetis_spi.obj" \

OBJS_OS_FORMAT += \
./Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/kinetis_spi_bm/d4dlcdhw_kinetis_spi.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/kinetis_spi_bm/d4dlcdhw_kinetis_spi.obj: ../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/kinetis_spi_bm/d4dlcdhw_kinetis_spi.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"$(ARM_ToolsDirEnv)/mwccarm" -gccinc @@"Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/kinetis_spi_bm/d4dlcdhw_kinetis_spi.args" -o "Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/kinetis_spi_bm/d4dlcdhw_kinetis_spi.obj" -c "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/kinetis_spi_bm/%.d: ../Sources/D4D/low_level_drivers/LCD/lcd_hw_interface/kinetis_spi_bm/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


