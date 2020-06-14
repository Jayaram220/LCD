/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f0xx.h"
#include "stm32f0308_discovery.h"
extern int EXTI0Flag = 0;


void EXTI0_1_IRQHandler(void)
{

    if( (EXTI->IMR & EXTI_IMR_MR0) && (EXTI->PR & EXTI_PR_PR0))
      {
         EXTI0Flag = 1;
         //delay(500);
         while(GPIOA->IDR & GPIO_IDR_0){} // Wait for the button
         EXTI->PR |= EXTI_PR_PR0 ; // Clear the flag
        }
}



int main(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // enable GPIOA clock (131072) enable 17 th bit
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	//RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // enable GPIOC clock
	GPIOC->MODER |= 0x40000;
	//GPIOC->MODER |= 0x40000;    // 00|00 00|00 00|00 01|00 00|00 00|00 00|00 00|00


	//GPIOC->OTYPER &= ~(0x00000200);
	GPIOC->OTYPER &= (0xFFFFFDFF);

	GPIOA->OSPEEDR |= (0x00000003);
	GPIOC->OSPEEDR |= (0x000C0000);
	GPIOA->PUPDR &= ~(0x00000003);
	 GPIOC->PUPDR &= ~(0x000C0000);

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Enable syscfg clock

	SYSCFG->EXTICR[0]  = 0x0000; //0x0000 for PA0
	EXTI->RTSR = EXTI_RTSR_TR0;
	EXTI->IMR = EXTI_IMR_MR0;
	NVIC_SetPriority(EXTI0_1_IRQn, 1); //4. Set Priority to 1
	NVIC_EnableIRQ(EXTI0_1_IRQn);  // 5. Enable EXTI0_1 interrupt in NVIC (do 4 first)

	while(1)
	{
		//if(GPIOC->IDR & 0x2000)// if pc13 is high
		//if(GPIOA->IDR & 0x001)
		if(EXTI0Flag)
		{
			EXTI0Flag = 0;
			GPIOC->ODR ^= 0x200; // turn on led
		}
	/*	else
		{

			GPIOC->ODR = 0x00000;// turn off led
		}
		*/

	}


}
