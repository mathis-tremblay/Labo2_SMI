#include "controleurled.h"
#include "stm32f4xx.h"
#include "GPIO.h"
#include "pwm.h"
#include "adc.h"
#include "delai.h"

// Pins utilisées
#define BTN_PORT    GPIOA
#define BTN_PIN     0    // Bouton bleu
#define ADC_PIN_PORT GPIOC
#define ADC_PIN     3    // PC3 (ADC channel 13)
#define ADC_INSTANCE ADC1
#define ADC_CHANNEL  13

// Valeur ADC max (12 bits)
#define ADC_MAX      4095U

// Map 0-4095 vers 0-100
static uint8_t map_adc_to_duty(uint32_t adc_value) {
    if (adc_value >= ADC_MAX) return 100;
    return (uint8_t)((adc_value * 100U) / ADC_MAX);
}

void ControleurLED_Init(void) {
    // 1) Initialise le systick
    SysTick_Init(9000);

    // 2) Configurer pin du potentiometre en mode analogique (PC3)
    GPIO_Config(ADC_PIN_PORT, ADC_PIN, 3, 0, 0, 0);

    // 3) Configurer l'ADC1 channel 13
    ADC_Config(ADC_INSTANCE, ADC_CHANNEL);

    // 4) Initialiser le PWM et sa pin (PA5)
    GPIO_Config(GPIOA, 5, 2, 0, 2, 1);
    PWM_Init();

    // 5) Configurer le bouton en entrée
    GPIO_Config(BTN_PORT, BTN_PIN, 0 /* input */, 0 /* pull-down */, 0 /* speed */, 0 /* af mode*/);

    // Init led fermé
    PWM_SetDutyCycle(0);
}

void ControleurLED_On(void) {
    PWM_SetDutyCycle(100);
}

void ControleurLED_Off(void) {
    PWM_SetDutyCycle(0);
}

void ControleurLED_UpdateFromADCAndApply(void) {
    // Demande une conversion puis applique
    ADC_StartConvert(ADC_INSTANCE);

    while (!ADC_IsReady()) {
    }

    uint32_t sample = ADC_GetSample(ADC_INSTANCE); // lire valeur recuperee
    uint8_t duty = map_adc_to_duty(sample); // 0 a 100
    PWM_SetDutyCycle(duty); // appliquer
}
