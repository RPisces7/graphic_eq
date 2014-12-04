/*****************************************************************************
 * (c) Copyright 2010, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      sim.h
 * @author    R55013
 * @version   1.0.1.0
 * @date      Mar-4-2011
 * @brief     SIM driver header file.
 ******************************************************************************/
#ifndef __SIM_H
#define __SIM_H

/******************************************************************************
 * definition configuration structure                                         *
 ******************************************************************************/
typedef struct
{
  uint32 SOPT1, SOPT2, SOPT4, SOPT5, SOPT6, SOPT7, SCGC1, SCGC2, SCGC3, SCGC4,
         SCGC5, SCGC6, SCGC7, CLKDIV1, CLKDIV2;
} tSIM;

#define SIM_MODULE_CONFIG_LCD   /* ports clocked                  */  \
(tSIM){                                                                       \
/* SOPT1   */ 0l,                                                             \
/* SOPT2   */ 0l,                                                             \
/* SOPT4   */ 0l,                                                             \
/* SOPT5   */ 0l,                                                             \
/* SOPT6   */ 0l,                                                             \
/* SOPT7   */ 0l,                                                             \
/* SCGC1   */ 0l,                                                             \
/* SCGC2   */ 0l,                                                             \
/* SCGC3   */ SIM_SCGC3 |= SIM_SCGC3_SPI2_MASK,                               \
/* SCGC4   */ 0l,                                                             \
/* SCGC5   */ SIM_SCGC5_PORTA_MASK|SIM_SCGC5_PORTD_MASK|SIM_SCGC5_PORTE_MASK, \
/* SCGC6   */ SIM_SCGC6_PIT_MASK,                          \
/* SCGC7   */ 0l,                                                             \
/* CLKDIV1 */ SIM_CLKDIV1_OUTDIV1(0)|SIM_CLKDIV1_OUTDIV2(0)|                  \
              SIM_CLKDIV1_OUTDIV3(0)|SIM_CLKDIV1_OUTDIV4(0),                  \
/* CLKDIV2 */ SIM_CLKDIV2_I2SDIV(1)|SIM_CLKDIV2_I2SFRAC(0)|                   \
              SIM_CLKDIV2_USBDIV(0)|SIM_CLKDIV2_USBFRAC_MASK,                 \
}

/******************************************************************************
 * default configurations                                                     *
 ******************************************************************************/
#if (defined(CPU_MK60N512VMD100))
#define SIM_MODULE_CONFIG   /* all peripherals clocks on                  */  \
(tSIM){                                                                       \
/* SOPT1   */ SIM_SOPT1_USBREGEN_MASK|SIM_SOPT1_USBSTBY_MASK,                 \
/* SOPT2   */ SIM_SOPT2_TRACECLKSEL_MASK,                                     \
/* SOPT4   */ 0l,                                                             \
/* SOPT5   */ 0l,                                                             \
/* SOPT6   */ 0l,                                                             \
/* SOPT7   */ 0l,                                                             \
/* SCGC1   */ SIM_SCGC1_UART4_MASK|SIM_SCGC1_UART5_MASK,                      \
/* SCGC2   */ SIM_SCGC2_ENET_MASK|SIM_SCGC2_DAC0_MASK|SIM_SCGC2_DAC1_MASK,    \
/* SCGC3   */ SIM_SCGC3_SDHC_MASK|SIM_SCGC3_FTM2_MASK|SIM_SCGC3_ADC1_MASK|    \
              SIM_SCGC3_RNGB_MASK|SIM_SCGC3_FLEXCAN1_MASK|SIM_SCGC3_SPI2_MASK,\
/* SCGC4   */ SIM_SCGC4_USBOTG_MASK|SIM_SCGC4_CMP_MASK|SIM_SCGC4_VREF_MASK|   \
              SIM_SCGC4_LLWU_MASK|SIM_SCGC4_EWM_MASK|SIM_SCGC4_CMT_MASK|      \
              SIM_SCGC4_I2C0_MASK|SIM_SCGC4_I2C1_MASK|SIM_SCGC4_UART0_MASK|   \
              SIM_SCGC4_UART1_MASK|SIM_SCGC4_UART2_MASK|SIM_SCGC4_UART3_MASK, \
/* SCGC5   */ SIM_SCGC5_LPTIMER_MASK|SIM_SCGC5_REGFILE_MASK|                  \
              SIM_SCGC5_TSI_MASK|SIM_SCGC5_PORTA_MASK|SIM_SCGC5_PORTB_MASK|   \
              SIM_SCGC5_PORTC_MASK|SIM_SCGC5_PORTD_MASK|SIM_SCGC5_PORTE_MASK, \
/* SCGC6   */ SIM_SCGC6_CRC_MASK|SIM_SCGC6_USBDCD_MASK|SIM_SCGC6_PIT_MASK|    \
              SIM_SCGC6_PDB_MASK|SIM_SCGC6_FTM0_MASK|SIM_SCGC6_FTM1_MASK|     \
              SIM_SCGC6_ADC0_MASK|SIM_SCGC6_RTC_MASK|SIM_SCGC6_FTFL_MASK|     \
              SIM_SCGC6_DMAMUX_MASK|SIM_SCGC6_FLEXCAN0_MASK|                  \
              SIM_SCGC6_DSPI0_MASK|SIM_SCGC6_SPI1_MASK|SIM_SCGC6_I2S_MASK,    \
/* SCGC7   */ SIM_SCGC7_FLEXBUS_MASK|SIM_SCGC7_DMA_MASK|SIM_SCGC7_MPU_MASK,   \
/* CLKDIV1 */ SIM_CLKDIV1_OUTDIV1(0)|SIM_CLKDIV1_OUTDIV2(0)|                  \
              SIM_CLKDIV1_OUTDIV3(0)|SIM_CLKDIV1_OUTDIV4(0),                  \
/* CLKDIV2 */ SIM_CLKDIV2_I2SDIV(1)|SIM_CLKDIV2_I2SFRAC(0)|                   \
              SIM_CLKDIV2_USBDIV(0)|SIM_CLKDIV2_USBFRAC_MASK,                 \
}
#endif

#if (defined(CPU_MK40N512VMD100))
#define SIM_MODULE_CONFIG   /* all peripherals clocks on                  */  \
(tSIM){                                                                       \
/* SOPT1   */ SIM_SOPT1_USBREGEN_MASK|SIM_SOPT1_USBSTBY_MASK,                 \
/* SOPT2   */ SIM_SOPT2_TRACECLKSEL_MASK,                                     \
/* SOPT4   */ 0l,                                                             \
/* SOPT5   */ 0l,                                                             \
/* SOPT6   */ 0l,                                                             \
/* SOPT7   */ 0l,                                                             \
/* SCGC1   */ SIM_SCGC1_UART4_MASK|SIM_SCGC1_UART5_MASK,                      \
/* SCGC2   */ SIM_SCGC2_DAC0_MASK|SIM_SCGC2_DAC1_MASK,                        \
/* SCGC3   */ SIM_SCGC3_SDHC_MASK|SIM_SCGC3_FTM2_MASK|SIM_SCGC3_ADC1_MASK|    \
              SIM_SCGC3_SLCD_MASK|SIM_SCGC3_FLEXCAN1_MASK|SIM_SCGC3_SPI2_MASK,\
/* SCGC4   */ SIM_SCGC4_USBOTG_MASK|SIM_SCGC4_CMP_MASK|SIM_SCGC4_VREF_MASK|   \
              SIM_SCGC4_LLWU_MASK|SIM_SCGC4_EWM_MASK|SIM_SCGC4_CMT_MASK|      \
              SIM_SCGC4_I2C0_MASK|SIM_SCGC4_I2C1_MASK|SIM_SCGC4_UART0_MASK|   \
              SIM_SCGC4_UART1_MASK|SIM_SCGC4_UART2_MASK|SIM_SCGC4_UART3_MASK, \
/* SCGC5   */ SIM_SCGC5_LPTIMER_MASK|SIM_SCGC5_REGFILE_MASK|                  \
              SIM_SCGC5_TSI_MASK|SIM_SCGC5_PORTA_MASK|SIM_SCGC5_PORTB_MASK|   \
              SIM_SCGC5_PORTC_MASK|SIM_SCGC5_PORTD_MASK|SIM_SCGC5_PORTE_MASK, \
/* SCGC6   */ SIM_SCGC6_CRC_MASK|SIM_SCGC6_USBDCD_MASK|SIM_SCGC6_PIT_MASK|    \
              SIM_SCGC6_PDB_MASK|SIM_SCGC6_FTM0_MASK|SIM_SCGC6_FTM1_MASK|     \
              SIM_SCGC6_ADC0_MASK|SIM_SCGC6_RTC_MASK|SIM_SCGC6_FTFL_MASK|     \
              SIM_SCGC6_DMAMUX_MASK|SIM_SCGC6_FLEXCAN0_MASK|                  \
              SIM_SCGC6_DSPI0_MASK|SIM_SCGC6_SPI1_MASK|SIM_SCGC6_I2S_MASK,    \
/* SCGC7   */ SIM_SCGC7_FLEXBUS_MASK|SIM_SCGC7_DMA_MASK|SIM_SCGC7_MPU_MASK,   \
/* CLKDIV1 */ SIM_CLKDIV1_OUTDIV1(0)|SIM_CLKDIV1_OUTDIV2(0)|                  \
              SIM_CLKDIV1_OUTDIV3(0)|SIM_CLKDIV1_OUTDIV4(0),                  \
/* CLKDIV2 */ SIM_CLKDIV2_I2SDIV(1)|SIM_CLKDIV2_I2SFRAC(0)|                   \
              SIM_CLKDIV2_USBDIV(0)|SIM_CLKDIV2_USBFRAC_MASK,                 \
}
#endif

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/

/******************************************************************************
 * command definitions                                                        *
 ******************************************************************************/

/******************************************************************************
 * public function prototypes                                                 *
 ******************************************************************************/
extern void SIM_Init (tSIM volatile sim);

#endif /* __SIM_H */
