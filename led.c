#include <nxp/iolpc2148.h> // Ensure the correct header file is included

// Delay function
void delay() {
    for (int i = 0x00; i <= 0xff; i++) {
        for (int j = 0x00; j <= 0xff; j++);
    }
}

int main() {
    PINSEL2 = 0x00000000; // Configure P1.24 to P1.31 as GPIO
    IO1DIR = 0xFF000000; // Set P1.24 to P1.31 as output ports

    while (1) {
        // Blink the first 4 LEDs (P1.24 to P1.27)
        IO1SET = 0xF0000000; // Set P1.24 to P1.27 high
        delay(); // Delay
        IO1CLR = 0xF0000000; // Set P1.24 to P1.27 low
        delay(); // Delay

        // Blink the next 4 LEDs (P1.28 to P1.31)
        IO1SET = 0x0F000000; // Set P1.28 to P1.31 high
        delay(); // Delay
        IO1CLR = 0x0F000000; // Set P1.28 to P1.31 low
        delay(); // Delay
    }

    return 0; // This line will never be reached
}
