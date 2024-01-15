// LCD 
#ifndef LCD_H
#define LCD_H

#define lcd_D7     (*((volatile unsigned long *)0x40004020)) // PORTD7 // PB3
#define lcd_D6     (*((volatile unsigned long *)0x40004010)) // PORTD6 // PB2
#define lcd_D5     (*((volatile unsigned long *)0x40004008)) // PORTD5 // PB1
#define lcd_D4     (*((volatile unsigned long *)0x40004004)) // PORTD4 // PB0

#define lcd_E      (*((volatile unsigned long *)0x40005010)) // PORTB2 // PA2 = 0000 0100 = 0x04  = EN // LCD Enable pin
#define lcd_RS     (*((volatile unsigned long *)0x40005020)) // PORTB3 // PA3 = 0000 1000 = 0x08  = RS // Register Select Pin
	

// LCD initializations 
void lcd_init_4b(void);
void lcd_write_4(unsigned char Nibble);
void lcd_write_instruction_4b(unsigned char theInstruction);
void lcd_write_character_4b(unsigned char Character);

void lcd_write_string_4b(char theString[]);

void handleButton(char key);

void clearDisplay();

#endif