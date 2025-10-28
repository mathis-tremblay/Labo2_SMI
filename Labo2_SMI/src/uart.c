#include "GPIO.h"
#include "stm32f4xx.h"
#include "uart.h"
#include "macros_utiles.h"

#define SYS_CLOCK_FREQ 72000000.0f // fréquence de base

// init fifo
static FIFO uart5_rx_fifo = { .tete = 0, .queue = 0 };
static FIFO uart5_tx_fifo = { .tete = 0, .queue = 0 };


int _fifo_pleine(FIFO* f){
	return ((f->tete + 1) % UART5_FIFO_TAILLE) == f->queue;
}

int _fifo_vide(FIFO* f){
	return f->tete == f->queue;
}

void _fifo_push(FIFO* f, uint8_t data){
	if (!_fifo_pleine(f)){
		f->buffer[f->tete] = data;
		f->tete = (f->tete + 1) % UART5_FIFO_TAILLE;
	}
}

uint8_t _fifo_pop(FIFO* f){
	uint8_t data = 0;
	if (!_fifo_vide(f)){
		data = f->buffer[f->queue];
		f->queue = (f->queue + 1) % UART5_FIFO_TAILLE;
	}
	return data; // 0 si fifo vide
}

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

	// 5. Baudrate de 115200 (APB1 a un prescaler de 4 selon system_stm32f4xx.c)
	UART5->BRR &= 0;
	UART5->BRR |= (int)((SYS_CLOCK_FREQ/4.0) / (115200.0f*16.0)); //TODO : vérif que ça fonctionne

	// 6. activer transmitter et receiver
	UART5->CR1 |= BIT3;
	UART5->CR1 |= BIT2;

	// enable uart
	UART5->CR1 |= BIT13;

	// activer interrupt NVIC
	NVIC_EnableIRQ(UART5_IRQn);
}

void UART5_SendByte(uint8_t data) {
    _fifo_push(&uart5_tx_fifo, data);
    UART5->CR1 |= BIT7; // activer TX interrupt pour dire que transmission prête
}

uint8_t UART5_ReadByte(void) {
    return _fifo_pop(&uart5_rx_fifo);
}

// verifie flag pour savoir si transmission ou réception
void UART5_IRQHandler(void){
	uint8_t data;
	if (UART5->SR & BIT7){ // transmission prête
		data = _fifo_pop(&uart5_tx_fifo);
		if (data == 0){ // rien a envoyer
			UART5->CR1 &= ~BIT7;
		}
		else {
			UART5->DR |= data;
		}
	}
	if (UART5->SR & BIT5){ // réception en attente
		data = (uint8_t)(UART5->DR & 0xFF); // 8 premier bits du registre DR, lire DR réinitialise le flag RXNE
		_fifo_push(&uart5_rx_fifo, data);
	}
}
