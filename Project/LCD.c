// LCD.c
#include "LCD.h"
#include "gpio.h"

// 12.5ns because we are working with 80MHz clock PLL
// >100ms = 110ms --> /12.5ns = 8800000
// >4.1ms =  10ms --> /12.5ns =   800000
// >100us = 200us --> /12.5ns =    16000
// 0.4 seconds delay between each key press!
// 1.5ms  = 1.5ms --> /12.5ns = 120000
// 3.0ms  =       --> /12.5ns = 240000
// 80800 = 1ms

void lcd_init_4b(void) {
    // Power-up delay
    SysTick_Wait(8800000); // initial 100 mSec delay 

    // Reset the LCD controller
    lcd_write_4(0x03); // first part of reset sequence
    SysTick_Wait(800000); // 4.1 mS delay (min)

    lcd_write_4(0x03); // second part of reset sequence
    SysTick_Wait(16000);

    lcd_write_4(0x03); // third part of reset sequence
    SysTick_Wait(16000); // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)

    // Preliminary Function Set instruction
    lcd_write_4(0x02); // set 4-bit mode
    SysTick_Wait(80800); // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)

    // Function Set instruction
    lcd_write_instruction_4b(0x28); // set mode, lines, and font
    SysTick_Wait(80800); // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)

    // Display On/Off Control instruction
    lcd_write_instruction_4b(0x08); // turn display OFF
    SysTick_Wait(80800); // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)

		// Clear Display instruction
    lcd_write_instruction_4b(0x01); // clear display RAM
    SysTick_Wait(1000000); // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS) 1.5ms

    // Entry Mode Set instruction
    lcd_write_instruction_4b(0x06); // set desinred shift characteristics
    SysTick_Wait(80800); // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)

    // Display On/Off Control instruction
    lcd_write_instruction_4b(0x0F); // turn the display ON
    SysTick_Wait(80800); // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)
}

void lcd_write_character_4b(unsigned char Character)
{
    GPIO_PORTA_DATA_R |= 0x08;                             // Select the Data Register (RS high)
    GPIO_PORTA_DATA_R &= ~(0x04);                          // Make sure E is initially low
    lcd_write_4((Character >> 4) & 0x0F);                         // Write the upper 4-bits of the data
    lcd_write_4(Character & 0x0F);                  // Write the lower 4-bits of the data
}

void lcd_write_instruction_4b(unsigned char theInstruction)
{
    GPIO_PORTA_DATA_R = 0x00;
    lcd_write_4((theInstruction >> 4) & 0x0F);                      // write the upper 4-bits of the data
    lcd_write_4(theInstruction & 0x0F);               // write the lower 4-bits of the data
}

void lcd_write_4(unsigned char Nibble)
{
   GPIO_PORTB_DATA_R = 0x00;
	 GPIO_PORTB_DATA_R = Nibble;
	
	// void function
	// 'Address set-up time' (40 nS)
	SysTick_Wait(800000);
	GPIO_PORTA_DATA_R |= 0x04;                   			// Enable pin high 0100
	SysTick_Wait(800000);                              // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)
	GPIO_PORTA_DATA_R &= ~(0x04);                  		  // Enable pin low
	SysTick_Wait(800000);                              // implement 'Data hold time' (10 nS) and 'Enable cycle time' (500 nS)
}

void clearDisplay()
{
	lcd_write_instruction_4b(0x01); // clear display RAM
	SysTick_Wait(1000000); // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS) 1.5ms
}


