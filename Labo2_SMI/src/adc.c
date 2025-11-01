#include "macros_utiles.h"
#include "adc.h"

static volatile uint32_t adc_sample = 0;
static volatile uint8_t adc_ready = 0;

/*
 * Un seul canal (Channel), donc une séquence d’une seule conversion
 * Pas de DMA 12 bits aligné à droite
 * Mode conversion simple (Single conversion)
 * La conversion débute par logiciel (Software)
 * L’interruption EOC doit être activée
 */
void ADC_Config(ADC_TypeDef * ADCx, uint8_t channel){

	// Activer horloge ADC correspondante
	if (ADCx == ADC1) RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	else if (ADCx == ADC2) RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
	else if (ADCx == ADC3) RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;

	// Configuration de base
	ADCx->CR1 = 0;  // reset CR1
	ADCx->CR2 = 0;  // reset CR2
	ADCx->SQR1 = 0; // 1 conversion

	// Conversion 12 bits
	ADCx->CR1 &= ~BIT25;
	ADCx->CR1 &= ~BIT24;

	// Activer interruptions EOC
	ADCx->CR1 |= BIT5;

	// Alignement a droite
	ADCx->CR2 &= ~BIT11;

	// Single conversion mode
	ADCx->CR2 &= ~BIT1;

	// Sélectionner le canal (5 bits max sur SQ1)
	ADCx->SQR3 = channel & 0x1F;

	// Enable ADC
	ADCx->CR2 |= BIT0;

	// Activer NVIC
	NVIC->ISER[0] = BIT18;
}

void ADC_StartConvert(ADC_TypeDef * ADCx){
    // Lancer conversion
    ADCx->CR2 |= BIT30;
}

uint8_t ADC_IsReady(void){
    return adc_ready;
}

uint32_t ADC_GetSample(ADC_TypeDef * ADCx){
	// Attendre une interruption
	adc_ready = 0;
	return adc_sample;
}

void ADC_IRQHandler(void) {
    if (ADC1->SR & ADC_SR_EOC) {       // EOC = End Of Conversion flag
        adc_sample = ADC1->DR;         // Lecture du DR efface le flag EOC
        adc_ready = 1;
    }
}

