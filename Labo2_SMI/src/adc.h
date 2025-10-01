#ifndef ADC_H_
#define ADC_H_

#include "stm32f4xx.h"

/*
 * - Pas de variable globale! Préférez l’utilisation du mot clé « static » si vous avez besoin d’une variable partagée entre les fonctions d’un même module
 * - Une fonction d’initialisation
 * - Une fonction qui démarre une conversion
 * - Une fonction qui retourne la valeur convertie par l’ADC
 * - Une fonction d’interruption lorsqu’une conversion est prête. Assurez-vous qu’elle porte le même nom que la fonction associée dans la table des vecteurs d’interruption
 *
 * */

void ADC_Config(ADC_TypeDef * ADCx, uint8_t channel);

void ADC_StartConvert(ADC_TypeDef * ADCx);

uint32_t ADC_GetSample(ADC_TypeDef * ADCx);

void ADC_IRQHandler(ADC_TypeDef * ADCx);

#endif /* ADC_H_ */
