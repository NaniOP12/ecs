#include <nxp/iolpc2148.h> 
#include <stdio.h> 

#define CRYSTAL_FREQUENCY_IN_HZ 12000000 
#define PCLK CRYSTAL_FREQUENCY_IN_HZ // Since VPBDIV = 0x01 

// 7-segment patterns for digits 0-9
unsigned char i2c_data[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x6F}; 

// Delay functions
static void delay(void) { 
    volatile int i, j; 
    for (i = 0; i < 100; i++) 
        for (j = 0; j < 1000; j++); 
} 

// I2C write function
void i2c_write(unsigned char data, unsigned char address) { 
    unsigned int temp;

    I2C0CONSET = 0x20; // Send START condition
    while (!(I2C0CONSET & 0x08)); // Wait for START to be set

    I2C0DAT = address; // Send slave address
    I2C0CONCLR = 0x28; // Clear START and SI bits
    while ((I2C0STAT & 0xF8) != 0x18); // Wait for address ACK

    I2C0DAT = data; // Send data
    I2C0CONCLR = 0x08; // Clear SI bit
    while ((I2C0STAT & 0xF8) != 0x28); // Wait for data ACK

    I2C0CONSET = 0x10; // Send STOP condition
    I2C0CONCLR = 0x08; // Clear SI bit
    delay(); 
} 

// Display digits on 7-segment displays
void display_time(int hours, int minutes, int seconds) { 
    // Extract digits for HH:MM:SS
    unsigned char h1 = hours / 10;  // Tens digit of hours
    unsigned char h2 = hours % 10;  // Units digit of hours
    unsigned char m1 = minutes / 10; // Tens digit of minutes
    unsigned char m2 = minutes % 10; // Units digit of minutes
    unsigned char s1 = seconds / 10; // Tens digit of seconds
    unsigned char s2 = seconds % 10; // Units digit of seconds

    // Write digits to respective displays
    i2c_write(i2c_data[h1], 0x70); // Display hours tens digit
    i2c_write(i2c_data[h2], 0x72); // Display hours units digit
    i2c_write(i2c_data[m1], 0x74); // Display minutes tens digit
    i2c_write(i2c_data[m2], 0x76); // Display minutes units digit
    i2c_write(i2c_data[s1], 0x78); // Display seconds tens digit
    i2c_write(i2c_data[s2], 0x7A); // Display seconds units digit
}

// Real-time clock main function
void main() { 
    // Initialize pins for I2C
    PINSEL0 = 0x00000055; // Enable I2C on relevant pins
    VPBDIV = 0x01;        // Set PCLK = CCLK

    // I2C initialization
    I2C0CONCLR = 0x6C;    // Clear all bits in I2C0CONCLR
    I2C0CONSET = 0x40;    // Enable I2C
    I2C0SCLH = 110;       // Set I2C clock high time
    I2C0SCLL = 90;        // Set I2C clock low time

    // Time variables
    int hours = 0, minutes = 0, seconds = 0;

    while (1) { 
        // Display the current time
        display_time(hours, minutes, seconds); 

        // Increment the time
        seconds++; 
        if (seconds == 60) { 
            seconds = 0; 
            minutes++; 
        } 
        if (minutes == 60) { 
            minutes = 0; 
            hours++; 
        } 
        if (hours == 24) { 
            hours = 0; 
        } 

        // Delay for 1 second
        delay(); 
    } 
} 
