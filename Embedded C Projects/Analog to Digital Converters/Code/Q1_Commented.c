#include "TM4C123GH6PM.h" // Include the header file for the TM4C123GH6PM microcontroller.

#define SAMPLE_MASK 0xFFF // Define a constant SAMPLE_MASK with the value 0xFFF (4095 in decimal).
#define DCOFFSET 0x7FF     // Define a constant DCOFFSET with the value 0x7FF (2047 in decimal).

void initfunc(){
    /* CLOCK CONFIG */
    SYSCTL->RCGCGPIO |= (1<<1); // Enable clock for GPIO Port B by setting the corresponding bit in RCGCGPIO.
    SYSCTL->RCGCADC |= 0x01;    // Enable clock for ADC0 by setting the corresponding bit in RCGCADC.

    /* Port configurations */
    GPIOB->DIR &= ~(1<<4);    // Clear bit 4 in the DIR register of GPIO Port B to set it as an input.
    GPIOB->DEN &= ~(1<<4);    // Clear bit 4 in the DEN register of GPIO Port B to disable digital functionality.
    GPIOB->AFSEL |= (1<<4);   // Set bit 4 in the AFSEL register of GPIO Port B to enable alternate function.
    GPIOB->AMSEL |= (1<<4);   // Set bit 4 in the AMSEL register of GPIO Port B to enable analog mode.

    /* ATD configuration */
    ADC0->ACTSS &= ~(1<<3);   // Disable sample sequencer 3 by clearing the corresponding bit in ACTSS.
    ADC0->EMUX &= ~(0xF000);  // Clear the bits [15:12] in EMUX to select the trigger source as software.
    ADC0->SSMUX3 = 0x0A;      // Set the input channel for sample sequencer 3 to channel 10.
    ADC0->SSCTL3 |= ((1<<1)|(1<<2)); // Set bits 1 and 2 in SSCTL3 to configure the sample control (e.g., enable sample interrupt).
    ADC0->PC = 0X01;          // Set the phase delay for the sample and hold circuit to 1 ADC clock cycle.
    ADC0->ACTSS |= (1<<3);    // Enable sample sequencer 3 by setting the corresponding bit in ACTSS.
}

int main(){
    initfunc(); // Call the initialization function.

    int reading; // Declare an integer variable for storing ADC readings.

    while(1){
        ADC0->PSSI |= (1<<3); // Initiate a conversion on sample sequencer 3 by setting the corresponding bit in PSSI.
        while ((ADC0->RIS & (1<<3)) == 0){} // Wait until the raw interrupt status bit is set, indicating the conversion is complete.
        reading = ADC0->SSFIFO3 & SAMPLE_MASK; // Read the ADC result from the FIFO and apply SAMPLE_MASK to get a 12-bit result.

        ADC0->ISC |= (1<<3); // Clear the interrupt status by setting the corresponding bit in ISC.
    }
}
