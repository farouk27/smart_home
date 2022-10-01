 /******************************************************************************
 *
 * Module: TIMER0
 *
 * File Name: timer0.c
 *
 * Description: source file for the timer0 AVR driver
 *
 * Author: Hesham Ashraf Kamel
 *
 *******************************************************************************/
#include "timer0.h"
#include <avr/interrupt.h>
#include "common_macros.h"
#include "gpio.h"
#include <avr/io.h>
#include <util/delay.h>


/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

uint8 alarm_close_tick = 0;

extern uint8 ext_buzzerAlarm;      //accumulator for 3 attempts

ISR(TIMER0_OVF_vect){
		alarm_close_tick++;

		/*240 ticks to get 60 seconds*/
		if(alarm_close_tick == 240)
		{
			CLEAR_BIT(PORTC , PC7);
			alarm_close_tick = 0;
			ext_buzzerAlarm = 0;
		}
}

ISR(TIMER0_COMP_vect){

}

/*
 * [Function Name]: TIMER0_init
 *  [Description]: function used to initiate the timer0 by passing structure contains:
 *1- mode of the timer.
 *2- the TCNT0 initial value.
 *3- the ICR0 initial value.
 *4- the required prescaller.
 * [Args]: takes TIMER0_CONFIG.
 * [Returns]: void.
 */
void TIMER0_init(TIMER0_CONFIG *timer0){

	/*
	 * FOC0 = 0 if the mode isn't PWM mode
	 */
	CLEAR_BIT(TCCR0 , FOC0);

	switch(timer0 -> timer0Mode){
	case NORMAL_MODE_TIMER0:
		/*
		 * to set the normal mode:
		 * 1- WGM00 = 0 , WGM01 = 0
		 * 2- COM00 = 0 , COM 01 = 0
		 * 3- clear the compare interrupt enable OCIE = 0
		 * 4- set the overflow interrupt enable TOIE = 1
		 */
		CLEAR_BIT(TCCR0 , WGM00);
		CLEAR_BIT(TCCR0 , WGM01);
		CLEAR_BIT(TCCR0 , COM00);
		CLEAR_BIT(TCCR0 , COM01);
		CLEAR_BIT(TIMSK , OCIE0);
		SET_BIT(TIMSK , TOIE0);
		break;

	case COMPARE_MODE_TIMER0:
		/*
		 * to set the compare mode:
		 * 1- WGM00 = 0 , WGM01 = 1
		 * 2- COM00 = 0 , COM 01 = 1
		 * 3- set the compare interrupt enable OCIE = 1
		 * 4- clear the overflow interrupt enable TOIE = 0
		 */
		CLEAR_BIT(TCCR0 , WGM00);
		SET_BIT(TCCR0 , WGM01);
		CLEAR_BIT(TCCR0 , COM00);
		SET_BIT(TCCR0 , COM01);
		CLEAR_BIT(TIMSK , TOIE0);
		SET_BIT(TIMSK , OCIE0);
		break;
	}

	/*select the prescaler*/
	TCCR0 = ((TCCR0 & 0XF8 ) | ((timer0 -> timer0Prescaler) & 0X07 ));

	/*initiate the TCNT0*/
	TCNT0 = (timer0 -> initialValue);

	/*initiate the OCR0*/
	OCR0 = (timer0 -> compareValue);

	/*enable the global interrupt*/
	SREG |= (1<<7);
}

/*
 * [Function Name]: TIMER0_DEINIT
 * [Description]: to de-initiate the timer0.
 * [Args]: takes void.
 * [Returns]: void.
 */
void TIMER0_DEINIT(void){
	TCCR0 = 0;
	TCNT0 = 0;
	OCR0 = 0;
	CLEAR_BIT(TIMSK , OCIE0);
	CLEAR_BIT(TIMSK , TOIE0);
}
