#include "TM4C123GH6PM.h"
#include "Pulse_init.h"
#include <string.h>


extern void OutStr(char*);
int time_val[2];
int count=0;
int countt = 0;
char tempchar[100];
int pulse_width, distance;

void delay(){
	for (int i = 32000; i>0;i--){
	}}

int main(){


pulse_init();
	
while(1){
	
  GPIOB->DATA |=(1<<3);
	delay();
	GPIOB->DATA &= ~(1<<3);
	while ((TIMER3->RIS & (1<<2))==0);
	time_val[0] = TIMER3->TAR;
	TIMER3->ICR = 5;
	while ((TIMER3->RIS & (1<<2))==0);
	time_val[1] = TIMER3->TAR;
	TIMER3->ICR = 5;
	pulse_width = (time_val[1] - time_val[0])/16;
	distance = (pulse_width * 34)/200;
	sprintf(tempchar, "Pulse Width: %d, Distance: %d\r\4", pulse_width,distance);
	OutStr(tempchar);	
	
	


	}	
}   