/*
 * buzzer.h
 *
 *  Created on: Oct 29, 2021
 *      Author: M.TAMER
 */

#ifndef BUZZER_H_
#define BUZZER_H_
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define BuzzerPort_ID PORTC_ID
#define BuzzerPin_ID PIN7_ID
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description:
 * initialize the BUZZER driver by :
 * set the direction of the buzzer pin.
 */
void Buzzer_Init(void);
/*
 * Description :
 * make the BUZZER ON mode
 */
void Buzzer_ON(void);
/*
 * Description :
 * make the BUZZER OFF mode
 */
void Buzzer_Off(void);
#endif /* BUZZER_H_ */
