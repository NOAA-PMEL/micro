#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "../../inc/includes.h"

#ifdef PMEL
#include <msp430fr5969.h>
#else
//#include "../../test/msp430fr5969.h"
#endif



#define SPI_A0      (0)
#define SPI_A1      (1)
#define SPI_B0      (2)

#define SPI_OK   (0)
#define SPI_FAIL (1)


uint8_t SPI_Master_3W_Init(uint8_t Port, uint32_t Baud,uint32_t ClkFreq, uint16_t Clk);

uint8_t SPI_Master_4W_Init(uint8_t Port, uint32_t Baud, uint32_t ClkFreq, uint16_t Clk);

uint8_t SPI_Slave_3W_Init(uint8_t Port, uint32_t Baud, uint32_t ClkFreq, uint16_t Clk);

uint8_t SPI_Slave_4W_Init(uint8_t Port, uint32_t Baud, uint32_t ClkFreq, uint16_t Clk);

uint8_t SPI_Master_EnableInterrupts(uint8_t Port,uint8_t TxInt, uint8_t RxInt);

uint8_t SPI_putc(char value, uint8_t Port);

uint8_t SPI_puts(uint8_t *value,uint16_t length, uint8_t Port);

#endif // SPI_H
