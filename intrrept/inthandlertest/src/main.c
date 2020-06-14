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


int main(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // enable GPIOA clock (131072) enable 17 th bit
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	//RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // enable GPIOC clock
	GPIOC->MODER |= 0x40000;
	//GPIOC->MODER |= 0x40000;    // 00|00 00|00 00|00 01|00 00|00 00|00 00|00 00|00
	while(1)
	{
		//if(GPIOC->IDR & 0x2000)// if pc13 is high
		if(GPIOA->IDR & 0x001)
		{
			GPIOC->ODR = 0x200; // turn on led
		}
		else
		{

			GPIOC->ODR = 0x00000;// turn off led
		}

	}


}
