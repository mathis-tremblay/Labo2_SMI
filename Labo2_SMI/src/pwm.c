#include "pwm.h"
#include "stm32f4xx.h"   // Def des registres STM32F429

// Freq de clock de base (à ajuster selon la config du RCC de ton projet)
#define SYS_CLOCK_FREQ 16000000UL

// Sauvegarde des valeurs courantes
static uint32_t current_freq = 1000;   // par défaut 1 kHz
static uint8_t current_duty = 50;      // par défaut 50%

void PWM_Init(void) {
    // 1. Activer clock du GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // 2. Mettre PA5 en mode Alternate Function (AF1 = TIM2_CH1)
    GPIOA->MODER &= ~(0x3 << (5 * 2));
    GPIOA->MODER |=  (0x2 << (5 * 2));   // Alternate Function

    GPIOA->AFR[0] &= ~(0xF << (5 * 4));
    GPIOA->AFR[0] |=  (0x1 << (5 * 4));  // AF1 = TIM2_CH1

    // 3. Activer la clock de TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // 4. Configurer le Timer2 pour PWM
    TIM2->PSC = (SYS_CLOCK_FREQ / (current_freq * 1000)) - 1; // prescaler
    TIM2->ARR = 1000 - 1;    // Periode fixer a 1000 pas (resolution duty cycle 0-1000)

    // 5. Configurer le canal 1 en mode PWM1
    TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM2->CCMR1 |= (0x6 << TIM_CCMR1_OC1M_Pos);  // PWM mode 1
    TIM2->CCMR1 |= TIM_CCMR1_OC1PE;              // Preload enable

    TIM2->CCER |= TIM_CCER_CC1E;   // Activer sortie canal 1

    // 6. Mettre un duty cycle par defaut
    TIM2->CCR1 = (current_duty * (TIM2->ARR + 1)) / 100;

    // 7. Activer le compteur
    TIM2->CR1 |= TIM_CR1_CEN;
}

void PWM_SetDutyCycle(uint8_t duty) {
    if (duty > 100) duty = 100;   // clamp 0–100%
    current_duty = duty;
    TIM2->CCR1 = (duty * (TIM2->ARR + 1)) / 100;
}

void PWM_SetFrequency(uint32_t freq) {
    if (freq == 0) return;
    current_freq = freq;

    // Reconfigurer le prescaler pour garder ARR = 999
    TIM2->PSC = (SYS_CLOCK_FREQ / (freq * 1000)) - 1;
}
