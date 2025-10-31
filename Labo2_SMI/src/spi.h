// TODO

// Module SPI pour initialiser pis faire fonctionner le LCD [DONE]

// Completer les fonctions de lcd_driver_src

// Activer la bonne clk

// Editer registres, et mettre mode alternatif sur GPIO

#ifndef SPI_H
#define SPI_H

#include <stdint.h>

// Initialisation du SPI (port F, peripherique SPI5)
void SPI_Init(void);


#endif // SPI_H
