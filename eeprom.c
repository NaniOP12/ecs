#include<nxp\iolpc2148.h>
#include<stdio.h>
#define PCLKFREQ 12000000
void delay(void)
{
   volatile  int i,j;
   for (i=0;i<5;i++)
    for (j=0;j<50;j++);
}
static void delay1(void )
{
	volatile  int i,j;
		for (i=0;i<0xff;i++)
		for (j=0;j<0xff;j++);
}

void serial_init(unsigned long baud_rate)
{
      unsigned int  BAUDRATEDIVISOR;
      PINSEL0|= 0x00000005;// TO SELCET THE UART0 TX AND RX PIN
      VPBDIV=0X01;// TO SET THE PERIPHERAL CLOCK AS SAME AS THE PROCESSOR CLOCK
      BAUDRATEDIVISOR=(PCLKFREQ/(baud_rate*16));      
      U0LCR=0X80;// TO SET THE DLAB BIT FOR ACCESSING THE DIVISOR LATCH REGISTER
      U0DLL = BAUDRATEDIVISOR & 0x00ff;
      U0DLM = (BAUDRATEDIVISOR >> 8) & 0x00ff;
      U0LCR=0X03;// TO SET THE WORD LENGTH AS 8 BIT
}
int putchar(int a)
{
   if(a == '\n')
   {
        while((U0LSR&0X20)!=0X20);
        U0THR=0x0D;
   }  
   while((U0LSR&0X20)!=0X20);
   return(U0THR=a);
}
void i2c_config(void)
{
        PINSEL0|=0X00000050;
        I2C0CONCLR=0X6C;
        I2C0CONSET=0X40;
        I2C0SCLH=70;
        I2C0SCLL=50;
}
void  i2c_write(void)
{
       I2C0CONSET=0X20;
       delay();
       while(I2C0STAT!=0x08);
       I2C0CONCLR=0X28;
            
       // To send a slave address
       I2C0DAT=0xAC;
       delay();
       while(I2C0STAT!=0x18);
       I2C0CONCLR=0x08; 
             
       // Higher order address
       I2C0DAT=0x00;
       delay();
       while(I2C0STAT!=0x28);
       delay1(); 
       I2C0CONCLR=0x08; 
       
       // Lower order address
       I2C0DAT=0x00;
       delay();
       while(I2C0STAT!=0x28);
       I2C0CONCLR=0x08; 
       
       //Data has been written to appropriate address
       for(char m=0;m<63;m++)
       {
           I2C0CONCLR=0x08; 
           I2C0DAT=m;
           delay();
           while(I2C0STAT!=0x28);
       }
       //stop condition
       I2C0CONSET=0X10;
       I2C0CONCLR=0x08; 
       delay1();
}
void i2c_read(void)
{
       char data; 
       I2C0CONSET=0X20;
       delay();
       while(I2C0STAT!=0x08);
       I2C0CONCLR=0X28;    
       // To send a slave address
        I2C0DAT=0XAC;
        delay();
        while(I2C0STAT!=0x18);
        I2C0CONCLR=0X08;  
      
        // Higher order address
        I2C0DAT=0x00;
        delay();
        while(I2C0STAT!=0x28);
        I2C0CONCLR=0x08; 

        // Lower order address
        I2C0DAT=0x00;
        delay();
        while(I2C0STAT!=0x28);
     	delay();

        //stop condition		
        I2C0CONSET=0x10;
        I2C0CONCLR=0x8;
        delay(); 
 
        // To make a start condition  
        I2C0CONSET=0X20;
        delay();
        while(I2C0STAT!=0x08);
        I2C0CONCLR=0X2C;
         
        // To send a slave address
        I2C0DAT=0XAD;
        delay();
        while(I2C0STAT!=0x40);
        I2C0CONSET=0x04;
        I2C0CONCLR=0X08;  
      
        printf("address------------data\n");
      
        //To read no of character from the slave(EPROM IC)
        for(int k=0;k<0xff;k++)
        {
           while(I2C0STAT!=0x50);
           data=I2C0DAT;
           printf("  00%d------------->%d \n",k,data);
           I2C0CONCLR = 0X0C;
           I2C0CONSET = 0X04;
	   I2C0CONCLR = 0X08;	
        }
      	//stop condition		
       	I2C0CONCLR=0x8;
	I2C0CONSET=0x10;
        delay1(); 
        while(1);
}
void main(void)
{
      serial_init(19200);
      i2c_config();
      i2c_write();
      printf("       welcome   \n");
      while(1)
      {
         i2c_read();
      }
}
