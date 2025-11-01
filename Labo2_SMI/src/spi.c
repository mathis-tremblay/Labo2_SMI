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
    // Desactiver le SPI pendant configuration
    SPIx->CR1 &= ~SPI_CR1_SPE;

    // Nettoyer CR1 et CR2
    SPIx->CR1 = 0;
    SPIx->CR2 = 0;

    // Configurer CR1 (registre principal)
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
    if (motorola_mode) {
        cr2 |= SPI_CR2_FRF;  // 1 = TI frame format
    }
    else {
        cr2 &= ~SPI_CR2_FRF; // 0 = Motorola
    }

    SPIx->CR2 = cr2;

    // Configurer si les peripheriques sont enables
    if (enable_periph) {
    	SPIx->CR1 |= SPI_CR1_SPE;
    }
    else {
    	SPIx->CR1 &= ~SPI_CR1_SPE;
    }
}




#ifndef SPI_TX_TIMEOUT
#define SPI_TX_TIMEOUT  (1000000UL)
#endif

void SPI_Transmit(const void *data, size_t nbytes)
{
    const uint8_t *p8  = (const uint8_t *)data;
    const uint16_t *p16 = (const uint16_t *)data;
    uint32_t guard;

    // Verifier si SPI configurer en 8 ou 16 bits (bit DFF du CR1)
    uint8_t is16bit = (LCD_SPI->CR1 & SPI_CR1_DFF) ? 1 : 0;

    if (!is16bit) {
        // ======== MODE 8 BITS ========
        for (size_t i = 0; i < nbytes; ++i) {
            guard = SPI_TX_TIMEOUT;
            while ((LCD_SPI->SR & SPI_SR_TXE) == 0) {
                if (--guard == 0) return;  // timeout TXE
            }
            *(__IO uint8_t *)&LCD_SPI->DR = *p8++;
        }
    }
    else {
        // ======== MODE 16 BITS ========
        size_t count = nbytes / 2;
        for (size_t i = 0; i < count; ++i) {
            guard = SPI_TX_TIMEOUT;
            while ((LCD_SPI->SR & SPI_SR_TXE) == 0) {
                if (--guard == 0) return;  // timeout TXE
            }
            *(__IO uint16_t *)&LCD_SPI->DR = *p16++;
        }
    }

    // Attendre la fin matérielle du dernier bit (BSY=0)
    guard = SPI_TX_TIMEOUT;
    while (LCD_SPI->SR & SPI_SR_BSY) {
        if (--guard == 0) break;
    }

    // Flush RX pour éviter OVR
    (void)LCD_SPI->DR;
    (void)LCD_SPI->SR;
}
