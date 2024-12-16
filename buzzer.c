#include<nxp\iolpc2148.h>
void delay(double x)
{
double i;
for(i=0;i<x;i++);
}
void main(void)
{
    PINSEL0_bit.P0_15=0;
    IO0DIR_bit.P0_15=1;
    while(1)
    {
      IO0PIN_bit.P0_15=1;
      delay(5000);
      IO0PIN_bit.P0_15=0;
      delay(5000);
    }
}