#include "stm32f4xx.h"

#ifndef AFFICHAGE_H_
#define AFFICHAGE_H_

#define FONT_WIDTH 11       // CHAR_WIDTH_16
#define FONT_HEIGHT 16      // CHAR_HEIGHT_16
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320
#define LINE_SPACING 2      // Espace vertical entre les lignes

void Affichage_Init(void);
void Affichage_SetBgColor(uint8_t r, uint8_t g, uint8_t b);
void Affichage_SetCharColor(uint8_t r, uint8_t g, uint8_t b);
void Affichage_UpdateBg();
void Affichage_CharBgWrite(uint8_t character, uint16_t cursorX, uint16_t cursorY);
void Affichage_ProcessString(const char* str);

#endif /* AFFICHAGE_H_ */
