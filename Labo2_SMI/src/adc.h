#ifndef ADC_H_
#define ADC_H_

#include "stm32f4xx.h"

/*
 * - Pas de variable globale! Pr�f�rez l�utilisation du mot cl� � static � si vous avez besoin d�une variable partag�e entre les fonctions d�un m�me module
 * - Une fonction d�initialisation
 * - Une fonction qui d�marre une conversion
 * - Une fonction qui retourne la valeur convertie par l�ADC
 * - Une fonction d�interruption lorsqu�une conversion est pr�te. Assurez-vous qu�elle porte le m�me nom que la fonction associ�e dans la table des vecteurs d�interruption
 *
 * */

void ADC_Init(ADC_TypeDef * ADCx, uint8_t channel);

void ADC_StartConvert(void);

uint32_t ADC_GetSample(void);

void ADC_IRQHandler(void);

#endif /* ADC_H_ */
