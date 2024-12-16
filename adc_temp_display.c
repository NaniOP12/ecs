
#include <nxp/iolpc2148.h>

#define rs IO0PIN_bit.P0_8
#define en IO0PIN_bit.P0_11

unsigned char commandarray[5] = {0x38, 0x01, 0x06, 0x0c, 0x80};

// Function prototypes
void delay(unsigned int x);
void command();
void singlecommandmove(unsigned char command1);
void movedata(unsigned char dataarray[]);
void serial_init(unsigned long baud_rate);
int putchar(int a);

void delay(unsigned int x) {
    unsigned int i;
    for (i = 0; i < x; i++);
}

void command() {
    int i;
    for (i = 0; i < 5; i++) {
        IO1PIN = commandarray[i] << 16;
        rs = 0; // Command mode
        en = 1; // Enable
        delay(4095);
        en = 0; // Disable
    }
}

void singlecommandmove(unsigned char command1) {
    IO1PIN = command1 << 16;
    rs = 0; // Command mode
    en = 1; // Enable
    delay(4095);
    en = 0; // Disable
}

void movedata(unsigned char dataarray[]) {
    int i;
    for (i = 0; dataarray[i] != '\0'; i++) {
        IO1PIN = dataarray[i] << 16;
        rs = 1; // Data mode
        en = 1; // Enable
        delay(4095);
        en = 0; // Disable
    }
}

void serial_init(unsigned long baud_rate) {
    unsigned int BAUDRATEDIVISOR;
    PINSEL0 |= 0x00000005; // Select UART0 TX and RX pin
    VPBDIV = 0x01; // Set peripheral clock as same as processor clock
    BAUDRATEDIVISOR = (12000000 / (baud_rate * 16));
    U0LCR = 0x80; // Set DLAB bit to access divisor latch
    U0DLL = BAUDRATEDIVISOR & 0x00ff;
    U0DLM = (BAUDRATEDIVISOR >> 8) & 0x00ff;
    U0LCR = 0x03; // Set word length as 8 bits
}

int putchar(int a) {
    if (a == '\n') {
        while ((U0LSR & 0x20) != 0x20); // Wait until the transmitter is empty
        U0THR = 0x0D; // Send carriage return
    }
    while ((U0LSR & 0x20) != 0x20); // Wait until the transmitter is empty
    return (U0THR = a); // Transmit the character
}

void main() {
  char buffer[16];
 
    unsigned int val = 0, volt;
    float vlt;
         PINSEL1_bit.P0_31=0;
PINSEL1_bit.P0_29=0;
    IO0DIR_bit.P0_29=1;
     IO0DIR_bit.P0_31=1;
    PINSEL0 = 0x00000000; // Set GPIO pins
    PINSEL2 = 0x00000000; // Set GPIO pins for LCD
    IO0DIR_bit.P0_8 = 1;  // Set RS as output
    IO0DIR_bit.P0_11 = 1; // Set EN as output
    IO1DIR = 0x00FF0000;   // Set data pins as output

    command(); // Initialize the LCD

    // ADC initialization
    PINSEL1 = 0x01000000; // Select ADC0.1
    AD0CR_bit.CLKDIV = 0x02; // VPB clock (PCLK) is divided by 2
    AD0CR_bit.BURST = 1; // To prevent repeated conversion
    AD0CR_bit.CLKS = 0; // 11 clocks / 10 bits
    AD0CR_bit.PDN = 1; // A/D converter is operational
    AD0CR_bit.SEL = 0x02; // Select ADC channel 1

    while (1) {
        while (AD0DR_bit.DONE == 0); // Start A/D conversion
        val = AD0DR1_bit.RESULT; // Read the ADC result
        volt = val / 3.07; // Convert to temperature
        vlt = (val/1023.0) * 3.3;

        // Display the temperature on LCD
        singlecommandmove(0x80); // Move cursor to the beginning of the first line
        movedata("Temp :"); // Display label
       
        //char buffer[16]; // Buffer to hold temperature string
        sprintf(buffer, "%d C", volt); // Format the temperature
        movedata(buffer); // Display temperature
        delay(100000); // Delay before the next reading
        singlecommandmove(0xC0); // Move cursor to the second line
        movedata("Volt : ");
        sprintf(buffer,"%f V",vlt);
        movedata(buffer);
      if(volt > 27)
    {
     IO0PIN_bit.P0_29=1;
     IO0PIN_bit.P0_31=1;
        delay(5000);
        IO0PIN_bit.P0_29=0;
     IO0PIN_bit.P0_31=0;
        delay(5000);
    }
    }
}