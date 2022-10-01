/*
 * buzzer.c
 *
 *  Created on: Oct 29, 2021
 *      Author: M.TAMER
 */

#include"gpio.h"
#include"buzzer.h"
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description:
 * initialize the BUZZER driver by :
 * set the direction of the buzzer pin.
 */
void Buzzer_Init(void)
{
	GPIO_setupPinDirection(BuzzerPort_ID,BuzzerPin_ID,PIN_OUTPUT);
	/* set the direction of the buzzer pin as output bin*/
}
/*
 * Description :
 * make the BUZZER ON mode
 */
void Buzzer_ON(void)
{
	GPIO_writePin(BuzzerPort_ID,BuzzerPin_ID,LOGIC_HIGH);/* give the PIN logic high */
}
/*
 * Description :
 * make the BUZZER OFF mode
 */
void Buzzer_Off(void)
{
	GPIO_writePin(BuzzerPort_ID,BuzzerPin_ID,LOGIC_LOW); /* give the PIN logic low */
}
