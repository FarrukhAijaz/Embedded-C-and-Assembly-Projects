#include "TM4C123GH6PM.h" // Include the header file for the TM4C123GH6PM microcontroller.

#define SAMPLE_MASK 0xFFF // Define a constant SAMPLE_MASK with the value 0xFFF (4095 in decimal).
#define DCOFFSET 0x7FF     // Define a constant DCOFFSET with the value 0x7FF (2047 in decimal).
extern void OutStr(char*); // Declare an external function prototype for OutStr.

/******************************************************************************************************
 * @brief Scales the received integer and turns it into a character array.
 *
 * Since the processor lacks floating-point precision, this function scales the input integer by a factor
 * of 165 and then breaks it into individual digits to store in a character array.
 *
 * @param reading Integer value between -2048 to 2048.
 * @param result  Char array used to store the return value, the returned array scaled.
 * ...
 * @return void since the result is passed as a pointer to the function.
 ******************************************************************************************************/
void scaledval(int reading, char* result) {
    int neg = 0;
    int s_integer = (reading * 165) / 2048; // Scale the reading to a range between -165 to 165.

    if (s_integer < 0) {
        s_integer = -s_integer;
        neg = 1;
    } else {
        neg = 0;
    }

    int hundreds = s_integer / 100;      // Extract each digit of the number separately.
    int remainder = s_integer % 100;
    int tens = remainder / 10;
    int ones = remainder % 10;

    if (neg) {
        result[0] = '-'; // If the number is negative, set the first character in the array to '-'.
    } else {
        result[0] = ' '; // If the number is non-negative, set the first character in the array to ' ' (space).
    }

    result[1] = (char)('0' + hundreds);  // Convert the hundreds digit to a character and store it.
    result[2] = '.';                    // Set the decimal point.
    result[3] = (char)('0' + tens);      // Convert the tens digit to a character and store it.
    result[4] = (char)('0' + ones);      // Convert the ones digit to a character and store it.
    result[5] = '\r';                    // Add a carriage return character.
    result[6] = '\4';                    // Add an end-of-transmission character (Ctrl-D).

}

void delay() {
    for (int i = 0; i <= 1600000; i++) {
        ; // A simple delay loop.
    }
}

void initfunc() {
    /* CLOCK CONFIG */
    SYSCTL->RCGCGPIO |= (1 << 1);  // Enable the clock for GPIO Port B by setting the corresponding bit in RCGCGPIO.
    SYSCTL->RCGCADC |= 0x01;       // Enable the clock for ADC0 by setting the corresponding bit in RCGCADC.

    /* Port configurations */
    GPIOB->DIR &= ~(1 << 4);       // Clear bit 4 in the DIR register of GPIO Port B to set it as an input.
    GPIOB->DEN &= ~(1 << 4);       // Clear bit 4 in the DEN register of GPIO Port B to disable digital functionality.
    GPIOB->AFSEL |= (1 << 4);      // Set bit 4 in the AFSEL register of GPIO Port B to enable alternate function.
    GPIOB->AMSEL |= (1 << 4);      // Set bit 4 in the AMSEL register of GPIO Port B to enable analog mode.

    /* ADC configuration */
    ADC0->ACTSS &= ~(1 << 3);      // Disable sample sequencer 3 by clearing the corresponding bit in ACTSS.
    ADC0->EMUX &= ~(0xF000);        // Clear the bits [15:12] in EMUX to select the trigger source as software.
    ADC0->SSMUX3 = 0x0A;            // Set the input channel for sample sequencer 3 to channel 10.
    ADC0->SSCTL3 |= ((1 << 1) | (1 << 2)); // Set bits 1 and 2 in SSCTL3 to configure the sample control (e.g., enable sample interrupt).
    ADC0->PC = 0X01;                // Set the phase delay for the sample and hold circuit to 1 ADC clock cycle.
    ADC0->ACTSS |= (1 << 3);        // Enable sample sequencer 3 by setting the corresponding bit in ACTSS.
}

int main() {
    initfunc(); // Call the initialization function.

    int reading;             // Declare an integer variable for storing ADC readings.
    signed int final_reading; // Declare a signed integer variable for the final result.
    char scaled[7];           // Declare a character array to store the scaled reading as a string.

    while (1) {
        ADC0->PSSI |= (1 << 3); // Initiate a conversion on sample sequencer 3 by setting the corresponding bit in PSSI.
        while ((ADC0->RIS & (1 << 3)) == 0) {} // Wait until the raw interrupt status bit is set, indicating the conversion is complete.
        reading = ADC0->SSFIFO3 & SAMPLE_MASK; // Read the ADC result from the FIFO and apply SAMPLE_MASK to get a 12-bit result.
        final_reading = DCOFFSET - reading;   // Calculate the final reading by subtracting the ADC reading from DCOFFSET.
        scaledval(final_reading, scaled);     // Call the scaledval function to convert the final reading to a character array.
        ADC0->ISC |= (1 << 3); // Clear the interrupt status by setting the corresponding bit in ISC.
        OutStr(scaled); // Call the external function OutStr to send the scaled value to an external device.
        delay(); // Delay for a short period of time before the next iteration.
    }
}
