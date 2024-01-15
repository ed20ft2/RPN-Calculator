// ------------------------------------------------------------------------------
// Project: Calculator
// Authors: Farhan Tanvir
// Date: November 29, 2023
// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
// Includes
// ------------------------------------------------------------------------------
#include "TExaS.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "gpio.h"
#include "keypad.h"
#include "PLL.h"
#include "systick.h"
#include "LCD.h"

// ------------------------------------------------------------------------------
// Constants
// ------------------------------------------------------------------------------
#define MAX_STRING_LENGTH 20        // Change this according to your needs
#define MAX_EXPRESSION_SIZE 20
#define MAX_STACK_SIZE 10            // Adjust based on the maximum number of operators expected

// ------------------------------------------------------------------------------
// Function Prototypes
// ------------------------------------------------------------------------------
void addToInputString(char character);
void clearInputString();
void floatToString(float num, char* str, unsigned char precision);
char* intToString(int num, char* str);

// Stack-related function prototypes
void push(char operator);
char pop();
int isOperator(char token);
int precedence(char operator);
int isLeftAssociative(char operator);
void convertToRPN(char* infix, char* rpn);

// Stack-related function prototypes (Part 2)
float rpnStack[50];
int rpnTop = -1;
void rpnPush(float value);
float rpnPop();
float calculateRPN(char* rpn);

void clear_one(void); // Clear one character (Part 3)
void shiftStateLED();

// Password Module
volatile int isEnteringPassword = 1;  // Flag to indicate if password is being entered
void handlePasswordInput();
unsigned char selectedPrecision = 2;
void handlePrecisionSelection(unsigned char *precision);

//
// Cases
void button1();
void button2();
void button3();
void buttonA();
void button4();
void button5();
void button6();
void buttonB();
void button7();
void button8();
void button9();
void buttonC();
void button_star();
void button0();
void button_hash();
void buttonD();

// ------------------------------------------------------------------------------
// Variables
// ------------------------------------------------------------------------------
char inputString[MAX_STRING_LENGTH];

int inputIndex = 0;
volatile int shiftState = 0;  // 0: No shift, 1: Addition/Subtraction, 2: Multiplication/Division

// Define a stack structure with static memory allocation
char operatorStack[MAX_STACK_SIZE]; // MAX_STACK_SIZE is 20
int operatorTop = -1;

// ------------------------------------------------------------------------------
// MAIN: Mandatory for a C Program to be executable
// ------------------------------------------------------------------------------
int main(void){
	
	PLL_Init(80000000);  // 80MHz = 80,000,000
	SysTick_Init();      //
  PortF_Init();        // Call initialization of port PF4 PF2 
	PortA_Init();        //
	PortB_Init();        //
	PortE_Init();        // Keypad rows: input from PORTE [0:3]. 
	PortD_Init();        // Keypad columns: output from PORTD [0:3].

	lcd_init_4b();       //
	lcd_write_string_4b("ELEC3662            ");
	lcd_write_string_4b("201528357");
	lcd_write_string_4b("         ");
	clearDisplay();
	lcd_write_string_4b("Intializing");
	lcd_write_string_4b("...      ");

	
	clearInputString();
	
	while(1){
			char key = Keypadpressed();
			handleButton(key);
			if (isEnteringPassword) {
			handlePasswordInput();
			handlePrecisionSelection(&selectedPrecision);
			isEnteringPassword = 0;  // Reset the flag after handling password input
		}
	}
}


// ------------------------------------------------------------------------------
// VOID functions: Mandatory for a C Program to be executable
// ------------------------------------------------------------------------------


void handlePasswordInput() {
    int i;
    char password[] = "1234";

    // Allow multiple attempts
    int attempts = 0;
    while (attempts < 3) {  // Adjust the number of allowed attempts as needed
        clearInputString();
        lcd_write_string_4b("Password:");

        while (inputIndex < 4) {
            GPIO_PORTF_DATA_R = 0x02;
            char pass = Keypadpressed();
            handleButton(pass);
        }

        // Check if the entered password is correct
        if (strcmp(inputString, password) == 0) {
            // Password is correct
            lcd_write_string_4b("       Access Granted");
            lcd_write_string_4b("                          ");
            clearDisplay();					
            return;  // Exit the function if the password is correct
        } else {
            // Password is incorrect
            lcd_write_string_4b("       Access Denied");
						lcd_write_string_4b("              ");
						clearDisplay();
            lcd_write_string_4b("Please try again...");
						lcd_write_string_4b("              ");
            clearDisplay();
            attempts++;
        }
    }

    // Display a message if the user exceeds the maximum number of attempts
    lcd_write_string_4b("Maximum attempts reached");
    SysTick_Wait(16000000);  // Add a delay before clearing the display
    clearDisplay();
}

void handlePrecisionSelection(unsigned char *precision) {
		clearInputString();
    *precision = 0;  // Initialize precision to 0 or a default value
		//char precision[] = "1";
    lcd_write_string_4b("Precision: ");
		while(1){
		while (inputIndex < 1) {
					GPIO_PORTF_DATA_R = 0x02;
					char pass = Keypadpressed();
					handleButton(pass);
			}
		// Check the entered key and set the precision accordingly
			switch (inputString[0]) {
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
							*precision = inputString[0] - '0';  // Convert char to int
							lcd_write_string_4b("        ");
					    lcd_write_string_4b("Valid input");
							lcd_write_string_4b("            ");
							clearDisplay();
							//Testing Purposes
							//lcd_write_string_4b("Precision: ");
							//lcd_write_character_4b(*precision + '0');  // Convert int to char and display
							//lcd_write_string_4b("            ");
							break;
					default:
            // Handle invalid input (optional)
					  lcd_write_string_4b("        ");
            lcd_write_string_4b("Invalid input");
						lcd_write_string_4b("            ");
            // You may add additional logic or feedback here
					  clearInputString();
					  clearDisplay();
					  lcd_write_string_4b("Precision: ");
            continue;
			}
			break;
		}
		*precision = selectedPrecision;
			
    // Reset inputIndex and clear inputString for future use
    inputIndex = 0;
    clearInputString();
		clearDisplay();
}

void handleButton(char key) {
			switch (key) {
			default:
				shiftStateLED();
				break;
			case '1':
				button1();
				break;
			case '2':
				button2();
				break;
			case '3':
				button3();
				break;
			case 'A':
				buttonA();
				break;
			case '4':
				button4();
				break;
			case '5':
				button5();
				break;
			case '6':
				button6();
				break;
			case 'B':
				buttonB();
				break;
			case '7':
				button7();
				break;
			case '8':
				button8();
				break;
			case '9':
				button9();
				break;
			case 'C':
				buttonC();
				break;
			case '*':
				button_star();
				break;
			case '0':
				button0();
				break;
			case '#':
				button_hash();
				break;
			case 'D':
				buttonD();
				break;
	}
}

void shiftStateLED() {
    switch (shiftState) {
    default:
        GPIO_PORTF_DATA_R = 0x08; // Green LED for shift state 0
        break;
    case 1:
        GPIO_PORTF_DATA_R = 0x04; // Blue LED for shift state 1
        break;
    case 2:
        GPIO_PORTF_DATA_R = 0x02; // Red LED for shift state 2
        break;
    }
}


// Function to add a character to the input string
void addToInputString(char character) {
    if (inputIndex < MAX_STRING_LENGTH - 1) {
        inputString[inputIndex] = character;
        inputIndex++;
        inputString[inputIndex] = '\0'; // Null-terminate the string
    }
}

// Function to clear the input string
void clearInputString() {
    inputIndex = 0;
    inputString[0] = '\0';
}

void lcd_write_string_4b(char theString[])
{
    volatile int i = 0;  // character counter
    while (theString[i] != '\0')
    {
			lcd_write_character_4b(theString[i]);
			i++;
			SysTick_Wait(80000);  // 40 uS delay (min)
    }
}

// Case Functions

void button1(){
		lcd_write_character_4b('1');  // Write '1' to the LCD
		addToInputString('1');
		SysTick_Wait(16000000);
		SysTick_Wait(8000000);
}

void button2(){
		lcd_write_character_4b('2');  // Write '1' to the LCD
		addToInputString('2');
		SysTick_Wait(16000000);
		SysTick_Wait(8000000);
}

void button3(){
		lcd_write_character_4b('3');  // Write '3' to the LCD#
		addToInputString('3');
		SysTick_Wait(16000000);
		SysTick_Wait(8000000);
}

void buttonA(){
		if (shiftState == 1) {
			// Subtraction
			lcd_write_character_4b('-');
			addToInputString('-');
	} else if (shiftState == 2) {
			if (inputIndex > 0) {
			// Check if there is something to delete
			inputIndex--; // Move back one position
			inputString[inputIndex] = '\0'; // Null-terminate to "remove" the last character
			// Delete previously sent value
			lcd_write_instruction_4b(0x10);
			lcd_write_character_4b(0x20);
			lcd_write_instruction_4b(0x10);
			}
	} else {
			// Addition
			lcd_write_character_4b('+');
			addToInputString('+');
	}
	SysTick_Wait(16000000);
	SysTick_Wait(8000000);
}

void button4(){
		lcd_write_character_4b('4');  // Write '3' to the LCD
		addToInputString('4');
		SysTick_Wait(16000000);
		SysTick_Wait(8000000);
}
void button5(){
		lcd_write_character_4b('5');  // Write '3' to the LCD
		addToInputString('5');
		SysTick_Wait(16000000);
		SysTick_Wait(8000000);
}
void button6(){
		lcd_write_character_4b('6');  // Write '3' to the LCD
		addToInputString('6');
		SysTick_Wait(16000000);
		SysTick_Wait(8000000);
}
void buttonB(){
		if (shiftState == 1) {
				// Division
				lcd_write_character_4b(0xB8);
				addToInputString('/');
		} else if (shiftState == 2) {
				// Power
				lcd_write_character_4b('^');
				addToInputString('^');
		} else {
				// Multiplication
				lcd_write_character_4b(0xB7);
				addToInputString('*');
		}
		SysTick_Wait(16000000); // Wait for button press effect
		SysTick_Wait(8000000);  // Additional wait time
}
void button7(){
		lcd_write_character_4b('7');  // Write '7' to the LCD
		addToInputString('7');
		SysTick_Wait(16000000);
		SysTick_Wait(8000000);
}
void button8(){
		lcd_write_character_4b('8');  // Write '8' to the LCD
		addToInputString('8');
		SysTick_Wait(16000000);
		SysTick_Wait(8000000);
}
void button9(){
		lcd_write_character_4b('9');  // Write '9' to the LCD
		addToInputString('9');
		SysTick_Wait(16000000);
		SysTick_Wait(8000000);
}
void buttonC(){
		if (shiftState == 1) {
				// Open parenthesis
				lcd_write_character_4b('(');
				addToInputString('(');
		} else if (shiftState == 2) {
				// Close parenthesis
				lcd_write_character_4b(')');
				addToInputString(')');
		} else {
				// Decimal point
				lcd_write_character_4b('.');
				addToInputString('.');
		}
		SysTick_Wait(16000000); // Wait for button press effect
		SysTick_Wait(8000000);  // Additional wait time
}
void button_star(){
		// Toggle the shift state
		shiftState = (shiftState + 1) % 3;  // Cycle through 0, 1, 2
		// Add your logic for '*' button based on the shift state
		if (shiftState == 0) {
				// No shift - Reset to default state
				// You can provide visual feedback on the LCD if needed
				//lcd_write_string_4b("Default Mode");
		} else if (shiftState == 1) {
				// Shifted state 1 - First alternate functions
				// Provide visual feedback on the LCD if needed
				//lcd_write_string_4b("Alt Mode 1");
		} else if (shiftState == 2) {
				// Shifted state 2 - Second alternate functions
				// Provide visual feedback on the LCD if needed
				//lcd_write_string_4b("Alt Mode 2");
		}
		SysTick_Wait(16000000);
		SysTick_Wait(8000000);
}
void button0(){
		lcd_write_character_4b('0');  // Write '0' to the LCD
		addToInputString('0');
		SysTick_Wait(16000000);
		SysTick_Wait(8000000);
}
void button_hash(){
		//lcd_write_character_4b('#');  // Write '#' to the LCD
		clearInputString();
						// Clear Display instruction
		clearDisplay();
		SysTick_Wait(16000000);
		SysTick_Wait(8000000);
}
void buttonD(){
		lcd_write_character_4b('=');  // Write '=' to the LCD
		char resultString[50];
		char finalString[50];

		// test purposes!
		//char infixExpression[] = "((8/2)+2)^2";
		convertToRPN(inputString, resultString);
	
		//lcd_write_string_4b(inputString);
	
		float result = calculateRPN(resultString);
		
		floatToString(result, finalString, selectedPrecision);  // Assuming 2 decimal places

		lcd_write_string_4b(finalString);

		SysTick_Wait(16000000);
		SysTick_Wait(8000000);
}

// Function to add an operator to the stack
void push(char operator) {
    if (operatorTop < MAX_STACK_SIZE - 1) {
        operatorStack[++operatorTop] = operator;
    }
}

// Function to remove and return the top operator from the stack
char pop() {
    if (operatorTop != -1) {
        return operatorStack[operatorTop--];
    }
    return '\0';
}

// Function to check if the token is an operator
int isOperator(char token) {
    return token == '+' || token == '-' || token == '*' || token == '/' || token == '^';
}

// Function to determine the precedence of operators
int precedence(char operator) {
    switch (operator) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}

// Function to check if an operator is left associative
int isLeftAssociative(char operator) {
    // The '^' operator is right-associative, not left-associative
    return operator != '^';
}

void convertToRPN(char* infix, char* rpn) {
    int rpnIndex = 0;
    operatorTop = -1; // Reset the stack top
    int i;
    char token;

    for (i = 0; infix[i]; ++i) {
        token = infix[i];

        if (isdigit(token) || token == '.') { // Include decimal point
            // If the token is a number or decimal point, add it to the RPN expression
            while (isdigit(infix[i]) || infix[i] == '.') {
                rpn[rpnIndex++] = infix[i++];
            }
            rpn[rpnIndex++] = ','; // Add a comma delimiter after the number
            --i; // Since the for loop also increments i, we decrement it here
        } else if (token == '(') {
            // If the token is a left parenthesis, push it onto the stack
            push(token);
        } else if (token == ')') {
            // If the token is a right parenthesis, pop operators until a left parenthesis is encountered
            while (operatorTop != -1 && operatorStack[operatorTop] != '(') {
                rpn[rpnIndex++] = pop();
                rpn[rpnIndex++] = ','; // Add a comma delimiter after the operator
            }
            pop(); // Pop the left parenthesis
        } else if (isOperator(token)) {
            // If the token is an operator, pop operators with higher precedence or equal precedence if left-associative
            while (operatorTop != -1 && precedence(operatorStack[operatorTop]) >= precedence(token) && (isLeftAssociative(token) || precedence(operatorStack[operatorTop]) > precedence(token))) {
                rpn[rpnIndex++] = pop();
                rpn[rpnIndex++] = ','; // Add a comma delimiter after the operator
            }
            push(token);
        }
    }

    // Pop any remaining operators from the stack
    while (operatorTop != -1) {
        rpn[rpnIndex++] = pop();
        if (operatorTop != -1) { // Avoid adding a comma after the last operator
            rpn[rpnIndex++] = ','; // Add a comma delimiter after the operator
        }
    }

    if (rpnIndex > 0 && rpn[rpnIndex - 1] == ',') {
        rpnIndex--; // Remove the trailing comma if present
    }
    rpn[rpnIndex] = '\0'; // Null-terminate the RPN expression
}




void floatToString(float num, char* str, unsigned char precision) {
    char* result = str;  // Point result to the beginning of the destination array

    // Handle negative numbers
    if (num < 0) {
        *result++ = '-';
        num = -num;
    }

    // Convert integer part to string
    int integerPart = (int)num;
    result = intToString(integerPart, result);

    if (precision > 0) {
        *result++ = '.';  // Add decimal point

        // Convert fractional part to string with the specified precision
        float fractionalPart = num - integerPart;

        // Round to the desired precision
        float multiplier = pow(10, precision);
        fractionalPart = round(fractionalPart * multiplier) / multiplier;

        unsigned char i;
        for (i = 0; i < precision; i++) {
            fractionalPart *= 10;
            *result++ = '0' + (int)fractionalPart;
            fractionalPart -= (int)fractionalPart;
        }
    }

    // Null-terminate the result string
    *result = '\0';
}

char* intToString(int num, char* str) {
    // Convert integer to string
    if (num == 0) {
        *str++ = '0';
    } else {
        char buffer[10];
        int i = 0;

        while (num > 0) {
            buffer[i++] = '0' + num % 10;
            num /= 10;
        }

        // Reverse the buffer and copy to str
        while (--i >= 0) {
            *str++ = buffer[i];
        }
    }

    // Null-terminate the string
    *str = '\0';

    return str;  // Return the updated pointer position
}

// New LCD display function (replace this with your LCD display function)
void lcd_display_value(float value) {
    // Replace this with your LCD display logic
    // Example: lcd_write_float(value);
}


void rpnPush(float value) {
    if (rpnTop < MAX_STACK_SIZE - 1) {
        rpnStack[++rpnTop] = value;
    } else {
        // Handle stack overflow (replace with your error handling logic)
    }
}

float rpnPop() {
    if (rpnTop >= 0) {
        return rpnStack[rpnTop--];
    } else {
        // Handle stack underflow (replace with your error handling logic)
        return 0.0; // Placeholder return value, modify as needed
    }
}

// RPN calculator function
float calculateRPN(char* rpn) {
    int i = 0;
    float operand1, operand2;
    while (rpn[i] != '\0') {
        if (isdigit(rpn[i]) || (rpn[i] == '-' && isdigit(rpn[i + 1]))) {
            // Read the number and push it onto the stack
            float number = atof(&rpn[i]);
            rpnPush(number);
            // Skip the digits and optional decimal point
            while (isdigit(rpn[i]) || rpn[i] == '.') {
                i++;
            }
        } else if (rpn[i] == ',') {
            // Ignore the comma delimiter
            i++;
        } else {
            // Operator encountered, perform calculation
            operand2 = rpnPop();
            operand1 = rpnPop();
            switch (rpn[i]) {
                case '+':
                    rpnPush(operand1 + operand2);
                    break;
                case '-':
                    rpnPush(operand1 - operand2);
                    break;
                case '*':
                    rpnPush(operand1 * operand2);
                    break;
                case '/':
                    if (operand2 != 0) {
                        rpnPush(operand1 / operand2);
                    } else {
                        // Handle division by zero (replace with your error handling logic)
                        return 0.0; // Placeholder return value, modify as needed
                    }
                    break;
								case '^': rpnPush(pow(operand1, operand2)); 
										break; // Ensure math.h is included for pow
										// Add more operators as needed
                default:
                    // Handle unknown operator (replace with your error handling logic)
                    return 0.0; // Placeholder return value, modify as needed
            }
            i++;
        }
    }
    // The result should be on the top of the stack
    return rpnPop();
}

