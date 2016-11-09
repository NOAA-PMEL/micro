/** @file uart.h
 *  @brief
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date March 23, 2016
 *  @version 0.1.1
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *	@note
 *
 *  @bug  No known bugs
 */



#ifndef UART_H_
#define UART_H_

/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/
#include <msp430fr5949.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
/************************************************************************
*							HEADER FILES
************************************************************************/
#include "main.h"
#include "buffer_c.h"
#include "timer.h"

/************************************************************************
*							MACROS
************************************************************************/
#define UART_OK		(0)
#define UART_FAIL	(1)

/* If Carriage Return is required 1, otherwise 0*/
#define CARRIAGE_RETURN (1)

/* UART USCI Port Definitions */
#define UART_A0			(0)
#define UART_A1			(1)

/* UART Pin Definitions */
#define UART_A0_TX		(BIT0)
#define UART_A0_RX		(BIT1)
#define UART_A1_TX		(BIT5)
#define UART_A1_RX		(BIT6)

/* Clk Definitions */
#define UART_CLK_UCLK	(0)
#define UART_CLK_ACLK	(1)
#define UART_CLK_SMCLK	(2)

/* Baudrate Definitions */
#define UART_BAUD_1200		(1200)
#define UART_BAUD_2400		(2400)
#define UART_BAUD_4800		(4800)
#define UART_BAUD_9600		(9600)
#define UART_BAUD_14400		(14400)
#define UART_BAUD_19200		(19200)
#define UART_BAUD_28800		(28800)
#define UART_BAUD_38400		(38400)
#define UART_BAUD_57600		(57600)
#define UART_BAUD_115200	(115200)
#define UART_BAUD_230400	(230400)
#define UART_BAUD_460800	(460800)

#define BAUDRATE_OK			(0)
#define BAUDRATE_FAIL		(1)

/* Clock Definitions */
#define CLK_32768			(32768)
#define CLK_1000000			(1000000)
#define CLK_1048576			(1048576)
#define CLK_4000000			(4000000)
#define CLK_4194304			(4194304)
#define CLK_8000000			(8000000)

#define CLOCK_OK			(0)
#define CLOCK_FAIL			(1)

//#define UART_ACK            (0x06)
//#define UART_NACK           (0x15)

/* */
#define UART_puts(x)        (UART_Write((uint8_t*)&x,LENGTH_OF(x),UART_A1))
#define UART_putc(x)        (UART_WriteChar((uint8_t*)&x,UART_A1))
#define UART_getc(x)        (UART_Read((uint8_t*)&x,UART_A1))
#define UART_ACK()          (UART_WriteACK(UART_A1))
#define UART_NACK()         (UART_WriteNACK(UART_A1))
#define UART_Newline()      (UART_WriteNewline(UART_A1))

/* Ocean Server Macros */
#define OS_puts(x)          (UART_Write((uint8_t*)&x,LENGTH_OF(x),UART_A0))
#define OS_putc(x)          (UART_WriteChar((uint8_t)x,UART_A0))
#define OS_getc(x)          (UART_Read(uint8_t*)&x,UART_A1)

/* T-FLEX Macro */
#define TFLEX_puts(x)       (UART_Write((uint8_t*)&x,LENGTH_OF(x),UART_A1))
#define TFLEX_putc(x)       (UART_WriteChar((uint8_t)x,UART_A1))
#define TFLEX_getc(x)       (UART_Read((uint8_t*)&x,UART_A1))

/* */
typedef struct {
  uint8_t Echo;
  uint8_t CancelCnt;
  uint8_t CancelFlag;
  uint8_t SystemBusy;
  CircularBufferC_s Buffer;
  TimerMS_t *Timer;
}UART_t;


extern UART_t UART;
/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/

/** @brief Initialize the T-FLEX UART
 *
*	Sets the T-FLEX UART connection at 9600 Baud on UART A1
 *
 *  @param None
 *
 *  @return None
 */
void TFLEX_UART_Init(void);

/** @brief Initialize the T-FLEX UART
 *
*	Sets the T-FLEX UART connection at 9600 Baud on UART A1
 *
 *  @param None
 *
 *  @return None
 */
void OS5000S_UART_Init(void);

void OS5000S_UART_Halt(void);

/** @brief Reads from UART
 *
 *	Reads one character from UART port
 *
 *  @param *value Pointer to char variable
 *  @param Port UART Port reading from
 *
 *  @return Status
 */
uint8_t UART_Read(uint8_t *value, uint8_t Port);

/** @brief Write one char to port
 *
 *	Writes only one char to port
 *
 *  @param value char value to transmit
 *  @param Port UART Port writing to
 *
 *  @return none
 */
uint8_t UART_WriteChar(uint8_t value, uint8_t Port);

/** @brief Write string to UART
 *
 *	Write a char array to the selected UART port
 *
 *  @param *value Pointer to the character array
 *  @param length length of string
 *  @param Port UART port selected
 *
 *  @return none
 */
uint8_t UART_Write(uint8_t *value, uint16_t length, uint8_t Port);

/** @brief Write string to UART with terminating Null
 *
 *	Write a char array to the selected UART port
 *  and follow with a NULL value
 *
 *  @param *value Pointer to the character array
 *  @param length length of string
 *  @param Port UART port selected
 *
 *  @return none
 */
uint8_t UART_WriteIncludeNull(uint8_t *value, uint8_t length, uint8_t Port);

/** @brief Write the ACK character to the specifed Port 
 * 
 * Writes the ACK (0x06) character to the port specified 
 *
 * @param Port Port to write to
 * 
 * @return None
 */
uint8_t UART_WriteACK(uint8_t Port);

/** @brief Write the NACK character to the specifed Port 
 * 
 * Writes the ACK (0x15) character to the port specified 
 *
 * @param Port Port to write to
 * 
 * @return None
 */ 
uint8_t UART_WriteNACK(uint8_t Port);

/** @brief Write the Newline character to the specifed Port 
 * 
 * Writes the newline (0x15) character to the port specified 
 *
 * @param Port Port to write to
 * 
 * @return None
 */ 
uint8_t UART_WriteNewline(uint8_t Port);


#endif

