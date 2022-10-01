/*
 * servo.c
 *
 *  Created on: Apr 27, 2022
 *      Author: yahia
 */
#include <avr/io.h>
#include "servo.h"
#include "std_types.h"
#include "common_macros.h"

void SERVO_rotateAngle(uint16 duty_cycle)
{
	TCNT1 = 0;
	ICR1 = 2499;
	OCR1A = duty_cycle;
	SET_REG_BITS(TCCR1A , 0X82);
	SET_REG_BITS(TCCR1B , 0X1A);

}

