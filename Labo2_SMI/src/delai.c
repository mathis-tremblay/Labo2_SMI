#include "delai.h"

extern volatile uint64_t millis_count;

/*
 * RVR (LOAD) : nombre a partir du lequel la clock decremente (9000=1ms)
 * CVR (VAL) : Current value du compteur
 * CSR (CTRL) : Active interruptions TICKINT, et Enable
*/
void SysTick_Init(uint64_t load_value){
	SysTick->LOAD |= load_value;

	SysTick->VAL |= 0;

	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

uint64_t millis(){
	return millis_count;
}

void delai(uint64_t wait_millis){
	uint64_t millis_target = millis() + wait_millis;
	while(millis() < millis_target);
}

void SysTick_Handler(void){
	millis_count = millis_count + 1;
}
