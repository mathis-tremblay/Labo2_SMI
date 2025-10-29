#include "stm32f4xx.h"

#ifndef UART_H_
#define UART_H_

// FIFO circulaire
#define UART5_FIFO_TAILLE 128  // taille du buffer

typedef struct {
    uint8_t buffer[UART5_FIFO_TAILLE];
    volatile uint16_t tete;  // index d'écriture
    volatile uint16_t queue;  // index de lecture
} FIFO;

int _fifo_pleine(FIFO* f);

int _fifo_vide(FIFO* f);

void _fifo_push(FIFO* f, uint8_t data);

int _fifo_pop(FIFO* f, uint8_t* out);

void UART5_Config();

void UART5_SendByte(uint8_t data);

uint8_t UART5_ReadByte(void);

#endif /* UART_H_ */
