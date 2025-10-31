// TODO

// Module SPI pour initialiser pis faire fonctionner le LCD [DONE]

// Completer les fonctions de lcd_driver_src

// Activer la bonne clk

// Editer registres, et mettre mode alternatif sur GPIO

#ifndef SPI_H
#define SPI_H

#include <stdint.h>

// Initialisation du SPI (peripherique SPIx)
void SPI_Init(SPI_TypeDef *SPIx);

void SPI_Config(SPI_TypeDef *SPIx, uint8_t mode, uint8_t data_size, uint8_t cpol, uint8_t cpha,
                uint8_t prescaler, uint8_t lsb_first, uint8_t ssm_enable, uint8_t motorola_mode, uint8_t enable_periph);

#endif // SPI_H
