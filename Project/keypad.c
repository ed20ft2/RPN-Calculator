// keypad.c
#include "keypad.h"
#include "gpio.h"
#include "math.h"

unsigned char Keypadpressed(void){
	int col, row;
	char result = '\0';
	
  while(1){ 
		
	const char keypad[4][4] = {
			{'1', '2', '3', 'A'},
			{'4', '5', '6', 'B'},
			{'7', '8', '9', 'C'},
			{'*', '0', '#', 'D'}
    };
		for (col = 0; col < 4; col++){
		GPIO_PORTD_DATA_R = pow(2, col);
		for (row = 0; row < 4; row++) {
			volatile unsigned long ii = pow(2, row);
			if ((GPIO_PORTE_DATA_R&ii) == ii){
				result = keypad[row][col];
				}
      }
    }
		return result;
	}
}
