#include "state.h"
#include "EXTI.h"

// Joystick Center = PA0
// Joystick Up = PA3
// Joystick Down = PA5
void EXTI_Init(void){
	// GPIO Configuration
	RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOAEN;
	
	// GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~3U << 6; // UP
	GPIOA->MODER &= ~3U << 10; // DOWN
	GPIOA->MODER &= ~3U << 0; // CENTER

	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)	
	
	// The following is important: We have to pull-down PA.3. Otherwise it is float.
	// We do not need to pull down PA.0 and PA.5 because it has already been pulled down externally. Take a look at the schematics.
	GPIOA->PUPDR |= 2U << 6; // UP
	GPIOA->PUPDR |= 2U << 10; // DOWN
	GPIOA->PUPDR &= ~3U << 0; // CENTER

	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
	
	// EXIT Interrupt Enable
	// UP
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_SetPriority(EXTI3_IRQn, 0);
	
	// DOWN
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	NVIC_SetPriority(EXTI9_5_IRQn, 0);

	// CENTER
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_SetPriority(EXTI0_IRQn, 0);

	// Connect External Line to the GPI
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// UP
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3;     // SYSCFG external interrupt configuration registers
	SYSCFG->EXTICR[0] |=  SYSCFG_EXTICR1_EXTI3_PA;
	
	// DOWN
	SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI5;     // SYSCFG external interrupt configuration registers
	SYSCFG->EXTICR[1] |=  SYSCFG_EXTICR2_EXTI5_PA;

	// UP
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;     // SYSCFG external interrupt configuration registers
	SYSCFG->EXTICR[0] |=  SYSCFG_EXTICR1_EXTI0_PA;

	// Interrupt Mask Register // 0 = marked, 1 = not masked (i.e., enabled)
	EXTI->IMR1   |= EXTI_IMR1_IM3; //UP
	EXTI->IMR1   |= EXTI_IMR1_IM5; //DOWN
	EXTI->IMR1   |= EXTI_IMR1_IM0; //CENTER
	
	// Software interrupt event register
	// EXTI->SWIER1 |= EXTI_SWIER1_SWI3;
	
	// Rising trigger selection // 0 = trigger disabled, 1 = trigger enabled
	EXTI->RTSR1 |= EXTI_RTSR1_RT3; // UP
	EXTI->RTSR1 |= EXTI_RTSR1_RT5; // DOWN
	EXTI->RTSR1 |= EXTI_RTSR1_RT0; // CENTER
	// EXTI->FTSR1 |= EXTI_FTSR1_FT3;     // 0 = trigger disabled, 1 = trigger enabled
}

void EXTI3_IRQHandler(void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF3) == EXTI_PR1_PIF3) {
		// cleared by writing a 1
		EXTI->PR1 |= EXTI_PR1_PIF3;
		//theSTATE = RED_FLASH;
	}
}

void EXTI0_IRQHandler(void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF0) == EXTI_PR1_PIF0) {
		// cleared by writing a 1
		EXTI->PR1 |= EXTI_PR1_PIF0;
		for(int i = 0; i<4; i++)
		{
			Green_LED_On();
		}
		theSTATE = SEND_DATA;
	}
}

void EXTI9_5_IRQHandler(void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF5) == EXTI_PR1_PIF5) {
		// cleared by writing a 1
		EXTI->PR1 |= EXTI_PR1_PIF5;

		//theSTATE = GREEN_FLASH;
	}
}
