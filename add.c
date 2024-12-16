
#include<nxp\iolpc2148.h>
#define  rs IO0PIN_bit.P0_8
#define  en IO0PIN_bit.P0_11
#include <stdio.h>
unsigned char commandarray[5]={0x38,0x01,0x06,0x0c,0x80};
void delay(double x)
{
  unsigned int i;
  for(i=0;i<x;i++);
 
}
void command()
{
  int i;

  for(i=0;i<5;i++)
  {
   
    IO1PIN=commandarray[i]<<16;
      rs=0;
    en=1;
    delay(4095);
    en=0;
   
  }
}
void singlecommandmove(unsigned char command1)
{

  IO1PIN=command1<<16;
    rs=0;
  en=1;
  delay(4095);
  en=0;
}
void movedata(unsigned char dataarray[])
{
  int i;
 
  for(i=0;dataarray[i]!='\0';i++)
  {
   
    IO1PIN=dataarray[i]<<16;
     rs=1;
    en=1;
    delay(4095);
    en=0;
  }
}

void main()
{
  PINSEL0=0x00000000;
  PINSEL2=0x00000000;
  IO0DIR_bit.P0_8=1;
  IO0DIR_bit.P0_11=1;
  IO1DIR=0x00FF0000;
  command();
    unsigned int a,b,c,d;
 
  a = 0x1A3F;
  b = 0xB2;
  c = a + b;
  d = a - b;
  while(1)
  {
    char buffer1[16];
    char buffer[16];
    sprintf(buffer, "%u", c);
    sprintf(buffer1, "%u", d);
    singlecommandmove(0x80);
    movedata("Addition : ");
    movedata((unsigned char*)buffer);
    singlecommandmove(0xc0);
    movedata("Sub : ");
    movedata((unsigned char*)buffer1);
  }
}