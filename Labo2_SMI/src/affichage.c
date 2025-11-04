#include "affichage.h"
#include "stm32f4xx.h"
#include "lcd_driver_src/lcd_driver.h"
#include "uart.h"
#include "spi.h"
#include <string.h>
#include <stdlib.h>

static uint16_t charColor;
static uint16_t bgColor;
static uint16_t cursor_x = 0;
static uint16_t cursor_y = 0;

void Affichage_Init(void)
{
	LCD_InitGPIO();
	GPIO_Config(GPIOF, 7, 2, 0, 3, 5); // SCLK
	//GPIO_Config(GPIOF, 8, 2, 0, 3, 5); // MISO
	GPIO_Config(GPIOF, 9, 2, 0, 3, 5); // MOSI

	SPI_EnableClock(SPI5);
	SPI_Config(SPI5, 1, 8, 0, 0, 0, 0, 1, 0, 1);

	LCD_InitSerialInterface();
}

void Affichage_SetBgColor(uint8_t r, uint8_t g, uint8_t b)
{
	bgColor = (r << 11) | (g << 5) | b;
}

void Affichage_SetCharColor(uint8_t r, uint8_t g, uint8_t b)
{
	charColor = (r << 11) | (g << 5) | b;
}

void Affichage_UpdateBg()
{
	LCD_CopyColorToFrameBuffer(bgColor);
	LCD_TransmitFrameBuffer();
}

void Affichage_CharBgWrite(uint8_t character, uint16_t cursorX, uint16_t cursorY)
{
	LCD_WriteChar(character, bgColor, charColor, cursorX, cursorY);
}

void Affichage_ProcessString(const char* str)
{
	int len = strlen(str);
	for (int i = 0; i < len; i++)
	{
		char c = str[i];

		if (c == '\n')
		{
			cursor_x = 0;
			cursor_y += (FONT_HEIGHT + LINE_SPACING);
		}
		else if (c == '\r')
		{
			cursor_x = 0;
		}
		else if (strncmp(&str[i], "SC", 2) == 0 && (i + 8 <= len))
		{
			char rr[3] = { str[i+2], str[i+3], '\0' };
			char gg[3] = { str[i+4], str[i+5], '\0' };
			char bb[3] = { str[i+6], str[i+7], '\0' };

			uint8_t r = (uint8_t)atoi(rr);
			uint8_t g = (uint8_t)atoi(gg);
			uint8_t b = (uint8_t)atoi(bb);

			Affichage_SetCharColor(r, g, b);

			i += 7;
		}
		else if (strncmp(&str[i], "BC", 2) == 0 && (i + 8 <= len))
		{
			char rr[3] = { str[i+2], str[i+3], '\0' };
			char gg[3] = { str[i+4], str[i+5], '\0' };
			char bb[3] = { str[i+6], str[i+7], '\0' };

			uint8_t r = (uint8_t)atoi(rr);
			uint8_t g = (uint8_t)atoi(gg);
			uint8_t b = (uint8_t)atoi(bb);

			Affichage_SetBgColor(r, g, b);
			Affichage_UpdateBg();

			i += 7;
		}
		else
		{
			if (cursor_x + FONT_WIDTH > SCREEN_WIDTH)
			{
				cursor_x = 0;
				cursor_y += (FONT_HEIGHT + LINE_SPACING);
			}

			if (cursor_y + FONT_HEIGHT > SCREEN_HEIGHT)
			{
				cursor_y = 0;
			}

			Affichage_CharBgWrite(c, cursor_x, cursor_y);
			cursor_x += FONT_WIDTH;
		}
	}
}
