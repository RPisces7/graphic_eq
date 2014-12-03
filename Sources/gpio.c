/*
 * gpio.c
 *
 *  Created on: Nov 21, 2014
 *      Author: ral9602
 */
#include "gpio.h"

void GPIO_INIT(void) {
    // Clock on for leds
    SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;
 
    // Setup GPIO LEDS
    PORTA_PCR11 = PORT_PCR_MUX(1);
    PORTA_PCR28 = PORT_PCR_MUX(1);
    PORTA_PCR29 = PORT_PCR_MUX(1);
    PORTA_PCR10 = PORT_PCR_MUX(1);
    PORTB_PCR9 = PORT_PCR_MUX(1);		// GPIO3, TWR B23
    
    // Setup outputs drive mode
    GPIOA_PDDR = ( GPIO_PIN(LED_ORANGE) | GPIO_PIN(LED_YELLOW) | GPIO_PIN(LED_GREEN) | GPIO_PIN(LED_BLUE) );
    GPIOB_PDDR = GPIO_PIN(9);
    
    GPIOA_PDOR &= ~( GPIO_PIN(LED_ORANGE) | GPIO_PIN(LED_YELLOW) | GPIO_PIN(LED_GREEN) | GPIO_PIN(LED_BLUE) );
    GPIOB_PDOR |= GPIO_PIN(9);
}

