#include "sdram.h"
#include "gpio.h"
#include "macros_utiles.h"
#include "delai.h"

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

	while(FMC_Bank5_6->SDSR & FMC_SDSR_BUSY); // vérifiez que le FMC n’est pas occup
	// Étape 1 : config FMC_SDCR1
	// RPIPE à 1
	FMC_Bank5_6->SDCR[0] &= ~BIT13;
	FMC_Bank5_6->SDCR[0] &= ~BIT14;
	FMC_Bank5_6->SDCR[0] |= BIT13;

	// Clk a HCLK/2
	FMC_Bank5_6->SDCR[0] &= ~BIT11;
	FMC_Bank5_6->SDCR[0] &= ~BIT10;
	FMC_Bank5_6->SDCR[0] |= BIT11;

	// NCAS à 3 coups d'horloges
	FMC_Bank5_6->SDCR[0] &= ~BIT8;
	FMC_Bank5_6->SDCR[0] &= ~BIT7;
	FMC_Bank5_6->SDCR[0] |= BIT7;
	FMC_Bank5_6->SDCR[0] |= BIT8;

	// désactiver burst mode
	FMC_Bank5_6->SDCR[0] &= ~BIT12;

	// désactiver write protection
	FMC_Bank5_6->SDCR[0] &= ~BIT9;

	// number of banks = 4
	FMC_Bank5_6->SDCR[0] |= BIT6;

	// bus de données de 16 bits
	FMC_Bank5_6->SDCR[0] &= ~BIT5;
	FMC_Bank5_6->SDCR[0] |= BIT4;

	// 12 lignes d'adresse
	FMC_Bank5_6->SDCR[0] &= ~BIT3;
	FMC_Bank5_6->SDCR[0] |= BIT2;

	// 8 colonnes d'adresse
	FMC_Bank5_6->SDCR[0] &= ~BIT1;
	FMC_Bank5_6->SDCR[0] &= ~BIT0;

	// Étape 2
	// Trcd 1 cycle
	FMC_Bank5_6->SDTR[0] &= ~BIT27;
	FMC_Bank5_6->SDTR[0] &= ~BIT26;
	FMC_Bank5_6->SDTR[0] &= ~BIT25;
	FMC_Bank5_6->SDTR[0] &= ~BIT24;

	// Trp 1 cycle
	FMC_Bank5_6->SDTR[0] &= ~BIT23;
	FMC_Bank5_6->SDTR[0] &= ~BIT22;
	FMC_Bank5_6->SDTR[0] &= ~BIT21;
	FMC_Bank5_6->SDTR[0] &= ~BIT20;

	// Twr 2 cycles
	FMC_Bank5_6->SDTR[0] &= ~BIT19;
	FMC_Bank5_6->SDTR[0] &= ~BIT18;
	FMC_Bank5_6->SDTR[0] &= ~BIT17;
	FMC_Bank5_6->SDTR[0] |= BIT16;

	// Trc 3 cycles
	FMC_Bank5_6->SDTR[0] &= ~BIT15;
	FMC_Bank5_6->SDTR[0] &= ~BIT14;
	FMC_Bank5_6->SDTR[0] |= BIT13;
	FMC_Bank5_6->SDTR[0] &= ~BIT12;

	// Tras 2 cycles
	FMC_Bank5_6->SDTR[0] &= ~BIT11;
	FMC_Bank5_6->SDTR[0] &= ~BIT10;
	FMC_Bank5_6->SDTR[0] &= ~BIT9;
	FMC_Bank5_6->SDTR[0] |= BIT8;

	// Txsr 3 cycles
	FMC_Bank5_6->SDTR[0] &= ~BIT7;
	FMC_Bank5_6->SDTR[0] &= ~BIT6;
	FMC_Bank5_6->SDTR[0] |= BIT5;
	FMC_Bank5_6->SDTR[0] &= ~BIT4;

	// Tmrd 2 cycles
	FMC_Bank5_6->SDTR[0] &= ~BIT3;
	FMC_Bank5_6->SDTR[0] &= ~BIT2;
	FMC_Bank5_6->SDTR[0] &= ~BIT1;
	FMC_Bank5_6->SDTR[0] |= BIT0;

	// Étape 3 : Set MODE bits to ‘001’ and configure the Target Bank bits (CTB1 and/or CTB2) in the
	// 		FMC_SDCMR register to start delivering the clock to the memory (SDCKE is driven high).
	FMC_Bank5_6->SDCMR &= ~BIT2;
	FMC_Bank5_6->SDCMR &= ~BIT1;
	FMC_Bank5_6->SDCMR &= ~BIT0;
	FMC_Bank5_6->SDCMR |= BIT0;
	FMC_Bank5_6->SDCMR |= BIT3;

	// Étape 4 : Wait during the prescribed delay period. Typical delay is around 100 μs (refer to the
	// 		SDRAM datasheet for the required delay after power-up).
	delai(1);

	// Étape 5 : Set MODE bits to ‘010’ and configure the Target Bank bits (CTB1 and/or CTB2) in the
	// 		FMC_SDCMR register to issue a “Precharge All” command.
	FMC_Bank5_6->SDCMR &= ~BIT2;
	FMC_Bank5_6->SDCMR &= ~BIT1;
	FMC_Bank5_6->SDCMR &= ~BIT0;
	FMC_Bank5_6->SDCMR |= BIT1;
	FMC_Bank5_6->SDCMR |= BIT3;

	// Étape 6 : Set MODE bits to ‘011’, and configure the Target Bank bits (CTB1 and/or CTB2) as well
	// 		as the number of consecutive Auto-refresh commands (NRFS) in the FMC_SDCMR
	// 		register. (Selon datasheet sdram, 2 auto-refresh minimum nécessaire (3 pour être certain)
	FMC_Bank5_6->SDCMR &= ~BIT2;
	FMC_Bank5_6->SDCMR &= ~BIT1;
	FMC_Bank5_6->SDCMR &= ~BIT0;
	FMC_Bank5_6->SDCMR |= BIT1;
	FMC_Bank5_6->SDCMR |= BIT0;
	FMC_Bank5_6->SDCMR |= BIT3;
	FMC_Bank5_6->SDCMR |= BIT6;

	// Étape 7 : Configure the MRD field according to your SDRAM device, set the MODE bits to '100',
	// 		and configure the Target Bank bits (CTB1 and/or CTB2) in the FMC_SDCMR register
	// 		to issue a "Load Mode Register" command in order to program the SDRAM. In particular:
	// 			a) The CAS latency must be selected following configured value in FMC_SDCR1/2 registers
	// 			b) The Burst Length (BL) of 1 must be selected by configuring the M[2:0] bits to 000
	// 				in the mode register (refer to the SDRAM datasheet). If the Mode Register is not
	//	 			the same for both SDRAM banks, this step has to be repeated twice, once for
	// 				each bank, and the Target Bank bits set accordingly
	// Commande 0x231
	FMC_Bank5_6->SDCMR |= BIT9;
	FMC_Bank5_6->SDCMR &= ~BIT10;
	FMC_Bank5_6->SDCMR &= ~BIT11;
	FMC_Bank5_6->SDCMR &= ~BIT12;
	FMC_Bank5_6->SDCMR |= BIT13;
	FMC_Bank5_6->SDCMR |= BIT14;
	FMC_Bank5_6->SDCMR &= ~BIT15;
	FMC_Bank5_6->SDCMR &= ~BIT16;
	FMC_Bank5_6->SDCMR &= ~BIT17;
	FMC_Bank5_6->SDCMR |= BIT18;
	FMC_Bank5_6->SDCMR &= ~BIT19;
	FMC_Bank5_6->SDCMR &= ~BIT20;
	FMC_Bank5_6->SDCMR &= ~BIT21;
	// Mode 100
	FMC_Bank5_6->SDCMR &= ~BIT2;
	FMC_Bank5_6->SDCMR &= ~BIT1;
	FMC_Bank5_6->SDCMR &= ~BIT0;
	FMC_Bank5_6->SDCMR |= BIT2;
	// Target bank 2 (CTB2)
	FMC_Bank5_6->SDCMR |= BIT3;

	// Étape 8 : Program the refresh rate in the FMC_SDRTR register
	// Refresh rate = (tref/Nlignes * fclk)-20 = 542,5 => 542
	uint16_t refresh_count = 542;
	FMC_Bank5_6->SDRTR |= refresh_count << 1;

	// Étape 9 pas à faire (pas une mobile SDRAM)
}
