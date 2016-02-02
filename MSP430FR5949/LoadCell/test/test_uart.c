#include "unity.h"
#include "uart.h"
#include "../test/msp430fr5969.h"

void setUp(void)
{

	P1SEL0 = 0;
	P2SEL0 = 0;
	P3SEL0 = 0;
	P4SEL0 = 0;

	P1SEL1 = 0;
	P2SEL1 = 0;
	P3SEL1 = 0;
	P4SEL1 = 0;

}

void tearDown(void)
{
}


/************************** UART_Init Testing **************************/
void test_UART_Init_should_RejectInvalidModules(void)
{
	TEST_ASSERT_EQUAL(1, UART_Init(2,4800,CLK_32768,UART_CLK_SMCLK));
}

void test_UART_Init_should_AcceptValidModules(void)
{
	TEST_ASSERT_EQUAL(0, UART_Init(UART_A0,UART_BAUD_4800,CLK_32768,UART_CLK_SMCLK));
	TEST_ASSERT_EQUAL(0, UART_Init(UART_A1,UART_BAUD_4800,CLK_32768,UART_CLK_SMCLK));
}


void test_UART_Init_should_RejectInvalidBaudRates(void)
{
	TEST_ASSERT_EQUAL(UART_FAIL,UART_Init(UART_A0,10,CLK_32768,UART_CLK_SMCLK));
	TEST_ASSERT_EQUAL(UART_FAIL,UART_Init(UART_A0,25,CLK_32768,UART_CLK_SMCLK));
	TEST_ASSERT_EQUAL(UART_FAIL,UART_Init(UART_A0,255,CLK_32768,UART_CLK_SMCLK));
}


void test_UART_Init_should_AcceptValidBaudRates(void)
{
	TEST_ASSERT_EQUAL(UART_OK,UART_Init(UART_A0,UART_BAUD_2400,CLK_32768,UART_CLK_SMCLK));
	TEST_ASSERT_EQUAL(UART_OK,UART_Init(UART_A1,UART_BAUD_57600,CLK_32768,UART_CLK_SMCLK));
	TEST_ASSERT_EQUAL(UART_OK,UART_Init(UART_A0,UART_BAUD_115200,CLK_32768,UART_CLK_SMCLK));
}

void test_UART_Init_should_RejectInvalidClockSource(void)
{
	TEST_ASSERT_EQUAL(UART_FAIL,UART_Init(UART_A0,UART_BAUD_2400,CLK_32768,3));
}

void test_UART_Init_should_AcceptValidClockSource(void)
{
	TEST_ASSERT_EQUAL(UART_OK, UART_Init(UART_A0,UART_BAUD_2400,CLK_32768,UART_CLK_SMCLK));
	TEST_ASSERT_EQUAL(UART_OK, UART_Init(UART_A0,UART_BAUD_2400,CLK_32768,UART_CLK_ACLK));
	TEST_ASSERT_EQUAL(UART_OK, UART_Init(UART_A0,UART_BAUD_2400,CLK_32768,UART_CLK_UCLK));
}

void test_UART_Init_should_RejectInvalidClockFrequency(void)
{
	TEST_ASSERT_EQUAL(UART_FAIL,UART_Init(UART_A1,UART_BAUD_2400,0,UART_CLK_UCLK));
}

/************************** UART_Read Testing **************************/
void test_UART_Read_should_RejectInvalidPorts(void)
{
	uint8_t uartValue;
	TEST_ASSERT_EQUAL(UART_FAIL,UART_Read(&uartValue,3));
	TEST_ASSERT_EQUAL(UART_FAIL,UART_Read(&uartValue,255));
}


void test_UART_Read_should_AcceptValidPorts(void)
{
	uint8_t uartValue;
	TEST_ASSERT_EQUAL(UART_OK,UART_Read(&uartValue,UART_A0));
	TEST_ASSERT_EQUAL(UART_OK,UART_Read(&uartValue,UART_A1));
}

void test_UART_Read_should_ReturnCorrectBufferValues(void)
{
	uint8_t uartValue;
	UCA0RXBUF = 0x001F;
	TEST_ASSERT_EQUAL(UART_OK,UART_Read(&uartValue,UART_A0));
	TEST_ASSERT_EQUAL_HEX8(0x1F,uartValue);

	UCA1RXBUF = 0xFF1F;
	TEST_ASSERT_EQUAL(UART_OK,UART_Read(&uartValue,UART_A1));
	TEST_ASSERT_EQUAL_HEX8(0x001F,uartValue);
}

/************************ UART_WriteChar Testing ************************/
void test_UART_WriteChar_should_AcceptValidPorts(void)
{
	TEST_ASSERT_EQUAL(UART_OK,UART_WriteChar(5,UART_A0));
	TEST_ASSERT_EQUAL(UART_OK,UART_WriteChar(255,UART_A1));
}

void test_UART_WriteChar_should_RejectInvalidPorts(void)
{
	TEST_ASSERT_EQUAL(UART_FAIL,UART_WriteChar(0,3));
	TEST_ASSERT_EQUAL(UART_FAIL,UART_WriteChar(0,255));
}

void test_UART_WriteChar_should_WriteToCorrectBuffer(void)
{
	TEST_ASSERT_EQUAL(UART_OK,UART_WriteChar(7,UART_A0));
	TEST_ASSERT_EQUAL_HEX8(UCA0TXBUF,7);

	TEST_ASSERT_EQUAL(UART_OK,UART_WriteChar(255,UART_A1));
	TEST_ASSERT_EQUAL_HEX8(UCA1TXBUF,255);
}


/************************** UART_Write Testing **************************/
void test_UART_Write_should_AcceptValidPorts(void)
{
	uint8_t uartValue[] = {1,8,255,0,100};
	TEST_ASSERT_EQUAL(UART_OK,UART_Write(&uartValue[0],DIMENSION_OF(uartValue),UART_A0));
	TEST_ASSERT_EQUAL(UART_OK,UART_Write(&uartValue[0],DIMENSION_OF(uartValue),UART_A1));
}

void test_UART_Write_should_RejectInvalidPorts(void)
{
	uint8_t uartValue[] = {1,8,255,0,100};
	TEST_ASSERT_EQUAL(UART_FAIL,UART_Write(&uartValue[0],DIMENSION_OF(uartValue),3));
	TEST_ASSERT_EQUAL(UART_FAIL,UART_Write(&uartValue[0],DIMENSION_OF(uartValue),255));
}

void test_UART_Write_should_WriteEntireString_if_NoNullValue(void)
{
	uint8_t uartValue[] = {1,3,5,255};
	TEST_ASSERT_EQUAL(UART_OK,UART_Write(&uartValue[0],DIMENSION_OF(uartValue),UART_A0));
	TEST_ASSERT_EQUAL_HEX8('\n',UCA0TXBUF);
}

/********************* UART Init Testing *******************************/
void test_UART_Init_should_CompleteUCBRSRegistersCorrectly(void)
{
	TEST_ASSERT_EQUAL(UART_OK, UART_Init(UART_A0, UART_BAUD_9600, CLK_32768, UART_CLK_ACLK ));
	TEST_ASSERT_EQUAL_HEX16(3,UCA0BRW);
	TEST_ASSERT_EQUAL_HEX16(0x9200, UCA0MCTLW);

	TEST_ASSERT_EQUAL(UART_OK, UART_Init(UART_A1, UART_BAUD_57600, CLK_8000000, UART_CLK_SMCLK ));
	TEST_ASSERT_EQUAL_HEX16(4,UCA1BRW);
	TEST_ASSERT_EQUAL_HEX16(0xF7A1, UCA1MCTLW);
}

void test_UART_Init_should_Fail_if_BadSettingsArePassed(void)
{
	TEST_ASSERT_EQUAL(UART_FAIL, UART_Init(UART_A0, UART_BAUD_9600, 1400, UART_CLK_ACLK));
}


