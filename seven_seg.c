#include <nxp/iolpc2148.h> 
#include <stdio.h> 

#define DESIRED_BAUDRATE 19200  
#define CRYSTAL_FREQUENCY_IN_HZ 12000000 
#define PCLK CRYSTAL_FREQUENCY_IN_HZ // Since VPBDIV = 0x01 
#define DIVISOR (PCLK / (16 * DESIRED_BAUDRATE)) 

// 7-segment patterns for displaying digits 0-9 and A
unsigned char i2c_data[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67}; 

// Delay functions
static void delay(void) { 
    volatile int i, j; 
    for (i = 0; i < 5; i++) 
        for (j = 0; j < 50; j++); 
} 

static void delay1(void) { 
    volatile int i, j; 
    for (i = 0; i < 100; i++) 
        for (j = 0; j < 500; j++); 
} 

// I2C write function
void i2c_write(unsigned char data, unsigned char address) { 
    unsigned int temp;

    I2C0CONSET = 0x20; // Send START condition
    delay(); 
    temp = I2C0STAT; 
    while (temp != 0x08) { // Wait for START condition to be transmitted
        temp = I2C0STAT; 
    } 

    I2C0DAT = address; // Send slave address
    I2C0CONCLR = 0x28; // Clear START bit and SI bit
    temp = I2C0STAT; 
    while (temp != 0x18) { // Wait for address ACK
        temp = I2C0STAT; 
        delay(); 
    } 

    I2C0DAT = data; // Send data
    I2C0CONCLR = 0x08; // Clear SI bit
    temp = I2C0STAT; 
    while (temp != 0x28) { // Wait for data ACK
        temp = I2C0STAT; 
        delay(); 
    } 

    I2C0CONSET = 0x10; // Send STOP condition
    I2C0CONCLR = 0x08; // Clear SI bit
    delay1(); 
} 

// UART putchar function
int putchar(int ch) { 
    while (!(U0LSR & 0x20)) {} // Wait for THR empty
    return (U0THR = ch); 
} 

void main() { 
    // Initialize pins for UART and I2C
    PINSEL0 = 0x00000055; // Enable UART0 and I2C on relevant pins
    VPBDIV = 0x01;        // Set PCLK = CCLK

    // I2C initialization
    I2C0CONCLR = 0x6C;    // Clear all bits in I2C0CONCLR
    I2C0CONSET = 0x40;    // Enable I2C
    I2C0SCLH = 110;       // Set I2C clock high time
    I2C0SCLL = 90;        // Set I2C clock low time

    while (1) { 
        // Write data to six 7-segment displays
        for (int i = 0; i < 10; i++) {
            i2c_write(i2c_data[i], 0x70); // Write to display at 0x70
            i2c_write(i2c_data[i], 0x72); // Write to display at 0x72
            i2c_write(i2c_data[i], 0x74); // Write to display at 0x74
            i2c_write(i2c_data[i], 0x76); // Write to display at 0x76
            i2c_write(i2c_data[i], 0x78); // Write to display at 0x78
            i2c_write(i2c_data[i], 0x7A); // Write to display at 0x7A
        } 
    } 
} 
