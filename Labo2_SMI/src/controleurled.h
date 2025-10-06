#ifndef CONTROLEURLED_H
#define CONTROLEURLED_H

#include <stdint.h>

void ControleurLED_Init(void);

// Allume LED (active PWM)
void ControleurLED_On(void);

// Éteint LED
void ControleurLED_Off(void);

// Lecture et application directe : lance une conversion ADC et applique la valeur lue
void ControleurLED_UpdateFromADCAndApply(void);

#endif /* CONTROLEURLED_H */
