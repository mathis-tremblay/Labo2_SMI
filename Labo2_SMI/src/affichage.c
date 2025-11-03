#include "affichage.h"
#include "stm32f4xx.h"
#include "lcd_driver_src/lcd_driver.h"
#include "uart.h"
#include "spi.h"

static uint16_t charColor;
static uint16_t bgColor;

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

void Affichage_CharBgWrite(uint8_t character, uint16_t cursorX, uint16_t cursorY)
{
	LCD_WriteChar(character, bgColor, charColor, cursorX, cursorY);
}
