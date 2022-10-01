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
#include "common_macros.h"
#include "gpio.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer2.h"


/*************************************S******************************************
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
void TIMER2_init(TIMER0_CONFIG *timer2){

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



	/*initiate the TCNT0*/
	TCNT2 = (timer2 -> initialValue);

	/*initiate the OCR0*/
	OCR2 = (timer2 -> compareValue);

	/*enable the global interrupt*/
	SREG |= (1<<7);
}

/*
 * [Function Name]: TIMER0_setCallBack
 * [Description]: this function is used to set the function that should be processed at
 * the ISR.
 * [Args]: takes pointer to a function.
 * [Returns]: void.
 */


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
