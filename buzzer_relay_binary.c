#include <nxp/iolpc2148.h>  // Ensure this path matches your include directory structure

// Define a string with binary sequence (example: "010101")
const char signal[] = "01010010111";

// Delay function
void delay(unsigned long x) {
    unsigned long i;
    for (i = 0; i < x; i++);
}

void main(void) {
    // Initialize buzzer on P0.15
    PINSEL0_bit.P0_15 = 0;  // Set function to GPIO
    IO0DIR_bit.P0_15 = 1;   // Set P0.15 as output

    // Initialize relay on P0.29 and P0.31
    PINSEL1_bit.P0_29 = 0;  // Set function to GPIO
    PINSEL1_bit.P0_31 = 0;  // Set function to GPIO
    IO0DIR_bit.P0_29 = 1;   // Set P0.29 as output
    IO0DIR_bit.P0_31 = 1;   // Set P0.31 as output

    unsigned int i = 0;  // Index for the signal string
    while (1) {
        // Read each character in the signal string
        char current = signal[i];

        if (current == '0') {
            // Turn relay on
            IO0PIN_bit.P0_29 = 1;  // Turn relay 1 on
            IO0PIN_bit.P0_31 = 1;  // Turn relay 2 on
            delay(200000);          // Keep relay on for a while
            // Turn relay off
            IO0PIN_bit.P0_29 = 0;  // Turn relay 1 off
            IO0PIN_bit.P0_31 = 0;  // Turn relay 2 off
        } else if (current == '1') {
            // Turn buzzer on
            IO0PIN_bit.P0_15 = 1;  // Turn buzzer on
            delay(200000);          // Keep buzzer on for a while
            // Turn buzzer off
            IO0PIN_bit.P0_15 = 0;  // Turn buzzer off
        }

        // Move to the next character in the signal string
        i++;
        if (signal[i] == '\0') {
            // If end of string, reset index
            i = 0;
            delay(2000000);   
        }
        
        // Small delay to ensure signals are distinguishable
        delay(5000);
    }
}
