#include "spi.h"
#include "stm32f4xx.h"   // Def des registres STM32F429

void SPI_Init(SPI_TypeDef *SPIx)
{
    // Activer horloge SPI (APB1 ou APB2)
    if (SPIx == SPI1) RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    else if (SPIx == SPI2) RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    else if (SPIx == SPI3) RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
    else if (SPIx == SPI4) RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
    else if (SPIx == SPI5) RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;
    else if (SPIx == SPI6) RCC->APB2ENR |= RCC_APB2ENR_SPI6EN;
}


// mode : 0 = slave, 1 = master
// data_size : 8 = 8 bits, 16 = 16 bits
// cpol : 0 = horloge idle bas, 1 = idle haut
// cpha : 0 = echantillonnage sur 1er front, 1 = 2e front
// prescaler : 0 a 7 -> diviseur horloge = fPCLK / 2^(prescaler+1)
// lsb_first : 0 = MSB en premier, 1 = LSB en premier
// ssm_enable : 0 = NSS materiel, 1 = NSS logiciel (SSM=SSI=1)
// motorola_mode : 0 = format Motorola, 1 = format TI
// enable_periph : 0 = peripherique desactiver, 1 = peripherique activer
void SPI_Config(SPI_TypeDef *SPIx, uint8_t mode, uint8_t data_size, uint8_t cpol, uint8_t cpha,
                uint8_t prescaler, uint8_t lsb_first, uint8_t ssm_enable, uint8_t motorola_mode,
				uint8_t enable_periph)
{
    // 1️ Desactiver le SPI pendant configuration
    SPIx->CR1 &= ~SPI_CR1_SPE;

    // 2️ Nettoyer CR1 et CR2
    SPIx->CR1 = 0;
    SPIx->CR2 = 0;

    // 3️ Configurer CR1 (registre principal)
    uint32_t cr1 = 0;

    // Master ou slave
    if (mode) cr1 |= SPI_CR1_MSTR;

    // Polariter et phase
    if (cpol) cr1 |= SPI_CR1_CPOL;
    if (cpha) cr1 |= SPI_CR1_CPHA;

    // Ordre des bits
    if (lsb_first) cr1 |= SPI_CR1_LSBFIRST;

    // Taille des donnees (DFF)
    if (data_size == 16) cr1 |= SPI_CR1_DFF;

    // Prescaler (3 bits BR[2:0])
    cr1 |= ((uint32_t)(prescaler & 0x7) << 3);

    // Gestion NSS
    if (ssm_enable) {
        cr1 |= SPI_CR1_SSM | SPI_CR1_SSI;  // NSS logiciel
    }

    SPIx->CR1 = cr1;

    // Configurer CR2 (format Motorola / TI)
    uint32_t cr2 = 0;
    if (motorola_mode)
        cr2 |= SPI_CR2_FRF;  // 1 = TI frame format
    else
        cr2 &= ~SPI_CR2_FRF; // 0 = Motorola

    SPIx->CR2 = cr2;

    // Configurer si les peripheriques sont enables
    if (enable_periph)
    	SPIx->CR1 |= SPI_CR1_SPE;
    else
    	SPIx->CR1 &= ~SPI_CR1_SPE;

    // Activer le SPI
    SPIx->CR1 |= SPI_CR1_SPE;
}

