#include <nxp/iolpc2148.h>
#define DESIRED_BAUDRATE 19200
#define CRYSTAL_FREQUENCY_IN_HZ 12000000
#define PCLK CRYSTAL_FREQUENCY_IN_HZ
#define DIVISOR (PCLK/(16*DESIRED_BAUDRATE))

// Delay Routine
void delay_ms()
{
    int i, j;
    for(i=0; i<=0xFF; i++)
        for(j=0; j<=0xF0; j++);
}

// UART putchar function
int putchar(char ch)
{
    if (ch == '\n')
    {
        while (!(U0LSR & 0x20)); // Wait until Transmit Holding Register is empty
        U0THR = '\r';            // Send carriage return
    }
    while (!(U0LSR & 0x20));     // Wait until Transmit Holding Register is empty
    U0THR = ch;                  // Send character
    return ch;
}

// UART Initialization
void Arm_Uart0_Init()
{
    U0LCR = 0x83;                // Enable Divisor Latch access, set 8-bit word length
    VPBDIV = 0x01;               // Set PCLK = processor clock (VPBDIV)
    U0DLL = DIVISOR & 0xFF;      // Set UART0 Divisor Latch (LSB)
    U0DLM = DIVISOR >> 8;        // Set UART0 Divisor Latch (MSB)
    U0LCR = 0x03;                // Disable Divisor Latch access, 8-bit data
    U0FCR = 0x07;                // Enable and reset Tx/Rx FIFOs
}

// Function to print strings over UART
void print_uart(const char *str)
{
    while (*str)
    {
        putchar(*str++);
    }
}

// Main function
void main()
{
    // Pin selection and configuration for UART0 and GPIO
    PINSEL0 = 0x00000005;        // Pin selection for UART0 TXD0 and RXD0
    PINSEL2 = 0x00000000;        // P0.16 to P0.23 Configured as GPIO.
    IO0DIR = 0x00F00000;         // P0.20 to P0.23 (Output), P0.16 to P0.19 (Input)

    Arm_Uart0_Init();            // Initialize UART0

    print_uart("\n\r4 x 4 Matrix Keypad Interface\n\r");
    print_uart("Press any key on the Keypad:\n\r");

    while (1)
    {
        // Set each row to low one by one and check which key is pressed in that row.
        IO0PIN = 0x00E00000; // Set P0.20 (first row) to low
        if ((IO0PIN & 0x000F0000) != 0x000F0000) // Check if a key is pressed
        {
            switch (IO0PIN & 0x000F0000)
            {
                case 0x00070000: print_uart("F\n\r"); break;
                case 0x000B0000: print_uart("B\n\r"); break;
                case 0x000D0000: print_uart("7\n\r"); break;
                case 0x000E0000: print_uart("3\n\r"); break;
            }
        }

        IO0PIN = 0x00D00000; // Set P0.21 (second row) to low
        if ((IO0PIN & 0x000F0000) != 0x000F0000) // Check if a key is pressed
        {
            switch (IO0PIN & 0x000F0000)
            {
                case 0x00070000: print_uart("E\n\r"); break;
                case 0x000B0000: print_uart("A\n\r"); break;
                case 0x000D0000: print_uart("6\n\r"); break;
                case 0x000E0000: print_uart("2\n\r"); break;
            }
        }

        IO0PIN = 0x00B00000; // Set P0.22 (third row) to low
        if ((IO0PIN & 0x000F0000) != 0x000F0000) // Check if a key is pressed
        {
            switch (IO0PIN & 0x000F0000)
            {
                case 0x00070000: print_uart("D\n\r"); break;
                case 0x000B0000: print_uart("9\n\r"); break;
                case 0x000D0000: print_uart("5\n\r"); break;
                case 0x000E0000: print_uart("1\n\r"); break;
            }
        }

        IO0PIN = 0x00700000; // Set P0.23 (fourth row) to low
        if ((IO0PIN & 0x000F0000) != 0x000F0000) // Check if a key is pressed
        {
            switch (IO0PIN & 0x000F0000)
            {
                case 0x00070000: print_uart("C\n\r"); break;
                case 0x000B0000: print_uart("8\n\r"); break;
                case 0x000D0000: print_uart("4\n\r"); break;
                case 0x000E0000: print_uart("0\n\r"); break;
            }
        }

        delay_ms(); // Small delay between scans
    }
}