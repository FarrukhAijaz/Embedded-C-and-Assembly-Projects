#include "TM4C123GH6PM.h"

#define SAMPLE_MASK 0xFFF
#define DCOFFSET 0x7FF

/******************************************************************************************************
 * @brief scales the received integer and turns to char array
 *
 * we dont have floating point precission in this processor so we first
 * scale to 165 and then break that into each digit and then use that to get
 * 1.65 stored in a char array
 * @param reading integer value between -2048 to 2048
 * @param result char array which is used to store the return value i.e. the returned array scaled
 * ...
 * @return void since value passed as pointer to function
 ******************************************************************************************************/
void scaledval(int reading, char*result){
	int neg = 0;
	int s_integer = (reading*165)/2048; //this is to scale the reading between -165 to 165
	if (s_integer < 0) {
    s_integer = -s_integer;
    neg = 1;    
    } else {
        neg = 0;
    }
	int hundreds = s_integer / 100; //extract each digit of the number seperately
  int remainder = s_integer % 100;
  int tens = remainder / 10;
  int ones = remainder % 10;
	if (neg){
	result[0] = '-';
	}
  else{
	result[0] = ' ';
	}	
	result[1] = (char)('0' + hundreds);
  result[2] = '.';
  result[3] = (char)('0' + tens);
  result[4] = (char)('0' + ones);
  result[5] = '\0'; // Null-terminate the string 
	
	

}
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
	signed int final_reading;
	char scaled[6];
	
	while(1){
	ADC0->PSSI |= (1<<3);
	while ((ADC0->RIS & (1<<3)) == 0){}
	reading = ADC0->SSFIFO3 & SAMPLE_MASK;
  final_reading = DCOFFSET - reading;
	scaledval(final_reading,scaled);
	ADC0->ISC |= (1<<3);
	}
}

