#include "spi.h"
#include "stm32f4xx.h"   // Def des registres STM32F429

void SPI_Init(void) {
	// Activer le port F
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;

	// Activer le peripherique SPI5 (bus APB2)
	RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;
}

