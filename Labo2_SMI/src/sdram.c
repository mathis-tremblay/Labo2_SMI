#include "sdram.h"
#include "GPIO.h"
#include "macros_utiles.h"
#include "delai.h"

#define SDRAM_BASE_ADDR  ((uint32_t)0xD0000000) // Bank2
#define SDRAM_SIZE_BYTES ((uint32_t)0x00800000)   // 8 Mo (64 Mbits)

void SDRAM_Init(void){
	// Clk FMC
	RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;

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

	while(FMC_Bank5_6->SDSR & FMC_SDSR_BUSY); // verifiez que le FMC pas occup

	// Etape 1 : config FMC_SDCR1
	// RPIPE a 1
	FMC_Bank5_6->SDCR[0] &= ~(BIT14|BIT13);
	FMC_Bank5_6->SDCR[0] |= BIT13;

	// Clk a HCLK/2
	FMC_Bank5_6->SDCR[0] &= ~(BIT11|BIT10);
	FMC_Bank5_6->SDCR[0] |= BIT11;

	// NCAS a 3 coups d'horloges
	FMC_Bank5_6->SDCR[1] &= ~(BIT8|BIT7);
	FMC_Bank5_6->SDCR[1] |= (BIT8|BIT7);

	// désactiver burst mode / write protection
	FMC_Bank5_6->SDCR[0] &= ~(BIT12|BIT9);
	FMC_Bank5_6->SDCR[1] &= ~(BIT12|BIT9);

	// number of banks = 4
	FMC_Bank5_6->SDCR[1] |= BIT6;

	// bus de donnees de 16 bits
	FMC_Bank5_6->SDCR[1] &= ~(BIT5|BIT4);
	FMC_Bank5_6->SDCR[1] |= BIT4;

	// 12 lignes d'adresse
	FMC_Bank5_6->SDCR[1] &= ~(BIT3|BIT2);
	FMC_Bank5_6->SDCR[1] |= BIT2;

	// 8 colonnes d'adresse
	FMC_Bank5_6->SDCR[1] &= ~(BIT1|BIT0);

	// Etape 2
	// Trcd 1 cycle
	FMC_Bank5_6->SDTR[1] &= ~BIT27;
	FMC_Bank5_6->SDTR[1] &= ~BIT26;
	FMC_Bank5_6->SDTR[1] &= ~BIT25;
	FMC_Bank5_6->SDTR[1] &= ~BIT24;

	// Trp 1 cycle
	FMC_Bank5_6->SDTR[1] &= ~BIT23;
	FMC_Bank5_6->SDTR[1] &= ~BIT22;
	FMC_Bank5_6->SDTR[1] &= ~BIT21;
	FMC_Bank5_6->SDTR[1] &= ~BIT20;
	FMC_Bank5_6->SDTR[0] &= ~BIT23;
	FMC_Bank5_6->SDTR[0] &= ~BIT22;
	FMC_Bank5_6->SDTR[0] &= ~BIT21;
	FMC_Bank5_6->SDTR[0] &= ~BIT20;

	// Twr 2 cycles
	FMC_Bank5_6->SDTR[1] &= ~BIT19;
	FMC_Bank5_6->SDTR[1] &= ~BIT18;
	FMC_Bank5_6->SDTR[1] &= ~BIT17;
	FMC_Bank5_6->SDTR[1] |= BIT16;

	// Trc 3 cycles
	FMC_Bank5_6->SDTR[1] &= ~BIT15;
	FMC_Bank5_6->SDTR[1] &= ~BIT14;
	FMC_Bank5_6->SDTR[1] |= BIT13;
	FMC_Bank5_6->SDTR[1] &= ~BIT12;
	FMC_Bank5_6->SDTR[0] &= ~BIT15;
	FMC_Bank5_6->SDTR[0] &= ~BIT14;
	FMC_Bank5_6->SDTR[0] |= BIT13;
	FMC_Bank5_6->SDTR[0] &= ~BIT12;

	// Tras 2 cycles
	FMC_Bank5_6->SDTR[1] &= ~BIT11;
	FMC_Bank5_6->SDTR[1] &= ~BIT10;
	FMC_Bank5_6->SDTR[1] &= ~BIT9;
	FMC_Bank5_6->SDTR[1] |= BIT8;

	// Txsr 3 cycles
	FMC_Bank5_6->SDTR[1] &= ~BIT7;
	FMC_Bank5_6->SDTR[1] &= ~BIT6;
	FMC_Bank5_6->SDTR[1] |= BIT5;
	FMC_Bank5_6->SDTR[1] &= ~BIT4;

	// Tmrd 2 cycles
	FMC_Bank5_6->SDTR[1] &= ~BIT3;
	FMC_Bank5_6->SDTR[1] &= ~BIT2;
	FMC_Bank5_6->SDTR[1] &= ~BIT1;
	FMC_Bank5_6->SDTR[1] |= BIT0;

	// Etape 3 : Set MODE bits to '001' and configure the Target Bank bits (CTB1 and/or CTB2) in the
	// 		FMC_SDCMR register to start delivering the clock to the memory (SDCKE is driven high).
	FMC_Bank5_6->SDCMR = BIT0|BIT3;

	// Etape 4 : Wait during the prescribed delay period. Typical delay is around 100us (refer to the
	// 		SDRAM datasheet for the required delay after power-up).
	delai(1);

	// Etape 5 : Set MODE bits to '010' and configure the Target Bank bits (CTB1 and/or CTB2) in the
	// 		FMC_SDCMR register to issue a Precharge All command.
	FMC_Bank5_6->SDCMR = BIT1 | BIT3;
	while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);

	// Etape 6 : Set MODE bits to '011', and configure the Target Bank bits (CTB1 and/or CTB2) as well
	// 		as the number of consecutive Auto-refresh commands (NRFS) in the FMC_SDCMR
	// 		register. (Selon datasheet sdram, 2 auto-refresh minimum necessaires. On prend 8)
	FMC_Bank5_6->SDCMR = BIT0 | BIT1 | BIT3 | BIT7 | BIT6 | BIT5;
	while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);

	// Etape 7 : Configure the MRD field according to your SDRAM device, set the MODE bits to '100',
	// 		and configure the Target Bank bits (CTB1 and/or CTB2) in the FMC_SDCMR register
	// 		to issue a "Load Mode Register" command in order to program the SDRAM. In particular:
	// 			a) The CAS latency must be selected following configured value in FMC_SDCR1/2 registers
	// 			b) The Burst Length (BL) of 1 must be selected by configuring the M[2:0] bits to 000
	// 				in the mode register (refer to the SDRAM datasheet). If the Mode Register is not
	//	 			the same for both SDRAM banks, this step has to be repeated twice, once for
	// 				each bank, and the Target Bank bits set accordingly
	// Commande 0x231
	uint32_t mode_reg = 0x0231;
	FMC_Bank5_6->SDCMR = BIT2 | BIT3 | (mode_reg << 9);
	while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);

	// Etape 8 : Program the refresh rate in the FMC_SDRTR register
	// Refresh rate = (tref/Nlignes * fclk)-20 = 542,5 => 542
	uint16_t refresh_count = 543;
	FMC_Bank5_6->SDRTR = refresh_count << 1;
}

void SDRAM_Write(uint32_t address, uint16_t data) {
    if (address < 0x00800000) // 8 Mo maximum pour IS42S16400J (64 Mbits = 8 MB)
    	*(__IO uint16_t*)(SDRAM_BASE_ADDR + address) = data;
}


uint16_t SDRAM_Read(uint32_t address)
{
    if (address >= 0x00800000)
        return 0xFFFF; // Valeur d’erreur arbitraire

    return *(__IO uint16_t*)(SDRAM_BASE_ADDR + address);
}


void SDRAM_WriteArray(uint32_t address, const uint16_t* data, uint32_t length)
{
    // Empeche les erreurs evidentes
    if (data == 0 || length == 0) return;
    if (address >= SDRAM_SIZE_BYTES) return;

    // adresse de depart doit être paire (on peut pas ecrire sur un demi-bloc)
    if (address & 0x1) return;

    // Tronquer si ça depasse la taille de la SDRAM (evite overflow)
    uint32_t bytes_left   = SDRAM_SIZE_BYTES - address;
    uint32_t words_left   = bytes_left >> 1;            // bytes_left / 2
    if (length > words_left) {
    	length = words_left;
    }

    // Ecriture sequentielle
    for (uint32_t i = 0; i < length; ++i) {
            SDRAM_Write(address + (i << 1), data[i]);
    }
}
