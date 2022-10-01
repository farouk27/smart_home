/*
 * smartHome.c
 *
 *  Created on: Apr 27, 2022
 *      Author: Hesham Ashraf & Tamer & Yehia
 */
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "gpio.h"
#include "std_types.h"
#include "common_macros.h"
#include "lcd.h"
#include "KEYPAD.h"
#include "uart.h"
#include "servo.h"
#include "buzzer.h"
#include "timer0.h"
#include "timer2.h"

#define SIZE 5
#define OPEN_DOOR '+'
#define CHANGE_PASS '-'

uint8 g_password[SIZE]={0};
uint8 g_buffer_password[SIZE] = {0};
uint8 no_pass_flag = 0; //if 0 there is pass if 1 no pass

uint8 servo_angle_address = 0x30;
uint8 servo_angle_value = 0;

uint8 confirm_password = 0;

uint8 ext_buzzerAlarm = 0 ;      //accumulator for 3 attempts
uint8 ext_theaf_flag = 0;
/*externed at uart.c fil*/
extern uint8 room1_address;
extern uint8 room2_address;
extern uint8 ext_room1_value;
extern uint8 ext_room2_value;


/*
 * [Description]: this function used to check if the system used before or not.
 */
void checkNoPass(){
	uint8 digit = 0;
	for(uint8 i = 0 ; i < 5 ; i++)
	{
		digit = eeprom_read_byte ((uint8*)i);
		if(digit == 0xff)
		{
			no_pass_flag = 1;
		}
		else
		{
			no_pass_flag = 0;
		}
	}

	return;
}

/*
 * [Description]: this function used to check if the servo is zero or 90 angle
 */
void checkServoAndLeds(){
	servo_angle_value = eeprom_read_byte(servo_angle_address);
	ext_room1_value = eeprom_read_byte(room1_address);
	ext_room2_value = eeprom_read_byte(room2_address);
}
/*
 * [Description]: using this function to interface with the user to get the password
 * from by using the keypad.
 */
void getPassword()
{
	uint8 digitNum = 0;
	LCD_moveCursor(1,0);
	/*looping to get the password's digits from the user*/
	for(digitNum = 0 ; digitNum < SIZE ; digitNum++)
	{
		g_password[digitNum] = KEYPAD_get_Value();
		LCD_displayCharacter('*');
		_delay_ms(500);
	}

}

void getBufferPassword(){
	uint8 digitNum = 0;
	LCD_moveCursor(1,0);

	/*looping to get the password's digits from the user*/
	for(digitNum = 0 ; digitNum < SIZE ; digitNum++){
		g_buffer_password[digitNum] = KEYPAD_get_Value();
		LCD_displayCharacter('*');
		_delay_ms(500);
	}

}

/*
 * [Description]: this function used to is used at the first time during dealing with
 * the locker and when the user wants to change the password.
 */
void newPassword(void){
	LCD_clearScreen();
	LCD_displayString("New Pass:");
	getPassword();
}

/*
 * [Description]: this function used to confirm the password by making the user
 * insert the password from the previous step to confirm it.
 */
void confirmation(void) {
	LCD_clearScreen();
	LCD_displayString("Reenter Pass:");
	getBufferPassword();
}

/*
 * [Description]: this function used to take two arrays and compare if they have the same elements or not
 * according to this it gives a flag confirm_password = 0 if not matched and 1 if matched
 */
void compare(uint8 *arr1 , uint8 *arr2)
{
	for (uint8 i = 0; i < SIZE; i++)
	{
		if (arr1[i] != arr2[i])
		{
			confirm_password = 0;
			return;
		}
	}
	confirm_password = 1;
}

/*
 * [Description]: this function used to get the password from the g_password array to the eeprom
 */
void savePassword(){
	for (uint8 i = 0; i < SIZE; i++) {
		eeprom_write_byte((uint8*)i , g_password[i]);
	}
	no_pass_flag = 0;
}

/*
 * [Description]: this function used to get the password from the memory and assign it to the
 * g_buffer_password array
 */
void getPassFromMemory(){
	for (uint8 i = 0; i < SIZE; i++) {
		g_buffer_password[i] = eeprom_read_byte ((uint8*)i);
	}
}
/*
 * [Description]: this function used to display the options "open door" or "change the password"
 */
void mainOperation(void){
	LCD_clearScreen();
	LCD_displayString("D:Open door");
	LCD_moveCursor(1,0);
	LCD_displayString("C:change pass ");
}

/*
 * [Description]: this function used to get the password from the user when the option
 *  "open door" is selected.
 */
void openDoor(void){
	LCD_clearScreen();
	LCD_displayString("Enter Pass:");
	getPassword();
}

/*
 * [Description]: this function used to get the password from the user when the option
 * "change password" is selected.
 */
void changePassword(void){
	LCD_clearScreen();
	LCD_displayString("Enter Old Pass:");
	getPassword();
}


int main()
{
	/*variable used to hold the selected operation*/
	uint8 operation;


	/********* this sector to initialize the drivers *******/
	sei();
	LCD_init();
	LCD_displayString("WELCOME");
	_delay_ms(3000);
	UART_init(9600);
	Buzzer_Init();
	/*******************************************************/
	TIMER0_CONFIG timer0 = {NORMAL_MODE_TIMER0 , 0 , 0 , F_CPU0_1024};
	TIMER2_CONFIG timer2 = {NORMAL_MODE_TIMER0 , 0 , 0 , F_CPU0_1024};
	TIMER2_init(&timer2);

	/*Set the tap pin direction and give it initial value of 0*/
	GPIO_setupPinDirection(PORTD_ID, PIN7_ID, PIN_OUTPUT);
	GPIO_writePin(PORTD_ID, PIN7_ID, LOGIC_LOW);


	/*set the two rooms pins direction and make it off initially*/
	GPIO_setupPinDirection(PORTD_ID, PIN6_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTD_ID, PIN4_ID, PIN_OUTPUT);
	/*to close the two leds initially*/
	GPIO_writePin(PORTD_ID, PIN6_ID, LOGIC_LOW);
	GPIO_writePin(PORTD_ID, PIN4_ID, LOGIC_LOW);
	/*Make OC1A pin as output (for servo)*/
	GPIO_setupPinDirection(PORTD_ID, PIN5_ID, PIN_OUTPUT);

	/*make the sensors pins as input pins*/
	GPIO_setupPinDirection(PORTD_ID, PIN2_ID, PIN_INPUT);
	GPIO_setupPinDirection(PORTD_ID, PIN3_ID, PIN_INPUT);

	/*------------------------ checks sector---------------------*/
	checkNoPass();
	if (no_pass_flag)
	{
		do {
			newPassword();
			confirmation();
			compare(g_password , g_buffer_password );
		} while (!(confirm_password));
		savePassword();
	}

	checkServoAndLeds();
	if(servo_angle_value != 0){
		SERVO_rotateAngle(ROTATE_ZERO_ANGLE);
		servo_angle_value = 0;
		eeprom_write_byte(servo_angle_address, servo_angle_value);
	}

	if(ext_room1_value == 0){
		GPIO_writePin(PORTD_ID, PIN4_ID, LOGIC_LOW);
	}else if(ext_room1_value == 1){
		GPIO_writePin(PORTD_ID, PIN4_ID, LOGIC_HIGH);
	}

	if(ext_room2_value == 0){
		GPIO_writePin(PORTD_ID, PIN6_ID, LOGIC_LOW);
	}else if(ext_room2_value == 1){
		GPIO_writePin(PORTD_ID, PIN6_ID, LOGIC_HIGH);
	}
	/*--------------------------------------------------------------*/


	/********************************************************************************************/
	while (1){
		/*printing the main options and wait to get operation*/
		mainOperation();
		operation = KEYPAD_get_Value();
		_delay_ms(500);


		switch(operation)
		{
		/*if the selected operation is open door*/
		case OPEN_DOOR:
		{

			while(1)
			{
				openDoor();
				getPassFromMemory();
				compare(g_password , g_buffer_password );
				if(!(confirm_password))
				{
					/*to make the user have 3 attempts*/
					ext_buzzerAlarm++;
					if(ext_buzzerAlarm == 3)
					{
						ext_theaf_flag = 1;
						LCD_clearScreen();
						LCD_displayStringRowColumn(0,3,"A THIEF!!!");
						_delay_ms(1);
						LCD_displayStringRowColumn(1,3,"GO AWAY!!!!");
						_delay_ms(1);

						TIMER0_init(&timer0);
						Buzzer_ON();
						while(ext_buzzerAlarm != 0); //that zero value will be gotten to that variable by the timer0.c file
						TIMER0_DEINIT();
						ext_theaf_flag = 0;
						break;
					}

					continue;
					/*------ if the user entered wrong password for 3 times -------*/
				}

				/*------------- if the entered password is correct ------------*/
				else{
					LCD_clearScreen();
					_delay_ms(1);
					LCD_displayString("OPENED");
					_delay_ms(1);
					LCD_moveCursor(1,0);
					LCD_displayString("Press * to close");
					ext_buzzerAlarm = 0;
					SERVO_rotateAngle(ROTATE_90_ANGLE);
					servo_angle_value = 1;
					eeprom_write_byte(servo_angle_address, servo_angle_value);
					while(KEYPAD_get_Value() != 13);
					SERVO_rotateAngle(ROTATE_ZERO_ANGLE);
					servo_angle_value = 0;
					eeprom_write_byte(servo_angle_address, servo_angle_value);
					break;
				}
				/*-------------------------------------------------------------*/
			}

		}
		break;

		/*if the selected operation is change password*/
		case CHANGE_PASS:
		{
			while(1)
			{
				changePassword();
				getPassFromMemory();
				compare(g_password , g_buffer_password );

				if(!(confirm_password))
				{
					/*to make the user have 3 attempts*/
					ext_buzzerAlarm++;

					if(ext_buzzerAlarm == 3)
					{
						ext_theaf_flag = 1;
						LCD_clearScreen();
						LCD_displayStringRowColumn(0,3,"A THIEF!!!");
						_delay_ms(1);
						LCD_displayStringRowColumn(1,3,"GO AWAY!!!!");
						_delay_ms(1);

						TIMER0_init(&timer0);
						 Buzzer_ON();
						while(ext_buzzerAlarm != 0); //that zero value will be gotten to that variable by the timers.c file
						TIMER0_DEINIT();
						ext_theaf_flag = 0;
						break;
					}

					continue;
				}

				/*------------- if the entered password is correct ------------*/
				else
				{
					ext_buzzerAlarm = 0;
					do {
						newPassword();
						confirmation();
						compare(g_password , g_buffer_password );
					} while (!(confirm_password));
					savePassword();
				}
				/*-------------------------------------------------------------*/
				break;
			}

		}

		}
	}

}




