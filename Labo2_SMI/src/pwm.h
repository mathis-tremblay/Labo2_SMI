#ifndef PWM_H
#define PWM_H

#include <stdint.h>

// Initialisation du PWM (Timer2, PA5)
void PWM_Init(void);

// Ajuster le rapport cyclique (duty cycle en %)
void PWM_SetDutyCycle(uint8_t duty);

// Ajuster la fréquence en Hz
void PWM_SetFrequency(uint32_t freq);

#endif // PWM_H
