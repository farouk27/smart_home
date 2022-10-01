 /******************************************************************************
 *
 * Module: TIMER0
 *
 * File Name: timer2.c
 *
 * Description: source file for the timer0 AVR driver
 *
 * Author: Hesham Ashraf Kamel
 *
 *******************************************************************************/
#include "timer2.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "gpio.h"
#include "std_types.h"
#include "common_macros.h"
#include "servo.h"
#include "lcd.h"
#include "buzzer.h"
#include "uart.h"
#include <util/delay.h>


/*************************************S******************************************
 *                            functions                                 *
 *******************************************************************************/

extern uint8 servo_angle_value;
extern uint8 ext_theaf_flag;
extern uint8 servo_angle_address;

void Open_Tap()
{
	// open the tap
	GPIO_writePin(PORTD_ID, PIN7_ID, LOGIC_LOW);
}


void Close_Tap()
{
	// close the tap
	GPIO_writePin(PORTD_ID, PIN7_ID, LOGIC_HIGH);
}

void Open_Flame()
{
	if(servo_angle_value == 1){ //this condition if the door is opened and waiting to press "*"
		Buzzer_ON();
		_delay_ms(2);
	}else{
		SERVO_rotateAngle(ROTATE_90_ANGLE);
		_delay_ms(2);
		Buzzer_ON();
		_delay_ms(2);
	}
}

void Close_Flame()
{
	if(servo_angle_value == 1){
		Buzzer_Off();
		_delay_ms(2);
	}else{
		//GPIO_writePin(PORTC_ID, PIN7_ID, LOGIC_LOW);
		SERVO_rotateAngle(ROTATE_ZERO_ANGLE);
		_delay_ms(2);
		Buzzer_Off();
		_delay_ms(2);
	}

}


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
uint8 timer2_tick = 0;


ISR(TIMER2_OVF_vect){
	timer2_tick ++;
	if(timer2_tick == 2){
		//for flame
		if(BIT_IS_SET(PIND , PD2)){
			Open_Flame();
		}else{
			if(ext_theaf_flag == 0){
				Close_Flame();
			}else{
				SERVO_rotateAngle(ROTATE_ZERO_ANGLE);
			}
		}

		//for IR
		if(BIT_IS_SET(PIND , PD3)){
			Open_Tap();
		}else{
			Close_Tap();
		}

		timer2_tick = 0;
	}
}


/*
 * [Function Name]: TIMER2_init
 *  [Description]: function used to initiate the timer2 by passing structure contains:
 *1- mode of the timer.
 *2- the TCNT2 initial value.
 *3- the ICR2 initial value.
 *4- the required prescaller.
 * [Args]: takes TIMER0_CONFIG.
 * [Returns]: void.
 */
void TIMER2_init(TIMER2_CONFIG *timer2){

	/*
	 * FOC2 = 0 if the mode isn't PWM mode
	 */
	CLEAR_BIT(TCCR2 , FOC2);

	switch(timer2 -> timer2Mode){
	case NORMAL_MODE_TIMER2:
		/*
		 * to set the normal mode:
		 * 1- WGM20 = 0 , WGM21 = 0
		 * 2- COM20 = 0 , COM 21 = 0
		 * 3- clear the compare interrupt enable OCIE = 0
		 * 4- set the overflow interrupt enable TOIE = 1
		 */
		CLEAR_BIT(TCCR2 , WGM20);
		CLEAR_BIT(TCCR2 , WGM21);
		CLEAR_BIT(TCCR2 , COM20);
		CLEAR_BIT(TCCR2 , COM21);
		CLEAR_BIT(TIMSK , OCIE2);
		SET_BIT(TIMSK , TOIE2);
		break;

	case COMPARE_MODE_TIMER2:
		/*
		 * to set the compare mode:
		 * 1- WGM00 = 0 , WGM01 = 1
		 * 2- COM00 = 0 , COM 01 = 1
		 * 3- set the compare interrupt enable OCIE = 1
		 * 4- clear the overflow interrupt enable TOIE = 0
		 */
		CLEAR_BIT(TCCR0 , WGM20);
		SET_BIT(TCCR0 , WGM21);
		CLEAR_BIT(TCCR0 , COM20);
		SET_BIT(TCCR0 , COM21);
		CLEAR_BIT(TIMSK , TOIE2);
		SET_BIT(TIMSK , OCIE2);
		break;
	}
TCCR2 &= ~(0x07);

switch(timer2 -> timer2Prescaler){
case NO_CLOCK_2 :
	SET_REG_BITS(TCCR2 , 0X00);
	break;

case F_CPU2_1 :
	SET_REG_BITS(TCCR2 , 0X01);
	break;

case F_CPU2_8:
	SET_REG_BITS(TCCR2 , 0X02);
	break;

case F_CPU2_32:
	SET_REG_BITS(TCCR2 , 0X03);
	break;

case F_CPU2_64:
	SET_REG_BITS(TCCR2 , 0X04);
	break;

case F_CPU2_128:
	SET_REG_BITS(TCCR2 , 0X05);
	break;

case F_CPU2_256:
	SET_REG_BITS(TCCR2 , 0X06);
	break;

case F_CPU2_1024:
	SET_REG_BITS(TCCR2 , 0X07);
	break;
}

	/*initiate the TCNT0*/
	TCNT2 = (timer2 -> initialValue);

	/*initiate the OCR0*/
	OCR2 = (timer2 -> compareValue);

	/*enable the global interrupt*/
	SREG |= (1<<7);
}


/*
 * [Function Name]: TIMER0_DEINIT
 * [Description]: to de-initiate the timer0.
 * [Args]: takes void.
 * [Returns]: void.
 */
void TIMER2_DEINIT(void){
	TCCR2 = 0;
	TCNT2 = 0;
	OCR2 = 0;
	CLEAR_BIT(TIMSK , TOIE2);
	CLEAR_BIT(TIMSK , OCIE2);
}
