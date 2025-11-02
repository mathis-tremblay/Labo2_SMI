#include "delai.h"

extern volatile uint64_t millis_count;

/*
 * RVR (LOAD) : nombre a partir du lequel la clock decremente (9000=1ms)
 * CVR (VAL) : Current value du compteur
 * CSR (CTRL) : Active interruptions TICKINT, et Enable
*/
void SysTick_Init(uint32_t sysclk_hz)
{
    // Arrêter le SysTick pendant la config
    SysTick->CTRL = 0;

    // Reload pour 1 ms : (Fsys/1000) - 1
    // Exemple: 168 MHz -> 168000 - 1 = 167999
    uint32_t reload = (sysclk_hz / 1000u) - 1u;
    SysTick->LOAD = reload;

    // Clear le compteur courant
    SysTick->VAL = 0;

    // CLKSOURCE = 1 (HCLK), TICKINT = 1 (IRQ enable), ENABLE = 1
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk
                  | SysTick_CTRL_TICKINT_Msk
                  | SysTick_CTRL_ENABLE_Msk;

}

uint64_t millis(){
	return millis_count;
}

void delai(uint64_t wait_ms)
{
    uint64_t target = millis() + wait_ms;
    while (millis() < target) {
        __NOP();
    }
}

void SysTick_Handler(void){
	millis_count = millis_count + 1;
}
