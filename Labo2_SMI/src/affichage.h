#include "stm32f4xx.h"

#ifndef AFFICHAGE_H_
#define AFFICHAGE_H_

void Affichage_Init(void);
void Affichage_SetBgColor(uint8_t r, uint8_t g, uint8_t b);
void Affichage_SetCharColor(uint8_t r, uint8_t g, uint8_t b);
void Affichage_CharBgWrite(uint8_t character, uint16_t cursorX, uint16_t cursorY);

#endif /* AFFICHAGE_H_ */
