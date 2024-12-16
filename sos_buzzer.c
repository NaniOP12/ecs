#include<nxp\iolpc2148.h>
void delay(double x)
{
double i;
for(i=0;i<x;i++);
}

void dot();
void dash();
void letter_space();
void word_space();

void main(void)
{
// Pin Configuration
PINSEL1_bit.P0_31 = 0;  // Set P0.31 as GPIO
PINSEL1_bit.P0_29 = 0;  // Set P0.29 as GPIO
PINSEL0_bit.P0_15 = 0;  // Set P0.15 as GPIO
IO0DIR_bit.P0_29 = 1;   // Set P0.29 as output relay
IO0DIR_bit.P0_31 = 1;   // Set P0.31 as output relay
IO0DIR_bit.P0_15 = 1;   // Set P0.15 as output buzzer


    while(1) {
        // Morse Code for "SOS"
        // S: ...
        dot(); dot(); dot();
        letter_space();
       
        // O: ---
        dash(); dash(); dash();
        letter_space();
       
        // S: ...
        dot(); dot(); dot();
        word_space();
    }


}
void dot() {
    IO0PIN_bit.P0_15 = 1;  // Turn on Buzzer
    delay(5000);            
    IO0PIN_bit.P0_15 = 0;  // Turn off Buzzer
    delay(5000);            
}

void dash() {
    IO0PIN_bit.P0_29 = 1;  // Turn on relay
    delay(5000);            
    IO0PIN_bit.P0_29 = 0;  // Turn off relay
    delay(5000);            
}

void letter_space() {
    delay(5000);             // Space between letters
}

void word_space() {
    delay(17000);            // Space between words
}