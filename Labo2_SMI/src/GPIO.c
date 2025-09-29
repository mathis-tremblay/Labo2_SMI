#include "GPIO.h"

void GPIO_writePIN(GPIO_TypeDef *GPIOx, uint16_t pin, uint8_t state) {
	/*
	 * ===================
	 * Partie set pin to 1
	 * ===================
	 *
	 * GPIOX : ptr vers l'adresse memoire d'un port
	 * BSRR : Bit Set Reset Register
	 * GPIOx->BSRR : adresse memoire de "Bit Set Reset Register" (BSRR) pour ce port
	 * pin : masque de bit (ex : [HEX] 0x2000 -> [BIN] 0010 0000 0000 0000)
	 * GPIOx->BSRR = (1 << pin) : decale 1 du num du pin
	 *
	 *
	 * ===================
	 * Partie set pin to 0
	 * ===================
	 * GPIOX : ptr vers l'adresse memoire d'un port
	 * BSRR : Bit Set Reset Register
	 * GPIOx->BSRR : adresse memoire de "Bit Set Reset Register" (BSRR) pour ce port
	 * pin  : masque de bit (ex : [HEX] 0x2000 -> [BIN] 0010 0000 0000 0000)
	 * GPIOx->BSRR = (1 << (pin + 16) : decale 1 du num du pin + 16 (toujours a 0)
	 *
	 */
	if (state) {
		// set pin to 1
		GPIOx->BSRRL = (1 << pin);
	}
	else {
		// set pin to 0
		GPIOx->BSRRH = (1 << pin);
	}
}


// mode : 0 = input, 1 = output, 2 = alternate, 3 = analog
// pupd : 0 = no pull, 1 = pull-up, 2 = pull-down
// speed : 0 = low speed, 1 = medium speed, 2 : high speed, 3 : very high speed
void GPIO_Config(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t mode, uint8_t pupd, uint8_t speed) {
    // Activer l'horloge du port concerné (RCC->AHB1ENR)
    if (GPIOx == GPIOA) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    else if (GPIOx == GPIOB) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    else if (GPIOx == GPIOC) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    else if (GPIOx == GPIOD) RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    else if (GPIOx == GPIOE) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    else if (GPIOx == GPIOF) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
    else if (GPIOx == GPIOG) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
    else if (GPIOx == GPIOH) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
    else if (GPIOx == GPIOI) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;

    // Configurer MODER
    GPIOx->MODER &= ~(0b11 << (pin * 2));
    GPIOx->MODER |=  ((mode & 0b11) << (pin * 2));

    // Configurer PUPDR
    GPIOx->PUPDR &= ~(0b11 << (pin * 2));
    GPIOx->PUPDR |=  ((pupd & 0b11) << (pin * 2));

    // Configurer OSPEEDR
    GPIOx->OSPEEDR &= ~(0b11 << (pin * 2));
    GPIOx->OSPEEDR |=  ((speed & 0b11) << (pin * 2));
}

uint8_t GPIO_readPIN(GPIO_TypeDef *GPIOx, uint8_t pin) {
	return ((GPIOx->IDR & (1 << pin)) ? 1 : 0);
}
