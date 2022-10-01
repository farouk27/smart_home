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
	NO_CLOCK_2 , F_CPU2_1 , F_CPU2_8 ,F_CPU2_32, F_CPU2_64 , F_CPU2_128, F_CPU2_256 , F_CPU2_1024
}TIMER2_PRESCALER;

typedef struct{
	TIMER2_MODE timer2Mode;
	uint8 initialValue;
	uint8 compareValue;
	TIMER2_PRESCALER timer2Prescaler;
}TIMER2_CONFIG;

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
void TIMER2_init(TIMER2_CONFIG *timer2);


/*
 [Description]: to de-initiate the timer0.
 */
void TIMER2_DEINIT(void);
#endif /* TIMER2_H_ */
