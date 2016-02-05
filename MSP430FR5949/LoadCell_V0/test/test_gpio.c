#include "unity.h"
#include "gpio.h"
#include "../test/msp430fr5969.h"


void setUp(void)
{
	GPIO_Init();
}

void tearDown(void)
{
}

void test_GPIO_Init_should_ClearPortRegisters(void)
{
	TEST_ASSERT_EQUAL(0,GPIO_Init());
	TEST_ASSERT_EQUAL_HEX16(0,P1OUT);
	TEST_ASSERT_EQUAL_HEX16(0,P2SEL0);
	TEST_ASSERT_EQUAL_HEX16(0,P3SEL1);
	TEST_ASSERT_EQUAL_HEX16(0,P4REN);
	TEST_ASSERT_EQUAL_HEX16(0,P3OUT);
}

void test_GPIO_SetPinAsOutput_should_AcceptValidPins(void)
{
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsOutput(1,1));
	TEST_ASSERT_EQUAL_HEX16(BIT_TO_MASK(1),P1DIR);
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsOutput(2,5));
	TEST_ASSERT_EQUAL_HEX16(BIT_TO_MASK(5),P2DIR);
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsOutput(3,11));
	TEST_ASSERT_EQUAL_HEX16(BIT_TO_MASK(11),P3DIR);
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsOutput(4,15));
	TEST_ASSERT_EQUAL_HEX16(BIT_TO_MASK(15),P4DIR);


	GPIO_Init();

	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsOutput(2,5));
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsOutput(2,6));
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsOutput(2,7));
	TEST_ASSERT_EQUAL_HEX16(0x0E0,P2DIR);

}

void test_GPIO_SetPinAsOutput_should_RejectInvalidPins(void)
{
	TEST_ASSERT_EQUAL(1,GPIO_SetPinAsOutput(1,16));
	TEST_ASSERT_EQUAL(1,GPIO_SetPinAsOutput(2,32));
	TEST_ASSERT_EQUAL(1,GPIO_SetPinAsOutput(3,127));
}

void test_GPIO_SetPinAsOutput_should_RejectInvalidPorts(void)
{
	TEST_ASSERT_EQUAL(1,GPIO_SetPinAsOutput(0,5));
	TEST_ASSERT_EQUAL(1,GPIO_SetPinAsOutput(5,5));
	TEST_ASSERT_EQUAL(1,GPIO_SetPinAsOutput(11,5));
}

void test_GPIO_SetPin_should_AcceptValidInputs(void)
{
	GPIO_SetPinAsOutput(1,0);
	TEST_ASSERT_EQUAL(0,GPIO_SetPin(1,0));
	TEST_ASSERT_EQUAL_HEX16(BIT_TO_MASK(0),P1OUT);


	GPIO_SetPinAsOutput(2,5);
	TEST_ASSERT_EQUAL(0,GPIO_SetPin(2,5));
	TEST_ASSERT_EQUAL_HEX16(BIT_TO_MASK(5),P2OUT);

	GPIO_SetPinAsOutput(3,13);
	TEST_ASSERT_EQUAL(0,GPIO_SetPin(3,13));
	TEST_ASSERT_EQUAL_HEX16(BIT_TO_MASK(13),P3OUT);

	GPIO_SetPinAsOutput(4,3);
	TEST_ASSERT_EQUAL(0,GPIO_SetPin(4,3));
	TEST_ASSERT_EQUAL_HEX16(BIT_TO_MASK(3),P4OUT);
}


void test_GPIO_ClearPin_should_AcceptValidInputs(void)
{
	P1OUT = 0xFFFF;
	GPIO_SetPinAsOutput(1,0);
	TEST_ASSERT_EQUAL(0,GPIO_ClearPin(1,0));
	TEST_ASSERT_EQUAL(0xFFFE,P1OUT);

	P2OUT = 0xFFFF;
	GPIO_SetPinAsOutput(2,2);
	TEST_ASSERT_EQUAL(0,GPIO_ClearPin(2,2));
	TEST_ASSERT_EQUAL(0xFFFB,P2OUT);

	P3OUT = 0xFFFF;
	GPIO_SetPinAsOutput(3,10);
	TEST_ASSERT_EQUAL(0,GPIO_ClearPin(3,10));
	TEST_ASSERT_EQUAL(0xFBFF,P3OUT);

	P4OUT = 0xFFFF;
	GPIO_SetPinAsOutput(4,15);
	TEST_ASSERT_EQUAL(0,GPIO_ClearPin(4,15));
	TEST_ASSERT_EQUAL(0x7FFF,P4OUT);
}

void test_GPIO_SetAllPortOutputPinsLow_should_RejectInvalidPorts(void)
{
	TEST_ASSERT_EQUAL(1,GPIO_SetAllPortOutputPinsLow(0));
	TEST_ASSERT_EQUAL(1,GPIO_SetAllPortOutputPinsLow(5));
}

void test_GPIO_SetAllPortOutputPinsLow_should_AcceptValidPort(void)
{
	P1DIR = 0xFFFF;
	P1OUT = 0xFFFF;
	TEST_ASSERT_EQUAL(0,GPIO_SetAllPortOutputPinsLow(1));
	TEST_ASSERT_EQUAL_HEX16(0x0000,P1OUT);

	P2DIR = 0xFFFF;
	P2OUT = 0xFFFF;
	TEST_ASSERT_EQUAL(0,GPIO_SetAllPortOutputPinsLow(2));
	TEST_ASSERT_EQUAL_HEX16(0x0000,P2OUT);

	P3DIR = 0xFFFF;
	P3OUT = 0xFFFF;
	TEST_ASSERT_EQUAL(0,GPIO_SetAllPortOutputPinsLow(3));
	TEST_ASSERT_EQUAL_HEX16(0x0000,P3OUT);

	P4DIR = 0xFFFF;
	P4OUT = 0xFFFF;
	TEST_ASSERT_EQUAL(0,GPIO_SetAllPortOutputPinsLow(4));
	TEST_ASSERT_EQUAL_HEX16(0x0000,P4OUT);

}

void test_GPIO_SetAllPortOutputPinsHigh_should_RejectInvalidPorts(void)
{
	TEST_ASSERT_EQUAL(1,GPIO_SetAllPortOutputPinsHigh(0));
	TEST_ASSERT_EQUAL(1,GPIO_SetAllPortOutputPinsHigh(5));
}

void test_GPIO_SetAllPortOutputPinsHigh_should_AcceptValidPort(void)
{
	P1DIR = 0xFFFF;
	P1OUT = 0x0000;
	TEST_ASSERT_EQUAL(0,GPIO_SetAllPortOutputPinsHigh(1));
	TEST_ASSERT_EQUAL_HEX16(0xFFFF,P1OUT);

	P2DIR = 0xFFFF;
	P2OUT = 0x0000;
	TEST_ASSERT_EQUAL(0,GPIO_SetAllPortOutputPinsHigh(2));
	TEST_ASSERT_EQUAL_HEX16(0xFFFF,P2OUT);

	P3DIR = 0xFFFF;
	P3OUT = 0x0000;
	TEST_ASSERT_EQUAL(0,GPIO_SetAllPortOutputPinsHigh(3));
	TEST_ASSERT_EQUAL_HEX16(0xFFFF,P3OUT);

	P4DIR = 0xFFFF;
	P4OUT = 0x0000;
	TEST_ASSERT_EQUAL(0,GPIO_SetAllPortOutputPinsHigh(4));
	TEST_ASSERT_EQUAL_HEX16(0xFFFF,P4OUT);

}


void test_GPIO_SetPinAsInput_should_AcceptValidValues(void)
{
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsInput(1,0));
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsInput(2,5));
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsInput(3,12));
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsInput(4,15));

	P1DIR = 0xFFFF;
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsInput(1,0));
	TEST_ASSERT_EQUAL_HEX16(0xFFFE,P1DIR);
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsInput(1,1));
	TEST_ASSERT_EQUAL_HEX16(0xFFFC,P1DIR);
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsInput(1,15));
	TEST_ASSERT_EQUAL_HEX16(0x7FFC,P1DIR);

	P2DIR = 0xFFFF;
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsInput(2,2));
	TEST_ASSERT_EQUAL_HEX16(0xFFFB,P2DIR);
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsInput(2,6));
	TEST_ASSERT_EQUAL_HEX16(0xFFBB,P2DIR);
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsInput(2,10));
	TEST_ASSERT_EQUAL_HEX16(0xFBBB,P2DIR);

	P3DIR = 0xFFFF;
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsInput(3,3));
	TEST_ASSERT_EQUAL_HEX16(0xFFF7,P3DIR);
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsInput(3,7));
	TEST_ASSERT_EQUAL_HEX16(0xFF77,P3DIR);
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsInput(3,11));
	TEST_ASSERT_EQUAL_HEX16(0xF777,P3DIR);

	P4DIR = 0xFFFF;
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsInput(4,1));
	TEST_ASSERT_EQUAL_HEX16(0xFFFD,P4DIR);
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsInput(4,5));
	TEST_ASSERT_EQUAL_HEX16(0xFFDD,P4DIR);
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsInput(4,9));
	TEST_ASSERT_EQUAL_HEX16(0xFDDD,P4DIR);
	TEST_ASSERT_EQUAL(0,GPIO_SetPinAsInput(4,13));
	TEST_ASSERT_EQUAL_HEX16(0xDDDD,P4DIR);
}


void test_GPIO_SetPinAsInput_should_RejectInvalidValues(void)
{
	TEST_ASSERT_EQUAL(1,GPIO_SetPinAsInput(0,0));
	TEST_ASSERT_EQUAL(1,GPIO_SetPinAsInput(5,1));
}

void test_GPIO_ReadInputPin_should_RejectInvalidValues(void)
{
	TEST_ASSERT_EQUAL_INT(-1,GPIO_ReadInputPin(0,0));
}

void test_GPIO_ReadInputPin_should_AcceptValidValues(void)
{

	P1IN = 0x0001;
	TEST_ASSERT_EQUAL_INT(1,GPIO_ReadInputPin(1,0));
	P2IN = 0x0010;
	TEST_ASSERT_EQUAL_INT(1,GPIO_ReadInputPin(2,4));
	P3IN = 0x1400;
	TEST_ASSERT_EQUAL_INT(1,GPIO_ReadInputPin(3,12));
	P4IN = 0x0000;
	TEST_ASSERT_EQUAL_INT(0,GPIO_ReadInputPin(4,15));
}

void test_GPIO_ReadInputPort_should_RejectInvalidPorts(void)
{
	TEST_ASSERT_EQUAL(-1,GPIO_ReadInputPort(0));
	TEST_ASSERT_EQUAL(-1,GPIO_ReadInputPort(5));
	TEST_ASSERT_EQUAL(-1,GPIO_ReadInputPort(255));
}

void test_GPIO_ReadInputPort_should_AcceptValidPorts(void)
{

	P1IN = 0x0010;
	TEST_ASSERT_EQUAL(16,GPIO_ReadInputPort(1));
	P2IN = 0x00F0;
	TEST_ASSERT_EQUAL(240,GPIO_ReadInputPort(2));
	P3IN = 0xFFFF;
	TEST_ASSERT_EQUAL(65535,GPIO_ReadInputPort(3));
	TEST_ASSERT_EQUAL(0,GPIO_ReadInputPort(4));
}


void test_GPIO_TogglePin_should_AcceptValidValues(void)
{
	P1DIR = 0xFFFF;
	P1OUT = 0x0050;
	TEST_ASSERT_EQUAL(0,GPIO_TogglePin(1,6));
	TEST_ASSERT_EQUAL_HEX16(0x10,P1OUT);

	P2DIR = 0xFFFF;
	P2OUT = 0xC006;
	TEST_ASSERT_EQUAL(0,GPIO_TogglePin(2,0));
	TEST_ASSERT_EQUAL_HEX16(0xC007,P2OUT);
	TEST_ASSERT_EQUAL(0,GPIO_TogglePin(2,0));
	TEST_ASSERT_EQUAL_HEX16(0xC006,P2OUT);

	P3DIR = 0x0100;
	P3OUT = 0x0100;
	TEST_ASSERT_EQUAL(0,GPIO_TogglePin(3,8));
	TEST_ASSERT_EQUAL_HEX16(0x0000,P3OUT);
	TEST_ASSERT_EQUAL(0,GPIO_TogglePin(3,7));
	TEST_ASSERT_EQUAL_HEX16(0x0000,P3OUT);

	P4DIR = 0xF000;
	P4OUT = 0xF000;
	TEST_ASSERT_EQUAL(0,GPIO_TogglePin(4,12));
	TEST_ASSERT_EQUAL_HEX16(0xE000,P4OUT);
	TEST_ASSERT_EQUAL(0,GPIO_TogglePin(4,13));
	TEST_ASSERT_EQUAL_HEX16(0xC000,P4OUT);
	TEST_ASSERT_EQUAL(0,GPIO_TogglePin(4,14));
	TEST_ASSERT_EQUAL_HEX16(0x8000,P4OUT);
	TEST_ASSERT_EQUAL(0,GPIO_TogglePin(4,15));
	TEST_ASSERT_EQUAL_HEX16(0x0000,P4OUT);

	TEST_ASSERT_EQUAL(0,GPIO_TogglePin(3,10));
	TEST_ASSERT_EQUAL_HEX16(0x0000,P4OUT);
}

void test_GPIO_TogglePin_should_RejectInvalidValues(void)
{
	TEST_ASSERT_EQUAL(1,GPIO_SetPinAsOutput(1,33));
	TEST_ASSERT_EQUAL(1,GPIO_SetPinAsOutput(2,64));
	TEST_ASSERT_EQUAL(1,GPIO_SetPinAsOutput(3,24));
	TEST_ASSERT_EQUAL(1,GPIO_SetPinAsOutput(4,17));
}


/********************** GPIO Attach Interrupt **********************/
void test_GPIO_AttachInputInterrupt_should_AcceptValidPorts_and_Pins(void)
{

	TEST_ASSERT_EQUAL(GPIO_OK,GPIO_AttachInputInterrupt(1,5,1));
	TEST_ASSERT_EQUAL_HEX16(BIT_TO_MASK(5), P1IES);
	TEST_ASSERT_EQUAL_HEX16(BIT_TO_MASK(5), P1IE);

	TEST_ASSERT_EQUAL(GPIO_OK,GPIO_AttachInputInterrupt(2,10,1));
	TEST_ASSERT_EQUAL_HEX16(BIT_TO_MASK(10), P2IES);
	TEST_ASSERT_EQUAL_HEX16(BIT_TO_MASK(10), P2IE);

	TEST_ASSERT_EQUAL(GPIO_OK,GPIO_AttachInputInterrupt(3,0,1));
	TEST_ASSERT_EQUAL_HEX16(BIT_TO_MASK(0), P3IES);
	TEST_ASSERT_EQUAL_HEX16(BIT_TO_MASK(0), P3IE);

	TEST_ASSERT_EQUAL(GPIO_OK,GPIO_AttachInputInterrupt(4,15,1));
	TEST_ASSERT_EQUAL_HEX16(BIT_TO_MASK(15), P4IES);
	TEST_ASSERT_EQUAL_HEX16(BIT_TO_MASK(15), P4IE);

}

void test_GPIO_AttachInputInterrupt_should_OnlyAttachInputPins(void)
{
	P1DIR = 0xFFFF;
	P1IE = 0x0000;
	TEST_ASSERT_EQUAL(GPIO_OK,GPIO_AttachInputInterrupt(1,5,1));
	TEST_ASSERT_EQUAL_HEX16(0x0000,P1IE);

	P2DIR = 0xFF00;
	P2IE = 0x0000;
	TEST_ASSERT_EQUAL(GPIO_OK,GPIO_AttachInputInterrupt(2,5,1));
	TEST_ASSERT_EQUAL_HEX16(BIT_TO_MASK(5),P2IE);

}


void test_GPIO_AttachInputInterrupt_should_RejectInvalidPorts_and_Pins(void)
{
	TEST_ASSERT_EQUAL(GPIO_FAIL, GPIO_AttachInputInterrupt(0,5,1));
	TEST_ASSERT_EQUAL(GPIO_FAIL, GPIO_AttachInputInterrupt(5,5,1));
	TEST_ASSERT_EQUAL(GPIO_FAIL, GPIO_AttachInputInterrupt(0XFF,5,1));
	TEST_ASSERT_EQUAL(GPIO_FAIL, GPIO_AttachInputInterrupt(1,16,1));
	TEST_ASSERT_EQUAL(GPIO_FAIL, GPIO_AttachInputInterrupt(1,0xFFFF,1));
}


/********************** GPIO Detach Interrupt **********************/
void test_GPIO_DetachInputInterrupt_should_RejectInvalidPorts_and_Pins(void)
{
	TEST_ASSERT_EQUAL(GPIO_FAIL, GPIO_DetachInputInterrupt(0,5));
	TEST_ASSERT_EQUAL(GPIO_FAIL, GPIO_DetachInputInterrupt(5,5));
	TEST_ASSERT_EQUAL(GPIO_FAIL, GPIO_DetachInputInterrupt(0XFF,5));
	TEST_ASSERT_EQUAL(GPIO_FAIL, GPIO_DetachInputInterrupt(1,16));
	TEST_ASSERT_EQUAL(GPIO_FAIL, GPIO_DetachInputInterrupt(1,0xFFFF));
}


void test_GPIO_DetachInputInterrupt_should_AcceptValidPorts_and_Pins(void)
{
	P1IE = BIT_TO_MASK(5);
	TEST_ASSERT_EQUAL(GPIO_OK,GPIO_DetachInputInterrupt(1,5));
	TEST_ASSERT_EQUAL_HEX16(0 , (P1IE & BIT_TO_MASK(5)));

	TEST_ASSERT_EQUAL(GPIO_OK,GPIO_DetachInputInterrupt(2,10));
	TEST_ASSERT_EQUAL_HEX16(0, (P2IE & BIT_TO_MASK(10)));

	TEST_ASSERT_EQUAL(GPIO_OK,GPIO_DetachInputInterrupt(3,0));
	TEST_ASSERT_EQUAL_HEX16(0, (P3IE & BIT_TO_MASK(0)));

	TEST_ASSERT_EQUAL(GPIO_OK,GPIO_DetachInputInterrupt(4,15));
	TEST_ASSERT_EQUAL_HEX16(0, (P4IE & BIT_TO_MASK(15)));

}
