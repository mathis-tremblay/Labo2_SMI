#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include <stddef.h>
#include "stm32f4xx.h"

// Initialisation du SPI (peripherique SPIx)
void SPI_EnableClock(SPI_TypeDef *SPIx);

// Configuration des registres du SPIx
void SPI_Config(SPI_TypeDef *SPIx, uint8_t mode, uint8_t data_size, uint8_t cpol, uint8_t cpha,
                uint8_t prescaler, uint8_t lsb_first, uint8_t ssm_enable, uint8_t motorola_mode, uint8_t enable_periph);

// Deplacement des donnees
void SPI_Transmit(const void *data, size_t nbytes);

#endif // SPI_H
