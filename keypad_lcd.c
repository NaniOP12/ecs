#include <nxp/iolpc2148.h>

// LCD and Keypad Port Configuration
#define LCD_EN 0x00000800       // P0.11 - LCD Enable
#define RS 0x00000100           // P0.8 - RS
#define SCAN_MASK 0x00F00000    // Mask for Keypad Scan Lines (P0.20 to P0.23)
#define READ_MASK 0x000F0000    // Mask for Keypad Read Lines (P0.16 to P0.19)

// Buffer to store keys pressed so far
char key_buffer[17];   // Assuming a 16x2 LCD (first line can hold 16 characters)
int cursor_position = 0;

// Function Prototypes
void delay_ms();
void busy_check();
void command_write(int);
void lcd_init();
void lcd_out(unsigned char);
void lcd_display_key(unsigned char);
void display_message(const char *);
void scan_keypad_display();


void lcd_clear()
{
    command_write(0x01);       // Send the command to clear the display
    cursor_position = 0;       // Reset cursor position
    key_buffer[0] = '\0';      // Clear the key buffer
    command_write(0x80);       // Move cursor to the beginning of the first line
    display_message(key_buffer); // Refresh the display with the cleared buffer
}

// Delay Routine
void delay_ms()
{
    unsigned int i, j;
    for (j = 0; j < 0x100; j++)  // Reduce the outer loop iteration count
        for (i = 0; i < 0x40; i++);  // Reduce the inner loop iteration count
}

// Busy check delay
void busy_check()
{
    delay_ms();
}

// Send command to LCD
void command_write(int comm)
{
    busy_check();
    IO0CLR = RS;               // RS = 0 for command
    IO1PIN = comm << 16;        // Send command to P1.16 to P1.23
    IO0SET = LCD_EN;           // Enable LCD
    IO0CLR = LCD_EN;           // Disable LCD
}

// Initialize LCD
void lcd_init()
{
    command_write(0x38);       // 5x7 Matrix Display
    command_write(0x01);       // Clear Display
    command_write(0x0F);       // Display ON, Cursor Blinking
    command_write(0x06);       // Entry Mode
    command_write(0x80);       // Start at First Line
}

// Send a character to LCD
void lcd_out(unsigned char data)
{
    busy_check();
    IO0SET = RS;               // RS = 1 for data
    IO1PIN = (data << 16);      // Send data to P1.16 to P1.23
    IO0SET = LCD_EN;           // Enable LCD
    IO0CLR = LCD_EN;           // Disable LCD
}

// Display a string on the LCD
void display_message(const char *message)
{
    while (*message)
    {
        lcd_out(*message++);    // Display each character
    }
}

// Display the pressed keypad key on the LCD
void lcd_display_key(unsigned char key)
{
    // Add the key to the buffer
    if (cursor_position < 16)  // Ensure we don't overflow the first line
    {
        key_buffer[cursor_position++] = key;  // Add key to buffer and increment cursor
        key_buffer[cursor_position] = '\0';   // Null-terminate the buffer
    }

    // Display the entire buffer
    command_write(0x80);        // Set cursor at the beginning of the first line
    display_message(key_buffer); // Display the buffer on the LCD
}

// Scan keypad and display the detected key on the LCD
void scan_keypad_display()
{
    IO0DIR |= SCAN_MASK;        // Set P0.20 to P0.23 as output (Scan Lines)
    IO0DIR &= ~READ_MASK;       // Set P0.16 to P0.19 as input (Read Lines)

    while (1)
    {
        // Scan each row and check for a key press
        IO0PIN = 0x00E00000; // Set P0.20 (first row) to low
        if ((IO0PIN & READ_MASK) != READ_MASK)  // Check if a key is pressed in first row
        {
            switch (IO0PIN & READ_MASK)
            {
                case 0x00070000: lcd_display_key('F'); break;
                case 0x000B0000: lcd_display_key('B'); break;
                case 0x000D0000: lcd_display_key('7'); break;
                case 0x000E0000: lcd_display_key('3'); break;
            }
        }

        IO0PIN = 0x00D00000; // Set P0.21 (second row) to low
        if ((IO0PIN & READ_MASK) != READ_MASK)  // Check if a key is pressed in second row
        {
            switch (IO0PIN & READ_MASK)
            {
                case 0x00070000: lcd_display_key('E'); break;
                case 0x000B0000: lcd_display_key('A'); break;
                case 0x000D0000: lcd_display_key('6'); break;
                case 0x000E0000: lcd_display_key('2'); break;
            }
        }

        IO0PIN = 0x00B00000; // Set P0.22 (third row) to low
        if ((IO0PIN & READ_MASK) != READ_MASK)  // Check if a key is pressed in third row
        {
            switch (IO0PIN & READ_MASK)
            {
                case 0x00070000: lcd_display_key('D'); break;
                case 0x000B0000: lcd_display_key('9'); break;
                case 0x000D0000: lcd_display_key('5'); break;
                case 0x000E0000: lcd_display_key('1'); break;
            }
        }

        IO0PIN = 0x00700000; // Set P0.23 (fourth row) to low
        if ((IO0PIN & READ_MASK) != READ_MASK)  // Check if a key is pressed in fourth row
        {
            switch (IO0PIN & READ_MASK)
            {
                case 0x00070000: lcd_display_key('C'); break;
                case 0x000B0000: lcd_display_key('8'); break;
                case 0x000D0000: lcd_display_key('4'); break;
                case 0x000E0000: lcd_display_key('0'); break;
            }
        }

        delay_ms();  // Small delay to avoid bouncing
    }
}

// Main function
void main()
{    
    // Pin Selection and GPIO Configuration
    int i =0;
    PINSEL0 = 0x00000000;        // P0.8, P0.11 - GPIO for RS and LCD Enable
    PINSEL2 = 0x00000000;        // P0.16 to P0.23 as GPIO for keypad and LCD data
    IO0DIR = 0x00000900;         // P0.8 (RS), P0.11 (LCD Enable) as Output
    IO1DIR = 0xFF0000;           // P1.16 to P1.23 as Output (LCD data)

    lcd_init();                  // Initialize LCD
    display_message("4x4 Keypad Interface");
    command_write(0xC0);         // Move to second line
    display_message("Press a key...");
    for(i=0;i<50;i++)
      delay_ms();
    lcd_clear();
    scan_keypad_display();       // Continuously scan the keypad and display the keys
}