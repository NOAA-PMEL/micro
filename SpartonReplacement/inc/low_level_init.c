#include "includes.h"

int __low_level_init(void) {
  
  
  WDTCTL = WDTPW | WDTHOLD;
  
//  /* Config MPU */
//  MPUCTL0 = MPUPW;
//  MPUSEGB1 = 0x0480;
//  MPUSEGB2 = 0x05D0;
//  
//  MPUSAM = (MPUSEG1WE | MPUSEG1RE | MPUSEG2RE | MPUSEG3RE | MPUSEG3XE);
//  MPUCTL0 = MPUPW | MPUENA | MPUSEGIE;
  
  
  
  
  
  
  
  return 1;
}