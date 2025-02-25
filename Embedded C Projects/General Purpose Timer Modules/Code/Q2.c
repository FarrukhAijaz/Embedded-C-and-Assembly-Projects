#include "TM4C123GH6PM.h"
#include "Pulse_init.h"
#include <string.h>


extern void OutStr(char*);


int main()
{
	int time_val[3];
	time_val[2] = 123;
	int count=0;
	int countt = 0;
	char tempchar[100];
	int pulse_width, period;
	int duty_cycle;
	pulse_init();
	while(1)
	{
		if (count > 2)
		{
			pulse_width = (time_val[1] - time_val[0])/16;
			period = (time_val[2] - time_val[0])/16;
			duty_cycle = ((pulse_width*100)/period);
			if (countt == 0)
			{
				sprintf(tempchar, "Pulse Width: %d, Period: %d, Duty Cycle: %d\r\4", pulse_width, period, duty_cycle);
				OutStr(tempchar);
				countt = 1;
			}
			else if (countt == 1)
			{
				countt =0;
			}
			count = 0;
		}
		if (TIMER3->RIS & (1<<2))
		{
			time_val[count] = TIMER3->TAR;
			TIMER3->ICR = 5;
			count++;
		}

	}	
}   