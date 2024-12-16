#include <NXP/iolpc2148.h>  // Use the appropriate header for your LPC2148
#include "intrinsics.h"
#include "stdio.h"

// Define the buzzer pin (P0.15)
#define BUZZER_PIN (0x00008000)  // P0.15 bit mask

// External Interrupt Service Routine
void EXINT3(void) {
    EXTINT_bit.EINT3 = 1;  // Clear the interrupt flag for EINT3
    IO0SET = BUZZER_PIN;   // Turn ON the buzzer (set P0.15 high)
    delay();                // Keep the buzzer on for a while
    IO0CLR = BUZZER_PIN;   // Turn OFF the buzzer (clear P0.15)
    printf("External interrupt occurred and buzzer activated\n\r");
}

// Function to print to serial port
int putchar(int ch) {
    if (ch == '\n') {
        while ((U0LSR & 0x20) != 0x20);  // Wait for THR to be empty
        U0THR = '\r'; // Send carriage return
    }
    while ((U0LSR & 0x20) != 0x20);  // Wait for THR to be empty
    return (U0THR = ch);  // Send character
}

// Delay function
void delay() {
    for (int i = 0; i <= 0xFF; i++) {
        for (int j = 0; j <= 0xFF; j++);
    }
}

void main() {
    PINSEL0 = 0x00000005; // Configure RxD and TxD for serial communication
    VPBDIV = 0x01;        // Set PCLK = XCLK

    // Set baud rate to 19200
    int baudrate = (12000000 / (19200 * 16));
    U0LCR = 0x83;          // Enable DLAB
    U0DLL = baudrate & 0x00FF;
    U0DLM = (baudrate >> 8) & 0x00FF;
    U0LCR = 0x03;          // 8 data bits, 1 stop bit, no parity

    // Configure P0.30 for EINT3
    PINSEL1_bit.P0_30 = 2; 
    PINSEL2 = 0x00000000;
    IO1DIR = 0xFF000000;   // Set the direction for GPIO pins

    // Configure P0.15 as output for buzzer
    IO0DIR |= BUZZER_PIN;  // P0.15 - OUTPUT

    // Configure interrupt
    VICIntSelect &= ~(1 << VIC_EINT3);  // EINT3 as IRQ
    VICVectAddr0 = (unsigned int)&EXINT3;  // Assign ISR
    VICVectCntl0 = 0x20 | VIC_EINT3;  // Enable interrupt for EINT3
    VICIntEnable |= (1 << VIC_EINT3);  // Enable EINT3 interrupt

    __enable_interrupt();  // Enable global interrupts

    while (1) {
        IO1PIN = 0xFF000000;  // Example output action (can be modified)
        delay();               // Delay
        IO1PIN = 0x00000000;   // Reset output
        delay();               // Delay
    }
}

// IRQ Handler
#pragma vector = 0x18
__irq __arm void irq_handler(void) {
    void (*interrupt_function)();
    unsigned int vector;

    vector = VICVectAddr;  // Get interrupt vector
    interrupt_function = (void(*)())vector;  // Get the interrupt function address
    interrupt_function();  // Call the interrupt function
    VICVectAddr = 0;  // Clear the interrupt vector
}

// FIQ Handler (Not in use here, but defined)
__fiq __arm void fiq_handler(void) {
    while (1);  // FIQ handler does nothing in this example
}
