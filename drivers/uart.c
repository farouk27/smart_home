 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 * Author: Mohamed Yehia
 *
 *******************************************************************************/

#include "uart.h"
#include "avr/io.h" /* To use the UART Registers */
#include <avr/interrupt.h>
#include "gpio.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include <avr/eeprom.h>

uint8 ext_room1_value = 0;
uint8 ext_room2_value = 0;

uint8 room1_address = 0x50;
uint8 room2_address = 0x55;
/*******************************************************************************
 *                      ISR Definitions                                  *
 *******************************************************************************/

ISR(USART_RXC_vect)
{
	if (UDR == '1'){
		/*to open the led of room 1 */
		GPIO_writePin(PORTD_ID, PIN4_ID, LOGIC_HIGH);
		ext_room1_value = 1;
		eeprom_write_byte(room1_address, ext_room1_value);
	}else if (UDR == '2'){
		/*to close the led of room 1 */
		GPIO_writePin(PORTD_ID, PIN4_ID, LOGIC_LOW);
		ext_room1_value = 0;
		eeprom_write_byte(room1_address, ext_room1_value);
	}else if (UDR == '3'){
		/*to open the led of room 2 */
		GPIO_writePin(PORTD_ID, PIN6_ID, LOGIC_HIGH);
		ext_room2_value = 1;
		eeprom_write_byte(room2_address, ext_room2_value);
	}else if (UDR == '4'){
		/*to close the led of room 2 */
		GPIO_writePin(PORTD_ID, PIN6_ID, LOGIC_LOW);
		ext_room2_value = 0;
		eeprom_write_byte(room2_address, ext_room2_value);
	}
}


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(uint32 baud_rate)
{
	uint16 ubrr_value = 0;

	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);

	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/ 
	UCSRB = (1<<RXEN) | (1<<RXCIE);
	
	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1:0  = 00 Disable parity bit
	 * USBS    = 0 One stop bit
	 * UCSZ1:0 = 11 For 8-bit data mode
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/ 	
	UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1); 
	
	/* Calculate the UBRR register value */
	ubrr_value = (uint16)(((F_CPU / (baud_rate * 8UL))) - 1);

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;
}


/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
//uint8 UART_recieveByte(void)
//{
//	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
//	while(BIT_IS_CLEAR(UCSRA,RXC)){}
//
//	/*
//	 * Read the received data from the Rx buffer (UDR)
//	 * The RXC flag will be cleared after read the data
//	 */
//    return UDR;
//}
