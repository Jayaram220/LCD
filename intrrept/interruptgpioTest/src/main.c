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

//'BUTTON_PIN' must be between PB0 or PB1, unless you want
#define BUTTON_PIN     (1)
	// (This should also be a GPIOB pin, but can be from 0-15)
#define ROTARY_PIN_B   (0)
	// (This should also be a GPIOB pin; pin B3 is the
	//  onboard LED for 'Nucleo-32' boards.)
#define LED_PIN        (3)
volatile uint8_t led_on;




 void EXTI0_1_IRQ_handler(void) {
	       // Check that EXTI7 is the line that triggered.
	       if (EXTI->PR & (1 << BUTTON_PIN)) {
	         // If it was, clear the interrupt flag.
	         EXTI->PR |= (1 << BUTTON_PIN);

	         // If you are using a button, toggle the LED state:
	         //led_on = !led_on;

	         // If you are using a rotary encoder, turn the
	         // LED on or off depending on the direction.
	         // (Comment this out if using a single button)
	         if (GPIOB->IDR & (1 << ROTARY_PIN_B)) {
	           // Turn the LED on.
	           led_on = 1;
	         }
	         else {
	           // Turn the LED off.
	           led_on = 0;
	         }
	       }
	     }


int main(void)
{
	led_on =0;

	// Enable the SYSCFG peripheral.
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	RCC->AHBENR   |= RCC_AHBENR_GPIOBEN;

	GPIOB->MODER  &= ~(0x3 << (BUTTON_PIN*2));
	  GPIOB->PUPDR  &= ~(0x3 << (BUTTON_PIN*2));
	  GPIOB->PUPDR  |=  (0x1 << (BUTTON_PIN*2));
	  GPIOB->MODER  &= ~(0x3 << (ROTARY_PIN_B*2));
	  GPIOB->PUPDR  &= ~(0x3 << (ROTARY_PIN_B*2));
	  GPIOB->PUPDR  |=  (0x1 << (ROTARY_PIN_B*2));
	  // B3 is connected to an LED on the 'Nucleo' board.
	  //    It should be set to push-pull low-speed output.
	  GPIOB->MODER  &= ~(0x3 << (LED_PIN*2));
	  GPIOB->MODER  |=  (0x1 << (LED_PIN*2));
	  GPIOB->OTYPER &= ~(0x1 << LED_PIN);
	  GPIOB->PUPDR  &= ~(0x3 << (LED_PIN*2));

	  SYSCFG->EXTICR[(BUTTON_PIN/4)] &= ~(0xF << ((BUTTON_PIN % 4) * 4));
	   SYSCFG->EXTICR[(BUTTON_PIN/4)] |=  (0x1 << ((BUTTON_PIN % 4) * 4));

	   EXTI->IMR |= (1 << BUTTON_PIN);

	   // Disable the 'rising edge' trigger (button release).
	    EXTI->RTSR &= ~(1 << BUTTON_PIN);
	    // Enable the 'falling edge' trigger (button press).
	    EXTI->FTSR |= (1 << BUTTON_PIN);
	    // Enable the NVIC interrupt at minimum priority.
	    NVIC_SetPriority(EXTI0_1_IRQn, 0x03);
	    NVIC_EnableIRQ(EXTI0_1_IRQn);

	    // Light the button only if it should be on.
	     while (1) {
	       if (led_on) {
	         GPIOB->ODR |= (1 << LED_PIN);
	       }
	       else {
	         GPIOB->ODR &= ~(1 << LED_PIN);
	       }
	     }




}
