/*
 * gpio.h
 *
 *  Created on: Nov 21, 2014
 *      Author: ral9602
 */
#include "derivative.h" /* include peripheral declarations */

#ifndef GPIO_H_
#define GPIO_H_

#define LED_ORANGE 11
#define LED_YELLOW 28
#define LED_GREEN  29
#define LED_BLUE   10

#define GPIO_PIN(x) (uint32_t)(1 << x)

void GPIO_INIT(void);

#endif /* GPIO_H_ */
