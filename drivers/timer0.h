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

#ifndef TIMER0_H_
#define TIMER0_H_

#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 ******************************************************************************/



typedef enum{
	NORMAL_MODE_TIMER0 , COMPARE_MODE_TIMER0
}TIMER0_MODE;

typedef enum{
	NO_CLOCK_0 , F_CPU0_1 , F_CPU0_8 , F_CPU0_64 , F_CPU0_256 , F_CPU0_1024 , EXTERNAL_FALLING_0 , \
	EXTERNAL_RISING_0
}TIMER0_PRESCALER;

typedef struct{
	TIMER0_MODE timer0Mode;
	uint8 initialValue;
	uint8 compareValue;
	TIMER0_PRESCALER timer0Prescaler;
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
void TIMER0_init(TIMER0_CONFIG *timer0);


/*
 [Description]: to de-initiate the timer0.
 */
void TIMER0_DEINIT(void);
#endif /* TIMER0_H_ */
