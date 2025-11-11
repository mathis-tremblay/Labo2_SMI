#include "sdram.h"
#include "gpio.h"
#include "macros_utiles.h"

void SDRAM_Init(void){
	// Configuration des GPIOs requis
	GPIO_Config(GPIOF, 0, 2, 0, 3, 12); // A0
	GPIO_Config(GPIOF, 1, 2, 0, 3, 12); // A1
	GPIO_Config(GPIOF, 2, 2, 0, 3, 12); // A2
	GPIO_Config(GPIOF, 3, 2, 0, 3, 12); // A3
	GPIO_Config(GPIOF, 4, 2, 0, 3, 12); // A4
	GPIO_Config(GPIOF, 5, 2, 0, 3, 12); // A5
	GPIO_Config(GPIOF, 12, 2, 0, 3, 12); // A6
	GPIO_Config(GPIOF, 13, 2, 0, 3, 12); // A7
	GPIO_Config(GPIOF, 14, 2, 0, 3, 12); // A8
	GPIO_Config(GPIOF, 15, 2, 0, 3, 12); // A9
	GPIO_Config(GPIOG, 0, 2, 0, 3, 12); // A10
	GPIO_Config(GPIOG, 1, 2, 0, 3, 12); // A11

	GPIO_Config(GPIOD, 14, 2, 0, 3, 12); // D0
	GPIO_Config(GPIOD, 15, 2, 0, 3, 12); // D1
	GPIO_Config(GPIOD, 0, 2, 0, 3, 12); // D2
	GPIO_Config(GPIOD, 1, 2, 0, 3, 12); // D3
	GPIO_Config(GPIOE, 7, 2, 0, 3, 12); // D4
	GPIO_Config(GPIOE, 8, 2, 0, 3, 12); // D5
	GPIO_Config(GPIOE, 9, 2, 0, 3, 12); // D6
	GPIO_Config(GPIOE, 10, 2, 0, 3, 12); // D7
	GPIO_Config(GPIOE, 11, 2, 0, 3, 12); // D8
	GPIO_Config(GPIOE, 12, 2, 0, 3, 12); // D9
	GPIO_Config(GPIOE, 13, 2, 0, 3, 12); // D10
	GPIO_Config(GPIOE, 14, 2, 0, 3, 12); // D11
	GPIO_Config(GPIOE, 15, 2, 0, 3, 12); // D12
	GPIO_Config(GPIOD, 8, 2, 0, 3, 12); // D13
	GPIO_Config(GPIOD, 9, 2, 0, 3, 12); // D14
	GPIO_Config(GPIOD, 10, 2, 0, 3, 12); // D15

	GPIO_Config(GPIOC, 0, 2, 0, 3, 12); // SDNWE
	GPIO_Config(GPIOG, 8, 2, 0, 3, 12); // SDCLK
	GPIO_Config(GPIOF, 11, 2, 0, 3, 12); // SDNRAS
	GPIO_Config(GPIOG, 15, 2, 0, 3, 12); // SDNCAS
	GPIO_Config(GPIOB, 5, 2, 0, 3, 12); // SDCKE1
	GPIO_Config(GPIOB, 6, 2, 0, 3, 12); // SDNE1

	GPIO_Config(GPIOG, 4, 2, 0, 3, 12); // BA0
	GPIO_Config(GPIOG, 5, 2, 0, 3, 12); // BA1

	GPIO_Config(GPIOE, 0, 2, 0, 3, 12); // NBL0
	GPIO_Config(GPIOE, 1, 2, 0, 3, 12); // NBL1

	// Étape 1 : config FMC_SDCR1
	// RPIPE à 1
	FMC_SDCR1 &= ~BIT13;
	FMC->SDCR[1] &= ~BIT14;
	FMC->SDCR[1] |= BIT13;

	// Clk a HCLK/2
	FMC->SDCR[1] &= ~BIT11;
	FMC->SDCR[1] &= ~BIT10;
	FMC->SDCR[1] |= BIT11;

	// NCAS à 3 coups d'horloges
	FMC->SDCR[1] &= ~BIT8;
	FMC->SDCR[1] &= ~BIT7;
	FMC->SDCR[1] |= BIT7;
	FMC->SDCR[1] |= BIT8;

	// désactiver burst mode
	FMC->SDCR[1] &= ~BIT12;

	// désactiver write protection
	FMC->SDCR[1] &= ~BIT9;

	// number of banks = 4
	FMC->SDCR[1] |= BIT6;

	// bus de données de 16 bits
	FMC->SDCR[1] &= ~BIT5;
	FMC->SDCR[1] |= BIT4;

	// 12 lignes d'adresse
	FMC->SDCR[1] &= ~BIT3;
	FMC->SDCR[1] |= BIT2;

	// 8 colonnes d'adresse
	FMC->SDCR[1] &= ~BIT1;
	FMC->SDCR[1] &= ~BIT0;
}
