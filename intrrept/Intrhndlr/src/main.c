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

//void __disable_irq(void);
//void __enable_irq(void); // Enable Interrupts

extern int EXTI0Flag = 0;
int ledStatus = 0x0100;

void delay(int counter)
{
	int i;
	for(i = 0; i < counter*1000; i++)
	{

	}

}


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
	 /* GPIOC GPIOA Periph clock enable */

	//__disable_irq();

	        RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	        RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	        RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

	        GPIOA->MODER &= ~(GPIO_MODER_MODER0);
	        GPIOB->MODER &= ~(GPIO_MODER_MODER0);
	        GPIOC->MODER |= (GPIO_MODER_MODER8_0);

	        GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8);
	        // Ensure push pull mode selected for output pins--default
	        GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR0);
	        GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR0);
	        GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8);
	       //Ensure maximum speed setting (even though it is unnecessary)

	        GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0);
	        GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR0);
	        GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8);

	        //Ensure all pull up pull down resistors are disabled PA0 is connected to external pulldown on STM32F0Discovery baord
	        SYSCFG->EXTICR[0]  = 0x0000; //0x0000 for PA0 and 0x0001 for PB0

	        //1. clear bits 3:0 in the SYSCFG_EXTICR1 reg to amp EXTI Line to NVIC for PA0

	         //   clear bits 3:1 in the SYSCFG_EXTICR1 reg to amp EXTI line to NVIC for PB0
	        EXTI->RTSR = EXTI_RTSR_TR0;
	         // 2.Set interrupt trigger to rising edge

	        EXTI->IMR = EXTI_IMR_MR0; // 3. unmask EXTI0 line
	      //  __enable_irq();
	        NVIC_SetPriority(EXTI0_1_IRQn, 1); //4. Set Priority to 1
	        NVIC_EnableIRQ(EXTI0_1_IRQn);  // 5. Enable EXTI0_1 interrupt in NVIC (do 4 first)

	        while(1)
	        {
	           if(EXTI0Flag)
	             {
	                EXTI0Flag = 0;
	                 GPIOC -> ODR ^= ledStatus; // Toggle the LED if you get here
	              }
	           else
	           {
	        	   GPIOC -> ODR ^= 0x00;
	           }

	            }


}
