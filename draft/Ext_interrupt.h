/*
 * Ext_interrupt.h
 *
 *  Created on: Mar 12, 2022
 *      Author: Hesham Ashraf
 */

#ifndef EXT_INTERRUPT_H_
#define EXT_INTERRUPT_H_


/*Three functions to initialize the three external interrupts*/

/*void Open_Tap(void);
void Close_Tap(void);
void Open_Flame(void);
void Close_Flame(void);

void toggle_Flame();
void toggle_tap();*/


void INT0_UNIT(void);
void INT1_UNIT(void);
void INT2_UNIT(void);


#endif /* EXT_INTERRUPT_H_ */
