#include "TIMER.h"

/* Use timer 2 to generate an interrupt every second
void TIM2_Init(void){
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; // Enable TIMER 2 clock
	TIM2->CR1  &= ~TIM_CR1_DIR;           // Counting direction: 0 = up-counting, 1 = down-counting
    TIM2->PSC 	= 7999;                   // Prescaler < 65536.  Prescaler = 7999, timer clock 80MHz/(7999 + 1) = 10 KHz
    TIM2->ARR   = 10000 - 1;              // Auto-reload: Upcouting (0..ARR), Downcouting (ARR..0)
    TIM2->DIER |= TIM_DIER_UIE;           // Enable UIE interrupts
	TIM2->CR1  |= TIM_CR1_CEN;            // Enable counter

	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 1);
}*/
