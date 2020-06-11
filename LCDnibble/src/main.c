/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


// PC4 - PC7 : Data lines
// PB5 -- RS
//GND -- RW
//PB7 -- EN

#include "stm32f0xx.h"
#include "stm32f0308_discovery.h"

#define RS 0x20 // 0010 0000 -- PB5
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
	GPIOB->MODER |= 0x0004400;  //                0100 0100 0000 0000   -- o/p as pb5 , pb7
	GPIOB->BSRR = 0x00800000; // CLEAR EN 23 opposite of 7
	//set PC0 - PC7 as output

	GPIOC->MODER |= 0x00005500; // 0000 0000 0000 00|00 01|01 01|01 00|00 00|00
}

// control means wheather we are writing command or data
// data means data
void LCD_nibble_Write(char data,unsigned char control)
{
	GPIOC->BSRR = 0x00F00000; // Clear data bits
	GPIOC->BSRR = 0x00F00000; // Clear data bits
	GPIOC->BSRR = data & 0xF0;
	if(control & RS)
	{
		GPIOB->BSRR = RS; // RS = 1

	}
	else
	{
		GPIOB->BSRR = RS <<16;
	}
	// PULSE en
	GPIOB->BSRR = EN;
	delayMs(10);
	GPIOB->BSRR = EN<<16;

}

void LCD_command(unsigned char command)
{
	LCD_nibble_Write(command & 0XF0,0); // UPPER 4 BITS
	LCD_nibble_Write(command << 4, 0); // LoweR 4 BITS

	if(command <4)
	{
		delayMs(10);
	}
	else
	{
		delayMs(10);
	}

}

void LCD_data(char data)
{
	LCD_nibble_Write(data & 0XF0,RS);
	LCD_nibble_Write(data <<4,RS);
	delayMs(10);
}

void LCD_Init(void)
{
	Gpio_Init();
	delayMs(10);
	LCD_nibble_Write(0X30,0);
	delayMs(10);
	LCD_nibble_Write(0X30,0);
	delayMs(10);
	LCD_nibble_Write(0X30,0);
	delayMs(10);

	LCD_nibble_Write(0X20,0); // SET 4 BIT DATA MODE
	delayMs(10);
	LCD_command(0x28); // 4-bit mode, 2- line, 5*7 font
	LCD_command(0x06); // move curser right
	LCD_command(0x01); // clear screen, move cursor home
	LCD_command(0x0F); // turn on display and blink cursor

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

	//LCD_data('H');
	//LCD_command(0x01); // clear screen, move cursor home
		//LCD_command(0x0F); // turn on display and blink cursor
		mydisplay(130);
	/*	while(1)
		{
			LCD_data('H');
			LCD_data('E');
			LCD_data('L');
			LCD_data('L');
			LCD_data('O');
			delayMs(10);
			LCD_command(0x01); // clear screen;
			delayMs(10);
		} */

}
