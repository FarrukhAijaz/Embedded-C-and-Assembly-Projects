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
void initspeedslow(void) // Increasing speed function by loading twice the value in Systick
{
	SysTick->LOAD     = 8000; 
	SysTick->CTRL     = 3; 
	SysTick->VAL      = 0;
}
void initspeedfast(void)
{
	SysTick->LOAD     = 4000; // Increasing speed function by loading half the value in Systick
	SysTick->CTRL     = 3; 
	SysTick->VAL      = 0;
}


unsigned char getbutton(void) // function for getting push button input
{
	unsigned char input;
	return GPIOB->DATA & INPUTMASK;
}
	
unsigned char debounced_button() // debouncing problem solving
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

unsigned char released_button() // function to check whether the button has released or not
{
unsigned char button,release;
	button = debounced_button();
	while(1)
	{
		release = getbutton();
		if (release == 0x0F) // by comparing all swithces to high
		{
			return button;
		}
	}
}


void stepfulldir(int dir)
{
	if (dir==1) // Check for Clockwise or Anticlockwise
	{
		if (mcount == 7)  // Check for count and set corresponding LED high and all others LOW
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
		if (mcount == 1)
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
	int trigger = 2;
	while(1)
	{
 		unsigned char real_button;
		real_button = released_button();
		if (real_button == 0x07)
		{
			initspeedfast(); // increase the speed
		}
		if (real_button == 0x0B)
		{
			initspeedslow(); // decrease the speed
		}
		if (real_button == 0x0D)
		{
			trigger = 1; // run the motor in Clockwise 
		}
		if (real_button == 0x0E)
		{
			trigger = 0; // run the motor in anticlockwise
		}
		if (trigger != 2)
		{
			stepfulldir(trigger);
		}
	}
}
