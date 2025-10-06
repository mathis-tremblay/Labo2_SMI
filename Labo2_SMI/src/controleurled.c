#include "controleurled.h"
#include "stm32f4xx.h"
#include "GPIO.h"
#include "pwm.h"
#include "adc.h"
#include "delai.h"

/* Broches / périphériques utilisés (ajuster si nécessaire) */
#define BTN_PORT    GPIOA
#define BTN_PIN     0    /* Bouton bleu */
#define ADC_PIN_PORT GPIOC
#define ADC_PIN     3    /* PC3 -> ADC channel 13 */
#define ADC_INSTANCE ADC1
#define ADC_CHANNEL  13

/* Valeur ADC max (12 bits) */
#define ADC_MAX      4095U

/* Internal helper : map 0..4095 -> 0..100 */
static uint8_t map_adc_to_duty(uint32_t adc_value) {
    if (adc_value >= ADC_MAX) return 100;
    /* multiplication avant division pour meilleure précision */
    return (uint8_t)((adc_value * 100U) / ADC_MAX);
}

void ControleurLED_Init(void) {
    /* 1) Initialise le SysTick pour pouvoir utiliser delai() si nécessaire
       (valeur 9000 supposée donner 1ms d'après le labo). */
    SysTick_Init(9000);

    /* 2) Configurer la broche du potentiomètre en mode analogique (PC3) */
    GPIO_Config(ADC_PIN_PORT, ADC_PIN, 3, 0 , 0);

    /* 3) Configurer l'ADC (ADC1, channel correspondant à PC3) */
    ADC_Config(ADC_INSTANCE, ADC_CHANNEL);

    /* 4) Initialiser le PWM (met PA5 en AF, TIM2, etc.) */
    PWM_Init();

    /* 5) Configurer le bouton en entrée avec pull-down (on suppose bouton actif haut) */
    GPIO_Config(BTN_PORT, BTN_PIN, 0 /* input */, 2 /* pull-down */, 0 /* speed */);

    /* Par défaut, éteindre la LED (duty = 0) */
    PWM_SetDutyCycle(0);
}

void ControleurLED_On(void) {
    /* On ne 'démarre' pas le timer ici (PWM_Init l'a déjà), on règle le duty */
    /* Par défaut on met 100% (plein) ; typiquement on utilisera SetIntensityFromADC */
    PWM_SetDutyCycle(100);
}

void ControleurLED_Off(void) {
    PWM_SetDutyCycle(0);
}

void ControleurLED_SetIntensityFromADC(uint32_t adc_value) {
    uint8_t duty = map_adc_to_duty(adc_value);
    PWM_SetDutyCycle(duty);
}

void ControleurLED_UpdateFromADCAndApply(void) {
    /* Demande une conversion puis attend l'échantillon et applique */
    ADC_StartConvert(ADC_INSTANCE);

    /* attendre que l'ADC signale que l'échantillon est prêt (blocking) */
    while (!ADC_IsReady()) {
        // petit délai optionnel
        // delai(1);
    }

    uint32_t sample = ADC_GetSample(ADC_INSTANCE);   // argument ajouté ici
    ControleurLED_SetIntensityFromADC(sample);
}
