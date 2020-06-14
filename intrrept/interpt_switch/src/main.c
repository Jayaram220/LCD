/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

// led : pa5
// button: pc13

#include "stm32f0xx.h"
#include "stm32f0308_discovery.h"


void SystickDelayMS(int n)
{
	SysTick->LOAD = 16000; //reload with number of clocks per milliseconds
	SysTick->VAL = 0; // clear current value register
	SysTick->CTRL = 0x5; // Enable systick

	for(int i = 0; i<n; i++)
	{

		while((SysTick->CTRL & 10000)==0 )
		{
			
		}

	}
	SysTick->CTRL = 0;

}

void EXTI4_15_IRQhandler(void)
{
	GPIOA->BSRR = 0x20; // turn on led
	SystickDelayMS(300);
	GPIOA->BSRR = 0x002;// turn off led
	SystickDelayMS(300);

	GPIOA->BSRR = 0x20; // turn on led
	SystickDelayMS(300);
	GPIOA->BSRR = 0x002;// turn off led
	SystickDelayMS(300);


	EXTI->PR = 2000;

}


int main(void)
{
	//__disable_irq();

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //enable clock acess to porta
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; //enable clock acess to portc
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Enable syscfg clock
	GPIOA->MODER |= 0x400;    // 00|00 00|00 00|00 00|00 00|00 01|00 00|00 00|00
	GPIOA->BSRR = 0x20; // turn on led
	SYSCFG->EXTICR[3] = 0x0020; // select portc for ext13

	EXTI->IMR |= 0x2000; // unmask EXTI13
	EXTI->FTSR|= 0x2000; //Select falling edge trigger
	//SYSCFG_EXTICR4_EXTI13_PC;
	NVIC_EnableIRQ(EXTI4_15_IRQn);
	NVIC_SetPriority(EXTI4_15_IRQn,1);
	//__enable_irq();
	while(1)
	{

	}

}






