#include "unity.h"
#include "SPI.h"

void setUp(void)
{
	UCA0CTLW0 = 0;
	UCA0BRW = 0;
	UCA0MCTLW = 0;
	UCA0IE = 0;
	UCA1IE = 0;
	UCB0IE = 0;
}

void tearDown(void)
{
}



void test_SPI_Master_3W_Init_should_AcceptValidInputs(void)
{
	/* Test General Conditions */
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_A0,0,0,UCSSEL__ACLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_A0,0,0,UCSSEL__ACLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_A1,250000,1000000,UCSSEL__ACLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_B0,1000000,8000000,UCSSEL__ACLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_B0,8000000,8000000,UCSSEL__ACLK));
	
	/* Test Different Valid Ports and Clocks */
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_A0,0,0,UCSSEL__ACLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_A1,0,0,UCSSEL__ACLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_B0,0,0,UCSSEL__ACLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_A0,0,0,UCSSEL__SMCLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_A1,0,0,UCSSEL__SMCLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_B0,0,0,UCSSEL__SMCLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_A0,0,0,UCSSEL__UCLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_A1,0,0,UCSSEL__UCLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_B0,0,0,UCSSEL__UCLK));
	
	
	/* Test Different Valid Ports & Clocks and Frequencies */
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_A0,1000000,8000000,UCSSEL__ACLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_A1,1000000,8000000,UCSSEL__ACLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_B0,1000000,8000000,UCSSEL__ACLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_A0,1000000,8000000,UCSSEL__SMCLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_A1,1000000,8000000,UCSSEL__SMCLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_B0,1000000,8000000,UCSSEL__SMCLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_A0,1000000,8000000,UCSSEL__UCLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_A1,1000000,8000000,UCSSEL__UCLK));
	TEST_ASSERT_EQUAL(0,SPI_Master_3W_Init(SPI_B0,1000000,8000000,UCSSEL__UCLK));
}

void test_SPI_Master_3W_Init_should_RejectInvalidInputs(void)
{
	/* Reject Bad Ports */
	TEST_ASSERT_EQUAL(1,SPI_Master_3W_Init(3,0,0,UCSSEL__SMCLK));
	TEST_ASSERT_EQUAL(1,SPI_Master_3W_Init(255,0,0,UCSSEL__SMCLK));
	
	/* Reject Bad CLKS */
	TEST_ASSERT_EQUAL(1,SPI_Master_3W_Init(0,0,0,0x00FF));
	TEST_ASSERT_EQUAL(1,SPI_Master_3W_Init(0,0,0,0x0010));
	
	/* Reject Bit Rates Higher than Clk Freq */
	TEST_ASSERT_EQUAL(1,SPI_Master_3W_Init(SPI_A0,2000000,1000000,UCSSEL__ACLK));
	TEST_ASSERT_EQUAL(1,SPI_Master_3W_Init(SPI_A0,8000000,4000000,UCSSEL__ACLK));
	TEST_ASSERT_EQUAL(1,SPI_Master_3W_Init(SPI_A0,16000000,8000000,UCSSEL__ACLK));
}

void test_SPI_Master_3W_Init_should_SetCorrectRegisters(void)
{
	
	
}



void test_SPI_Master_EnableInterrupts_should_AcceptValidPorts(void)
{
	TEST_ASSERT_EQUAL(0,SPI_Master_EnableInterrupts(SPI_A0,true,true));
	TEST_ASSERT_EQUAL(0,SPI_Master_EnableInterrupts(SPI_A1,true,true));
	TEST_ASSERT_EQUAL(0,SPI_Master_EnableInterrupts(SPI_B0,true,true));
}

void test_SPI_Master_EnableInterrupts_should_RejectInvalidPorts(void)
{
	TEST_ASSERT_EQUAL(1,SPI_Master_EnableInterrupts(3,false,false));
	TEST_ASSERT_EQUAL(1,SPI_Master_EnableInterrupts(127,false,false));
	TEST_ASSERT_EQUAL(1,SPI_Master_EnableInterrupts(255,false,false));
}

void test_SPI_Master_EnableInterrupts_should_RejectInvalidInterruptValues(void)
{
	TEST_ASSERT_EQUAL(1,SPI_Master_EnableInterrupts(SPI_A0,2,false));
	TEST_ASSERT_EQUAL(1,SPI_Master_EnableInterrupts(SPI_A0,false,2));
	TEST_ASSERT_EQUAL(1,SPI_Master_EnableInterrupts(SPI_A0,255,false));
	TEST_ASSERT_EQUAL(1,SPI_Master_EnableInterrupts(SPI_A0,false,255));
	TEST_ASSERT_EQUAL(1,SPI_Master_EnableInterrupts(SPI_A0,2,true));
	TEST_ASSERT_EQUAL(1,SPI_Master_EnableInterrupts(SPI_A0,true,2));
	TEST_ASSERT_EQUAL(1,SPI_Master_EnableInterrupts(SPI_A0,255,true));
	TEST_ASSERT_EQUAL(1,SPI_Master_EnableInterrupts(SPI_A0,true,255));
	
}

void test_SPI_Master_EnableInterrupts_should_SetValidInterrupts(void)
{
	TEST_ASSERT_EQUAL(0,SPI_Master_EnableInterrupts(SPI_A0,false,false));
	TEST_ASSERT_EQUAL(0,UCA0IE);
	

	
}