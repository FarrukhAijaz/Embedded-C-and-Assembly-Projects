#include "TM4C123GH6PM.h"  // Include the header file for TM4C123GH6PM microcontroller
#include "Pulse_init.h"    // Include Pulse initialization header
#include <string.h>        // Include the string manipulation library

extern void OutStr(char*); // External function declaration

int time_val[2];          // Array to hold time values
int count = 0;            // Counter variable
int countt = 0;           // Another counter variable
char tempchar[100];       // Character array to store temporary strings
int pulse_width, distance; // Variables to hold pulse width and distance

// Function to introduce a delay
void delay() {
    for (int i = 32000; i > 0; i--) {
        // Intentionally empty loop to create delay
    }
}

int main() {
    pulse_init(); // Initialize pulse_init
    
    while (1) { // Infinite loop for continuous operation
        
        GPIOB->DATA |= (1 << 3); // Set pin 3 of GPIOB high
        delay();                 // Introduce a delay
        GPIOB->DATA &= ~(1 << 3); // Clear pin 3 of GPIOB
        
        // Wait until the specified event (Timer 3, bit 2) occurs
        while ((TIMER3->RIS & (1 << 2)) == 0);
        
        time_val[0] = TIMER3->TAR; // Store time value in array
        TIMER3->ICR = 5;            // Clear specific interrupt for Timer 3
        
        // Wait for another event (Timer 3, bit 2)
        while ((TIMER3->RIS & (1 << 2)) == 0);
        
        time_val[1] = TIMER3->TAR; // Store time value in array
        TIMER3->ICR = 5;            // Clear specific interrupt for Timer 3
        
        // Calculate pulse width and distance based on time values
        pulse_width = (time_val[1] - time_val[0]) / 16;
        distance = (pulse_width * 34) / 200;
        
        // Create a formatted string with pulse width and distance
        sprintf(tempchar, "Pulse Width: %d, Distance: %d\r\4", pulse_width, distance);
        
        // Call external function to output the string
        OutStr(tempchar);
    }
}
