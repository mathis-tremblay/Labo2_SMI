#include "sdram.h"
#include "gpio.h"
#include "macros_utiles.h"
#include "delai.h"

#define SDRAM_BASE_ADDR  ((uint32_t)0xD0000000) // Bank2
#define SDRAM_SIZE_BYTES ((uint32_t)0x00800000)   // 8 Mo (64 Mbits)

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

#define SDRAM_TMRD  2    // tMRD = 2 cycles
#define SDRAM_TXSR  3    // tXSR = 3 cycles
#define SDRAM_TRAS  2    // tRAS = 2 cycles
#define SDRAM_TRC   3    // tRC  = 3 cycles
#define SDRAM_TWR   2    // tWR  = 2 cycles
#define SDRAM_TRP   1    // tRP  = 1 cycle
#define SDRAM_TRCD  1    // tRCD = 1 cycle

void SDRAM_Init(void)
{
    /* 1) Active les horloges GPIO et FMC */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN |
                    RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN |
                    RCC_AHB1ENR_GPIOFEN | RCC_AHB1ENR_GPIOGEN;
    RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;

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

    /* 4) Séquence d’initialisation (étapes RM0090 § 37.7.3) */
    // Étape 1 : Clock enable command
    FMC_Bank5_6->SDCMR = FMC_SDCMR_CTB2 | (1 << FMC_SDCMR_MODE_Pos);
    while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);

	// number of banks = 4
	FMC_Bank5_6->SDCR[1] |= BIT6;

    // Étape 3 : Precharge All
    FMC_Bank5_6->SDCMR = FMC_SDCMR_CTB2 | (2 << FMC_SDCMR_MODE_Pos);
    while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);

    // Étape 4 : Auto-refresh × 8
    FMC_Bank5_6->SDCMR = FMC_SDCMR_CTB2 | (3 << FMC_SDCMR_MODE_Pos) |
                          (8 << FMC_SDCMR_NRFS_Pos);
    while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);

    // Étape 5 : Load Mode Register (0x231)
    FMC_Bank5_6->SDCMR = FMC_SDCMR_CTB2 | (4 << FMC_SDCMR_MODE_Pos) |
                          (0x231 << FMC_SDCMR_MRD_Pos);
    while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);

    /* 5) Programmation du registre de rafraîchissement (tREF) */
    // tREF = 64 ms / 4096 rows = 15.625 µs par ligne.
    // 36 MHz ⇒ 36 000 000 × 15.625e-6 ≈ 563 coups d’horloge.
    FMC_Bank5_6->SDRTR = (563 << 1);

    /* La SDRAM est maintenant opérationnelle. */
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
