#include "TM4C123GH6PM.h"

#define SAMPLE_MASK 0xFFF
#define DCOFFSET 0x7FF

void initfunc(){
/*CLOCK CONFIG*/
SYSCTL->RCGCGPIO |= (1<<1);
SYSCTL->RCGCADC |= 0x01;
	
/*Port configurations*/
GPIOB->DIR &= ~(1<<4);
GPIOB->DEN &= ~(1<<4);
GPIOB->AFSEL |= (1<<4);	
GPIOB->AMSEL |= (1<<4);
	
/*ATD configuration*/	
ADC0->ACTSS &= ~(1<<3);
ADC0->EMUX &= ~(0xF000);
ADC0->SSMUX3 = 0x0A;
ADC0->SSCTL3 |= ((1<<1)|(1<<2));
ADC0->PC = 0X01;
ADC0->ACTSS |= (1<<3);	
}

int main(){
	initfunc();
	int reading;
	
	
	while(1){
	ADC0->PSSI |= (1<<3);
	while ((ADC0->RIS & (1<<3)) == 0){}
	reading = ADC0->SSFIFO3 & SAMPLE_MASK;

	ADC0->ISC |= (1<<3);
	}
}

