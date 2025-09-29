#include "stm32f4xx.h"

#ifndef DELAI_H_
#define DELAI_H_

void SysTick_Init(uint64_t load_value);

uint64_t millis();

void delai(uint64_t wait_millis);

void SysTick_Handler(void);

#endif /* DELAI_H_ */
