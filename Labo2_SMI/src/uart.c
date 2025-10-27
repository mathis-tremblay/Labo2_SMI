#include "GPIO.h"
#include "stm32f4xx.h"
#include "uart.h"

#define SYS_CLOCK_FREQ 72000000UL // fréquence de base


void UART5_Config(){
	// Activer clk : UART5 => APB1
	RCC->APB1ENR |= RCC_APB1ENR_UART5EN;

	// Setup pins (PC12 pour le tx, PD2 pour le rx)
	GPIO_Config(GPIOC, 12, 2, 0, 2, 8);
	GPIO_Config(GPIOD, 2, 2, 0, 2, 8);

	// 1. Désactive UART pendant config
	UART5->CR1 &= ~BIT13;

	// 2. Taille mot (8)
	UART5->CR1 &= ~BIT12;

	// Activer interrupt RXNEIE
	UART5->CR1 |= BIT5;

	// Parité paire
	UART5->CR1 |= BIT10;
	UART5->CR1 &= ~BIT9;

	// 3. Stop bits (1)
	UART5->CR2 &= ~BIT12;
	UART5->CR2 &= ~BIT13;

	// 5. Baudrate de 115200
	UART5->BRR &= 0;
	UART5->BRR |= 0; //TODO

	// 6. activer transmitter et receiver
	UART5->CR1 |= BIT3;
	UART5->CR1 |= BIT2;

	// enable uart
	UART5->CR1 |= BIT13;

	// activer interrupt NVIC
	NVIC_EnableIRQ(UART5_IRQn);
}
