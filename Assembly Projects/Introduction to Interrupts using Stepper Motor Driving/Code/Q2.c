#include "TM4C123GH6PM.h"
#define BUTTONNUMBER 4
#define INPUTMASK 0x0F
int mcount=0;

void initfunc(void)
{
	SYSCTL->RCGCGPIO |= 0x2F; // turn on bus clock for GPIOB
	GPIOB->DIR       |= 0xF0; // set PB4-PB7 pin as a digital output pin
	GPIOB->DEN       |= 0xFF; // Enable PB0-PB7 pin as a digital pin
	GPIOB->AFSEL     &=~0xFF; // Disable Alternate Function 
	SysTick->LOAD     = 5000; // one milli second delay relaod value
	SysTick->CTRL     = 3;    // enable counter , interrupt and select system bus clock
	SysTick->VAL      = 0; 	  // initialize current value register
}
void SysTick_Handler(void) 
{
	 mcount++;
}

unsigned char getbutton(void)
{
	unsigned char input;
	return GPIOB->DATA & INPUTMASK;
}
	
	
	
unsigned char debounced_button()
{
  unsigned char button,prevbutton;
	button = 0x8F;
	while(1)
	{
		int syscount;
		prevbutton = getbutton();
		syscount = mcount;
	
		while(syscount == mcount)
		{
			button = getbutton();
		}
		if (button == prevbutton)
		{
			return button;
		}
	}
	}


void stepfulldir(int dir)
{
	if (dir==1) // Check for Clockwise or Anticlockwise
	{
		if (mcount == 7) // Check for count and set corresponding LED high and all others LOW
		{
			GPIOB->DATA &= ~0xF0;
			GPIOB->DATA |= 0x10;
		}
		if (mcount == 5)
		{
			GPIOB->DATA &= ~0xF0;
			GPIOB->DATA |= 0x20;
		}
		if (mcount == 3)
		{
			GPIOB->DATA &= ~0xF0;
			GPIOB->DATA |= 0x40;

		}
		if (mcount == 1)
		{
			GPIOB->DATA &= ~0xF0;
			GPIOB->DATA |= 0x80;
		}
		if (mcount > 8)
		{
			mcount = 0;
		}
	}
	else
	{
		if (mcount == 1) // Check for count and set corresponding LED high and all others LOW
		{
			GPIOB->DATA &= ~0xF0;
			GPIOB->DATA |= 0x10;
		}
		if (mcount == 3)
		{
			GPIOB->DATA &= ~0xF0;
			GPIOB->DATA |= 0x20;
		}
		if (mcount == 5)
		{
			GPIOB->DATA &= ~0xF0;
			GPIOB->DATA |= 0x40;
		}
		if (mcount == 7)
		{
			GPIOB->DATA &= ~0xF0;
			GPIOB->DATA |= 0x80;
		}
		if (mcount > 8)
		{
			mcount = 0;
		}
	}
}
int main()
{
	initfunc();
	while(1) // Run Continuously 
	{
		unsigned char real_button;
		real_button = debounced_button();
		if (real_button == 0x0D)
		{
			stepfulldir(1); // 1 for Clockwise , 0 for Anti Clock wise
		}
		if (real_button == 0x0E)
		{
			stepfulldir(0); // 1 for Clockwise , 0 for Anti Clock wise
		}
	}
}
