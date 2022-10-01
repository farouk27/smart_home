 /******************************************************************************
 *
 * Module: TIMER0
 *
 * File Name: timer0.h
 *
 * Description: Header file for the timer0 AVR driver
 *
 * Author: Hesham Ashraf Kamel
 *
 *******************************************************************************/

#ifndef TIMER2_H_
#define TIMER2_H_

#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 ******************************************************************************/



typedef enum{
	NORMAL_MODE_TIMER2 , COMPARE_MODE_TIMER2
}TIMER2_MODE;

typedef enum{
	NO_CLOCK , F_CPU_1 , F_CPU_8 ,F_CPU_32, F_CPU_64 , F_CPU_128, F_CPU_256 , F_CPU_1024 , EXTERNAL_FALLING , \
	EXTERNAL_RISING
}TIMER2_PRESCALER;

typedef struct{
	TIMER2_MODE timer2Mode;
	uint8 initialValue;
	uint8 compareValue;
	TIMER2_PRESCALER timer2Prescaler;
}TIMER0_CONFIG;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 [Description]: function used to initiate the timer0 by passing structure contains:
 1- mode of the timer.
 2- the TCNT0 initial value.
 3- the ICR0 initial value.
 4- the required prescaller.
 */
void TIMER2_init(TIMER0_CONFIG *timer2);


/*
 [Description]: to de-initiate the timer0.
 */
void TIMER2_DEINIT(void);
#endif /* TIMER2_H_ */
