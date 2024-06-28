#include "TM4C123GH6PM.h" // Include the header file for the TM4C123GH6PM microcontroller.

#define SAMPLE_MASK 0xFFF // Define a constant SAMPLE_MASK with the value 0xFFF (4095 in decimal).
#define DCOFFSET 0x00     // Define a constant DCOFFSET with the value 0x00 (zero).

int high = 27;   // Declare and initialize an integer variable 'high' with the value 27.
int low = 18;    // Declare and initialize an integer variable 'low' with the value 18.
int state = 0;   // Declare and initialize an integer variable 'state' with the value 0.

void TIMER0A_Handler(void); // Declare a function prototype for the TIMER0A interrupt handler.
void pulse_init(void);     // Declare a function prototype for the 'pulse_init' function.

// Initialization of GPIO, Timer, and NVIC for generating pulses.
void pulse_init(void) {
    volatile int *NVIC_EN0 = (volatile int*) 0xE000E100; // Define a pointer to NVIC_EN0 register.
    volatile int *NVIC_PRI4 = (volatile int*) 0xE000E410; // Define a pointer to NVIC_PRI4 register.
    
    SYSCTL->RCGCGPIO |= 0x20; // Turn on bus clock for GPIOF.
    __ASM("NOP");
    __ASM("NOP");
    __ASM("NOP");
    
    GPIOF->DIR |= 0x08;      // Set PF2 as an output.
    GPIOF->AFSEL &= (0xFFFFFFFB);  // Regular port function for PF2.
    GPIOF->PCTL &= 0xFFFF0FFF;    // No alternate function for PF2.
    GPIOF->AMSEL = 0;             // Disable analog mode for PF2.
    GPIOF->DEN |= 0x08;           // Enable digital functionality for PF2.
    
    SYSCTL->RCGCTIMER |= 0x01; // Start Timer 0.
    __ASM("NOP");
    __ASM("NOP");
    __ASM("NOP");
    
    TIMER0->CTL &= 0xFFFFFFFE; // Disable Timer 0 during setup.
    TIMER0->CFG = 0x04;       // Set Timer 0 in 16-bit mode.
    TIMER0->TAMR = 0x02;      // Set Timer 0 to periodic, count down mode.
    TIMER0->TAILR = low;      // Set Timer 0 interval load as 'low'.
    TIMER0->TAPR = 63;        // Divide the clock by 16 to get 1us.
    TIMER0->IMR = 0x01;       // Enable Timer 0 timeout interrupt.
    
    // Timer0A is interrupt 19. Priority settings for interrupts 16-19 are handled by NVIC register PRI4.
    // Interrupt 19 is controlled by bits 31:29 of PRI4.
    *NVIC_PRI4 &= 0x00FFFFFF; // Clear interrupt 19 priority.
    *NVIC_PRI4 |= 0x40000000; // Set interrupt 19 priority to 2.
    
    // NVIC has to be enabled. Interrupts 0-31 are controlled by NVIC register EN0. 
    // Interrupt 19 is controlled by bit 19.
    *NVIC_EN0 |= 0x00080000;
    
    // Enable Timer 0.
    TIMER0->CTL |= 0x03; // Bit 0 to enable and bit 1 to stall on debug.
    return;
}

// Delay function using a simple loop.
void delay() {
    for (int i = 0; i <= 160000; i++) {
        ;
    }
}

// Function to scale the ADC reading to 1000.
int scaledval(int reading) {
    int s_integer = ((reading * 40) / 4095); // Scale the reading to 1000.
    return s_integer;
}

// Function to set PWM values.
void setpwm(int val) {
    high = val;
    low = 45 - val;
}

// Initialize clock, GPIO, and ADC configurations.
void initfunc() {
    /* CLOCK CONFIG */
    SYSCTL->RCGCGPIO |= (1 << 1);
    SYSCTL->RCGCADC |= 0x01;
    
    /* Port configurations */
    GPIOB->DIR &= ~(1 << 4);
    GPIOB->DEN &= ~(1 << 4);
    GPIOB->AFSEL |= (1 << 4);
    GPIOB->AMSEL |= (1 << 4);
    
    /* ADC configuration */
    ADC0->ACTSS &= ~(1 << 3);
    ADC0->EMUX &= ~(0xF000);
    ADC0->SSMUX3 = 0x0A;
    ADC0->SSCTL3 |= ((1 << 1) | (1 << 2));
    ADC0->PC = 0X01;
    ADC0->ACTSS |= (1 << 3);
}

int main() {
    initfunc(); // Call the initialization function.
    pulse_init(); // Call the pulse initialization function.
    int reading;
    int final_reading;
    int scaled;
    
    while (1) {
        ADC0->PSSI |= (1 << 3);
        while ((ADC0->RIS & (1 << 3)) == 0) {}
        reading = ADC0->SSFIFO3 & SAMPLE_MASK;
        final_reading = reading;
        scaled = scaledval(final_reading);
        setpwm(scaled);
        ADC0->ISC |= (1 << 3);
        delay();
    }
}

// Interrupt handler for Timer0A.
void TIMER0A_Handler(void) {
    if (state == 0) {
        GPIOF->DATA &= ~0x08;  // Toggle PF3 pin.
        TIMER0->TAILR = low;
        state = 1;
    } else {
        GPIOF->DATA |= 0x08;  // Toggle PF3 pin.
        TIMER0->TAILR = high;
        state = 0;
    }
    TIMER0->ICR |= 0x01; // Clear the interrupt.
    return;
}
