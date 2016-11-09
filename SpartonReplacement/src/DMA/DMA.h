/** @file DMA.h
 *  @brief Direct Memory Access (DMA) Functions
 *
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date October 11, 2016
 *  @version 0.0.1
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab
 *  @copyright Environmental Development Division
 *
 *
 *  @bug  No known bugs
 */

#ifndef DMA_H
#define DMA_H


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "main.h"


#ifdef PMEL
#include <msp430fr5969.h>
#endif

#ifndef true
#define true    (1)
#endif

#ifndef false
#define false   (0)
#endif
#define DMA_BUF_LENGTH      (128)//(2048)
#define DMA_ESC_COUNT       (2)
#define DMA_TIMEOUT         (10)







void DMA_Init(void);
void DMA_Start(void);
void DMA_Stop(void);
uint16_t DMA_WriteLength(void);


#endif //DMA_H