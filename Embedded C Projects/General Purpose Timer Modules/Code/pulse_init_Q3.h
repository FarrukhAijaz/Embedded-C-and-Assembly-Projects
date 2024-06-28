/*Pulse_init.h file
Function for creating a pulse train using interrupts
Uses Channel 0, and a 1Mhz Timer clock (_TAPR = 15)
Uses Timer0A to create pulse train on PF2
*/

#include "TM4C123GH6PM.h"
void pulse_init(void);
void TIMER0A_Handler (void);
/*PRACTICAL VALUES*/
#define LOW 	27
#define HIGH 	18
/*THEORETICAL VALUES*/
/*
#define LOW 	30
#define HIGH 	20
*/
int state = 0;

void pulse_init(void){
	volatile int *NVIC_EN0 = (volatile int*) 0xE000E100;
	volatile int *NVIC_PRI4 = (volatile int*) 0xE000E410;
	SYSCTL->RCGCGPIO |= 0x22; // turn on bus clock for GPIOF
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
  GPIOF->DIR			|= 0x04; //set PF2 as output
  GPIOF->AFSEL		&= (0xFFFFFFFB);  // Regular port function
	GPIOF->PCTL			&= 0xFFFFF0FF;  // No alternate function
	GPIOF->AMSEL		=0; //Disable analog
	GPIOF->DEN			|=0x04; // Enable port digital
	GPIOF->DIR       |= 0x08; //set BLUE pin as a digital output pin
  GPIOF->DEN       |= 0x08;  // Enable PF3 pin as a digital pin
	//setting pb2 as input
	GPIOB->DIR			&= ~(1<<2); //set PB2 as input
	GPIOB->DIR			|= (1<<3); //set PB3 as output
	GPIOB->AFSEL		&= ~(0xFF);  // Regular port function
  GPIOB->AFSEL		|=(1<<2);  // Regular port function
	GPIOB->PCTL     &= ~0xFFFFFFFF; /* configure PB2 as T3CCP0 pin */
  GPIOB->PCTL     |= 0x00000700;
	GPIOB->AMSEL		=0; //Disable analog
	GPIOB->DEN			|=(1<<2); // Enable port digital
	GPIOB->DEN			|=(1<<3); // Enable port digital

	

	
	
	
	
	SYSCTL->RCGCTIMER	|=0x09; // Start timer0 and timer 3
	
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	TIMER0->CTL			&=0xFFFFFFFE; //Disable timer during setup
	TIMER0->CFG			=0x04;  //Set 16 bit mode
	TIMER0->TAMR		=0x02; // set to periodic, count down
	TIMER0->TAILR		=LOW; //Set interval load as LOW
	TIMER0->TAPR		=15; // Divide the clock by 16 to get 1us
	TIMER0->IMR			=0x01; //Enable timeout intrrupt
	
	TIMER3->CTL      &= ~0x01; //Disable timer during setup
	TIMER3->CFG			  =0x04;  //Set 16 bit mode
	TIMER3->TAMATCHR  = 0xFFFF;
	TIMER3->TAMR		  =0x17; // set to edge time count up
	TIMER3->TAPR		  =15; // Divide the clock by 16 to get 1us	
	TIMER3->CTL      |=0xC; //both edges
  TIMER3->CTL      |=0x03;	//enable and halt with debugger
	TIMER3->TAILR		  =LOW*5;
	
	//Timer0A is interrupt 19
	//Interrupt 16-19 are handled by NVIC register PRI4
	//Interrupt 19 is controlled by bits 31:29 of PRI4
	*NVIC_PRI4 &=0x00FFFFFF; //Clear interrupt 19 priority
	*NVIC_PRI4 |=0x40000000; //Set interrupt 19 priority to 2
	
	//NVIC has to be neabled
	//Interrupts 0-31 are handled by NVIC register EN0
	//Interrupt 19 is controlled by bit 19
	*NVIC_EN0 |=0x00080000;
	
	//Enable timer
	TIMER0->CTL			 |=0x03; // bit0 to enable and bit 1 to stall on debug
	return;
}

void TIMER0A_Handler (void){
	if (state == 0){
	GPIOF->DATA  &= ~0x04;  //toggle PF3 pin
	TIMER0->TAILR = LOW;
		
		state =1;
	}
	else{
	GPIOF->DATA  |= 0x04;  //toggle PF3 pin
	TIMER0->TAILR	= HIGH;
		state = 0;
	}
	TIMER0->ICR |=0x01; //Clear the interrupt
	return;
}