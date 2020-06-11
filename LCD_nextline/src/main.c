/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

//pc0 - pc7 -- D0 - D7
// PB5 -- RS
//PB6 --- R/W
//PB7 --- EN

#include "stm32f0xx.h"
#include "stm32f0308_discovery.h"

#define RS 0x20 // 0010 0000 -- PB5
#define RW 0x40 // 0100 0000 --- PB6
#define EN 0x80 // 1000 0000 --- PB7

void delayMs(int delay)
{
	int i;
	for(i = 0; i < delay*1000; i++)
	{

	}

}



void Gpio_Init(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN; // enable GPIOB clock
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // enable GPIOC clock
	GPIOB->MODER |= 0x5400; // 0101 0100 0000 0000
	GPIOB->BSRR = 0x00C; // set EN and RW low ie, 22,23  -- 0000 0000 1100 0000 0000 0000 0000 0000
	//set PC0 - PC7 as output

	GPIOC->MODER |= 0x00005555; // 0000 0000 0000 0000 0101 0101 0101 0101

}

void LCD_command(unsigned char command)
{
	GPIOB->BSRR = (RS|RW)<<16; // RS = 0, RW=0
	GPIOC->ODR = command; // data is supplied to the data.
	GPIOB->BSRR = EN; // EN: E is known as the enable pin. It Is used by the LCD to secure information.
	// When data is supplied to the data pins a High-Low pulse must be applied
	delayMs(10);
	GPIOB->BSRR = EN <<16; // Clear EN pin
}

void LCD_data(char data)
{
	GPIOB->BSRR = RS; // RS = 1
	GPIOB->BSRR = RW <<16;  // R/W = 0
	GPIOC->ODR = data;
	GPIOB->BSRR = EN;
	delayMs(300);
	GPIOB->BSRR = EN <<16; // Clear EN pin

}

void LCD_Init(void)
{
	Gpio_Init(); //Init Gpio sequence
	delayMs(40);
	LCD_command(0x30); //  means we are setting 8-bit mode lcd having 1 line and we are initializing it to be 5×7 character display
	delayMs(20);
	LCD_command(0x30);
	delayMs(10);
	LCD_command(0x30);


	LCD_command(0x38); // set 8-bit data mode, 2 line, 5*7 font
	LCD_command(0x06); // move cursor right or Auto Increment cursor

	LCD_command(0x01); // clear screen, move cursor home
	LCD_command(0x0F); // turn display, blink cursor
}


void mydisplay(int val1) {
	char val_str[8] = {0};
	int i = 0;
    char Max_val1_str[8]="500";


	sprintf(val_str, "%d", val1);
	//LCD_command(0xC0);
	for ( i= 0; i < strlen(val_str); i++) {
		LCD_data(val_str[i]);
	}
	LCD_data('/');

	for ( i= 0; i < strlen(Max_val1_str); i++) {
			LCD_data(Max_val1_str[i]);
		}
	//LCD_command(1);
}

int main(void)
{

	LCD_Init();
	delayMs(40);
	LCD_command(0x01); // clear screen, move cursor home
	mydisplay(125);



}

