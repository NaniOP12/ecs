// Used Port Lines
// SWITCH INTERFACE
// SW0 - SW7 : P0.16 - P0.23
// LED0 - LED7 : P1.24 - P1.31
// Switch Staus Displayed on LED
#include <nxp/iolpc2148.h>
static void delay(void) //Delay Routine
{
volatile int i,j;
for (i=0;i<0x3F;i++)
for (j=0;j<500;j++);
}
void main()
{
 PINSEL1 = 0x00000000; // Configured as GPIO port
 PINSEL2 = 0x00000000; // Configured as GPIO port
 IO0DIR = 0X00000000; // P0.16 TO P0.23 Configured as input
 IO1DIR = 0Xff000000; // P1.23 TO P1.31 Configured as output
 while(1)
 {
 IO1PIN = IO0PIN<<0x08;  //Read the switch value
 delay();
 }
} //And shift 8 times and then move