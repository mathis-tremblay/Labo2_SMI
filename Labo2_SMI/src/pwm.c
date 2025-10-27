#include "pwm.h"
#include "stm32f4xx.h"   // Def des registres STM32F429

// Freq de clock de base (Trouver empiriquement avec oscilloscope)
#define SYS_CLOCK_FREQ 72000000UL

// Sauvegarde des valeurs courantes
static uint32_t current_freq = 1000;   // par dÃ©faut 1 kHz
static uint8_t current_duty = 50;      // par dÃ©faut 50%

void PWM_Init(void) {
    // 1. Activer la clock du TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // 2. Configurer le Timer2 pour PWM
    TIM2->PSC = (SYS_CLOCK_FREQ / (current_freq * 1000)) - 1;  // Prescaler
    TIM2->ARR = 1000 - 1;  // Période = 1000 (résolution duty cycle 0–1000)

    // 3. Configurer le canal 1 en mode PWM1
    TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM2->CCMR1 |= (0x6 << 4);           // PWM mode 1
    TIM2->CCMR1 |= TIM_CCMR1_OC1PE;      // Preload enable

    // 4. Activer la sortie du canal 1
    TIM2->CCER |= TIM_CCER_CC1E;

    // 5. Définir le duty cycle par défaut
    TIM2->CCR1 = (current_duty * (TIM2->ARR + 1)) / 100;

    // 6. Activer le compteur
    TIM2->CR1 |= TIM_CR1_CEN;
}

void PWM_SetDutyCycle(uint8_t duty) {
    if (duty > 100) duty = 100;   // clamp 0â€“100%
    current_duty = duty;
    TIM2->CCR1 = (duty * (TIM2->ARR + 1)) / 100;
}

void PWM_SetFrequency(uint32_t freq) {
    if (freq == 0) return;
    current_freq = freq;

    // Reconfigurer le prescaler pour garder ARR = 999
    TIM2->PSC = (SYS_CLOCK_FREQ / (freq * 1000)) - 1;
}
