#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx.h"

// GPIO function
// ==============

void GPIO_writePIN(GPIO_TypeDef *GPIOx, uint16_t pin, uint8_t state);
uint8_t GPIO_readPIN(GPIO_TypeDef *GPIOx, uint8_t pin);
void GPIO_Config(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t mode, uint8_t pupd, uint8_t speed);

#endif // GPIO_H
