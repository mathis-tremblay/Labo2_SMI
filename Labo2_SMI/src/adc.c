#import "macros_utiles.h"
#import "adc.h"

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
	NVIC_EnableIRQ(ADC_IRQn);
	NVIC_SetPriority(ADC_IRQn, 0);
}

void ADC_StartConvert(void){
	//ADC_STRT
}

uint32_t ADC_GetSample(void){

}

void ADC_IRQHandler(void){

}
