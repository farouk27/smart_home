/*
 * Ext_interrupt.c
 *
 *  Created on: Mar 12, 2022
 *      Author: Hesham Ashraf
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "gpio.h"
#include "std_types.h"
#include "common_macros.h"
#include "Ext_interrupt.h"
#include "servo.h"
#include "lcd.h"
#include "buzzer.h"
#include "uart.h"
#include <util/delay.h>


uint8 tap_flag  ;
uint8 fire_flag ;

extern uint8 servo_angle_value;

void Open_Tap()
{
	/*trigger the external INT1 with falling edge*/
	SET_BIT( MCUCR , ISC11);
	CLEAR_BIT( MCUCR , ISC10);

	// open the tap
	GPIO_writePin(PORTD_ID, PIN7_ID, LOGIC_HIGH);
	tap_flag = 1;         // change flag

}


void Close_Tap()
{
	/*trigger the external INT1 with rising edge*/
	SET_BIT( MCUCR , ISC11);
	SET_BIT( MCUCR , ISC10);

	// close the tap
	GPIO_writePin(PORTD_ID, PIN7_ID, LOGIC_LOW);
	tap_flag = 0;             // change flag

}

void Open_Flame()
{
	/*trigger the external INT0 with falling edge*/
	SET_BIT( MCUCR , ISC01);
	CLEAR_BIT( MCUCR , ISC00);

	if(servo_angle_value == 1){ //this condition if the door is opened and waiting to press "*"
		fire_flag=1;
		Buzzer_ON();
		_delay_ms(2);
	}else{
		fire_flag=1; // change flag
		SERVO_rotateAngle(ROTATE_90_ANGLE);
		_delay_ms(2);
		Buzzer_ON();
		_delay_ms(2);
	}
}

void Close_Flame()
{
	/*trigger the external INT0 with rising edge*/
	SET_BIT( MCUCR , ISC01);
	SET_BIT( MCUCR , ISC00);

	if(servo_angle_value == 1){
		fire_flag=0;
		Buzzer_Off();
		_delay_ms(2);
	}else{
		//GPIO_writePin(PORTC_ID, PIN7_ID, LOGIC_LOW);
		fire_flag=0;             // change flag
		SERVO_rotateAngle(ROTATE_ZERO_ANGLE);
		_delay_ms(2);
		Buzzer_Off();
		_delay_ms(2);
	}

}


/*********************** Interrupt Service Routines ****************************/


ISR(INT0_vect)
{
	CLEAR_BIT( GICR , INT0 );  // disable external interrupt pin INT0

	if(fire_flag == 0){
		Open_Flame();
	}else if(fire_flag == 1){
		Close_Flame();
	}

	SET_BIT( GICR , INT0 );  // enable external interrupt pin INT0
}

ISR(INT1_vect)
{
	CLEAR_BIT( GICR , INT1 );  // disable external interrupt pin INT1

	if(tap_flag == 0){
		Open_Tap();
	}else if(tap_flag == 1){
		Close_Tap();
	}

	SET_BIT( GICR , INT1 );  // enable external interrupt pin INT1
}

ISR(INT2_vect)
{

}

/***********************  Functions Definitions  ***************************/



void INT0_UNIT()
{
	CLEAR_BIT( DDRD , PD2); //define the direction of PD2 as input
//	SET_BIT( PORTD , PD2 ); //to enable the internal pull-up resistor

	/*trigger the external INT0 with rising edge*/
	SET_BIT( MCUCR , ISC01);
	SET_BIT( MCUCR , ISC00);

	SET_BIT( GICR , INT0 );  // enable external interrupt pin INT0
	SET_BIT( GIFR , INTF0 ); // clear the flag of INT0
}

void INT1_UNIT()
{
	CLEAR_BIT( DDRD , PD3 ); //define the direction of PD3 as input

	/*trigger the external INT1 with rising edge*/
	SET_BIT( MCUCR , ISC11);
	SET_BIT( MCUCR , ISC10);

	SET_BIT( GICR , INT1 ); // enable external interrupt pin INT1
	SET_BIT( GIFR , INTF1 ); //clear the flag of INT1

}

/*
void INT2_UNIT()
{
	CLEAR_BIT( DDRB , PB2 ); //define the direction of PB2 as input

	SET_BIT( MCUCSR , ISC2); //trigger the external INT2 with rising edge

	SET_BIT( GICR , INT2 ); // enable external interrupt pin INT2
	SET_BIT( GIFR , INTF2 ); //clear the flag of INT2
}
*/

