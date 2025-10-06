#ifndef CONTROLEURLED_H
#define CONTROLEURLED_H

#include <stdint.h>

/* Initialise le module ControleurLED (ADC, PWM, GPIO nécessaires) */
void ControleurLED_Init(void);

/* Allume la LED (active PWM) */
void ControleurLED_On(void);

/* Éteint la LED (désactive PWM ou met duty à 0) */
void ControleurLED_Off(void);

/* Met à jour l'intensité en fonction de la valeur ADC fournie (0-4095) */
void ControleurLED_SetIntensityFromADC(uint32_t adc_value);

/* Lecture et application directe : lance une conversion ADC et applique la valeur lue */
void ControleurLED_UpdateFromADCAndApply(void);

#endif /* CONTROLEURLED_H */
