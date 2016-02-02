/** @file msp430fr5969.h
 *  @brief 
 *  
 *  @author Matt Casari, matthew.casari@noaa.org
 *  @date Dec 7, 2015
 *  @version 
 *
 *  @copyright National Oceanic and Atmospheric Administration
 *  @copyright Pacific Marine Environmental Lab 
 *  @copyright Environmental Development Division
 *  
 *	@note 
 *
 *  @bug  No known bugs
 */



#ifndef MSP430FR5969_H_
#define MSP430FR5969_H_

#define DIMENSION_OF(a)	(sizeof(a)/sizeof(a[0]))
#define REGISTER volatile uint16_t
#define BIT_TO_MASK(a) (1u << (a))

#define BIT0	BIT_TO_MASK(0)
#define BIT1	BIT_TO_MASK(1)
#define BIT2	BIT_TO_MASK(2)
#define BIT3	BIT_TO_MASK(3)
#define BIT4	BIT_TO_MASK(4)
#define BIT5	BIT_TO_MASK(5)
#define BIT6	BIT_TO_MASK(6)
#define BIT7	BIT_TO_MASK(7)
#define BIT8	BIT_TO_MASK(8)
#define BIT9	BIT_TO_MASK(9)
#define BIT10	BIT_TO_MASK(10)
#define BIT11	BIT_TO_MASK(11)
#define BIT12	BIT_TO_MASK(12)
#define BIT13	BIT_TO_MASK(13)
#define BIT14	BIT_TO_MASK(14)
#define BIT15	BIT_TO_MASK(15)

#define __delay_cycles(a)

#ifdef TEST
#define INSTANCE(name, address, template) PORT_##address; template PORT_##address;
#else
#define INSTANCE(name, address, template) const template* name = (*(template*)(address))
#endif

/*************GPIO************/
REGISTER P1OUT;
REGISTER P2OUT;
REGISTER P3OUT;
REGISTER P4OUT;
REGISTER P5OUT;

REGISTER P1DIR;
REGISTER P2DIR;
REGISTER P3DIR;
REGISTER P4DIR;

REGISTER P1SEL0;
REGISTER P2SEL0;
REGISTER P3SEL0;
REGISTER P4SEL0;

REGISTER P1SEL1;
REGISTER P2SEL1;
REGISTER P3SEL1;
REGISTER P4SEL1;

REGISTER P1REN;
REGISTER P2REN;
REGISTER P3REN;
REGISTER P4REN;

REGISTER P1IN;
REGISTER P2IN;
REGISTER P3IN;
REGISTER P4IN;

/*************UART************/
REGISTER UCA0CTLW0;
REGISTER UCA0CTLW1;
REGISTER UCA0BRW;
REGISTER UCA0MCTLW;
REGISTER UCA0STATW;
REGISTER UCA0RXBUF;
REGISTER UCA0TXBUF;
REGISTER UCA0ABCTL;
REGISTER UCA0IRCTL;
REGISTER UCA0IE;
REGISTER UCA0IFG;
REGISTER UCA0IV;

REGISTER UCA1CTLW0;
REGISTER UCA1CTLW1;
REGISTER UCA1BRW;
REGISTER UCA1MCTLW;
REGISTER UCA1STATW;
REGISTER UCA1RXBUF;
REGISTER UCA1TXBUF;
REGISTER UCA1ABCTL;
REGISTER UCA1IRCTL;
REGISTER UCA1IE;
REGISTER UCA1IFG;
REGISTER UCA1IV;



/*************MPU************/
REGISTER MPUCTL0;
REGISTER MPUCTL1;
REGISTER MPUSEGB1;
REGISTER MPUSEGB2;
REGISTER MPUSAM;
REGISTER MPUIPC0;
REGISTER MPUIPSEGB1;
REGISTER MPUIPSEGB2;

/* MPUCTL0 Control Bits */
#define MPUENA               (0x0001u)  /* MPU Enable */
#define MPULOCK              (0x0002u)  /* MPU Lock */
#define MPUSEGIE             (0x0010u)  /* MPU Enable NMI on Segment violation */

/* MPUCTL0 Control Bits */
#define MPUENA_L            (0x0001u)  /* MPU Enable */
#define MPULOCK_L           (0x0002u)  /* MPU Lock */
#define MPUSEGIE_L          (0x0010u)  /* MPU Enable NMI on Segment violation */

#define MPUPW                (0xA500u)  /* MPU Access Password */
#define MPUPW_H              (0xA5)    /* MPU Access Password */

/* MPUCTL1 Control Bits */
#define MPUSEG1IFG           (0x0001u)  /* MPU Main Memory Segment 1 violation interupt flag */
#define MPUSEG2IFG           (0x0002u)  /* MPU Main Memory Segment 2 violation interupt flag */
#define MPUSEG3IFG           (0x0004u)  /* MPU Main Memory Segment 3 violation interupt flag */
#define MPUSEGIIFG           (0x0008u)  /* MPU Info Memory Segment violation interupt flag */
#define MPUSEGIPIFG          (0x0010u)  /* MPU IP Memory Segment violation interupt flag */

/* MPUCTL1 Control Bits */
#define MPUSEG1IFG_L        (0x0001u)  /* MPU Main Memory Segment 1 violation interupt flag */
#define MPUSEG2IFG_L        (0x0002u)  /* MPU Main Memory Segment 2 violation interupt flag */
#define MPUSEG3IFG_L        (0x0004u)  /* MPU Main Memory Segment 3 violation interupt flag */
#define MPUSEGIIFG_L        (0x0008u)  /* MPU Info Memory Segment violation interupt flag */
#define MPUSEGIPIFG_L       (0x0010u)  /* MPU IP Memory Segment violation interupt flag */


// MPUSEGB2 Control Bits

// MPUSEGB1 Control Bits

// MPUSAM Control Bits
#define MPUSEG1RE            (0x0001u)  /* MPU Main memory Segment 1 Read enable */
#define MPUSEG1WE            (0x0002u)  /* MPU Main memory Segment 1 Write enable */
#define MPUSEG1XE            (0x0004u)  /* MPU Main memory Segment 1 Execute enable */
#define MPUSEG1VS            (0x0008u)  /* MPU Main memory Segment 1 Violation select */
#define MPUSEG2RE            (0x0010u)  /* MPU Main memory Segment 2 Read enable */
#define MPUSEG2WE            (0x0020u)  /* MPU Main memory Segment 2 Write enable */
#define MPUSEG2XE            (0x0040u)  /* MPU Main memory Segment 2 Execute enable */
#define MPUSEG2VS            (0x0080u)  /* MPU Main memory Segment 2 Violation select */
#define MPUSEG3RE            (0x0100u)  /* MPU Main memory Segment 3 Read enable */
#define MPUSEG3WE            (0x0200u)  /* MPU Main memory Segment 3 Write enable */
#define MPUSEG3XE            (0x0400u)  /* MPU Main memory Segment 3 Execute enable */
#define MPUSEG3VS            (0x0800u)  /* MPU Main memory Segment 3 Violation select */
#define MPUSEGIRE            (0x1000u)  /* MPU Info memory Segment Read enable */
#define MPUSEGIWE            (0x2000u)  /* MPU Info memory Segment Write enable */
#define MPUSEGIXE            (0x4000u)  /* MPU Info memory Segment Execute enable */
#define MPUSEGIVS            (0x8000u)  /* MPU Info memory Segment Violation select */

/* MPUSAM Control Bits */
#define MPUSEG1RE_L         (0x0001u)  /* MPU Main memory Segment 1 Read enable */
#define MPUSEG1WE_L         (0x0002u)  /* MPU Main memory Segment 1 Write enable */
#define MPUSEG1XE_L         (0x0004u)  /* MPU Main memory Segment 1 Execute enable */
#define MPUSEG1VS_L         (0x0008u)  /* MPU Main memory Segment 1 Violation select */
#define MPUSEG2RE_L         (0x0010u)  /* MPU Main memory Segment 2 Read enable */
#define MPUSEG2WE_L         (0x0020u)  /* MPU Main memory Segment 2 Write enable */
#define MPUSEG2XE_L         (0x0040u)  /* MPU Main memory Segment 2 Execute enable */
#define MPUSEG2VS_L         (0x0080u)  /* MPU Main memory Segment 2 Violation select */

/* MPUSAM Control Bits */
#define MPUSEG3RE_H         (0x0001u)  /* MPU Main memory Segment 3 Read enable */
#define MPUSEG3WE_H         (0x0002u)  /* MPU Main memory Segment 3 Write enable */
#define MPUSEG3XE_H         (0x0004u)  /* MPU Main memory Segment 3 Execute enable */
#define MPUSEG3VS_H         (0x0008u)  /* MPU Main memory Segment 3 Violation select */
#define MPUSEGIRE_H         (0x0010u)  /* MPU Info memory Segment Read enable */
#define MPUSEGIWE_H         (0x0020u)  /* MPU Info memory Segment Write enable */
#define MPUSEGIXE_H         (0x0040u)  /* MPU Info memory Segment Execute enable */
#define MPUSEGIVS_H         (0x0080u)  /* MPU Info memory Segment Violation select */

/* MPUIPC0 Control Bits */
#define MPUIPVS              (0x0020u)  /* MPU MPU IP protection segment Violation Select */
#define MPUIPENA             (0x0040u)  /* MPU MPU IP Protection Enable */
#define MPUIPLOCK            (0x0080u)  /* MPU IP Protection Lock */

/* MPUIPC0 Control Bits */
#define MPUIPVS_L           (0x0020u)  /* MPU MPU IP protection segment Violation Select */
#define MPUIPENA_L          (0x0040u)  /* MPU MPU IP Protection Enable */
#define MPUIPLOCK_L         (0x0080u)  /* MPU IP Protection Lock */




/**********Watchdog*********/
REGISTER WDTCTL;

/* WDTCTL Control Bits */
#define WDTIS0              (0x0001u)  /* WDT - Timer Interval Select 0 */
#define WDTIS1              (0x0002u)  /* WDT - Timer Interval Select 1 */
#define WDTIS2              (0x0004u)  /* WDT - Timer Interval Select 2 */
#define WDTCNTCL            (0x0008u)  /* WDT - Timer Clear */
#define WDTTMSEL            (0x0010u)  /* WDT - Timer Mode Select */
#define WDTSSEL0            (0x0020u)  /* WDT - Timer Clock Source Select 0 */
#define WDTSSEL1            (0x0040u)  /* WDT - Timer Clock Source Select 1 */
#define WDTHOLD             (0x0080u)  /* WDT - Timer hold */

/* WDTCTL Control Bits */
#define WDTIS0_L            (0x0001u)  /* WDT - Timer Interval Select 0 */
#define WDTIS1_L            (0x0002u)  /* WDT - Timer Interval Select 1 */
#define WDTIS2_L            (0x0004u)  /* WDT - Timer Interval Select 2 */
#define WDTCNTCL_L          (0x0008u)  /* WDT - Timer Clear */
#define WDTTMSEL_L          (0x0010u)  /* WDT - Timer Mode Select */
#define WDTSSEL0_L          (0x0020u)  /* WDT - Timer Clock Source Select 0 */
#define WDTSSEL1_L          (0x0040u)  /* WDT - Timer Clock Source Select 1 */
#define WDTHOLD_L           (0x0080u)  /* WDT - Timer hold */

#define WDTPW               (0x5A00u)

#define WDTIS_0           (0*0x0001u)  /* WDT - Timer Interval Select: /2G */
#define WDTIS_1           (1*0x0001u)  /* WDT - Timer Interval Select: /128M */
#define WDTIS_2           (2*0x0001u)  /* WDT - Timer Interval Select: /8192k */
#define WDTIS_3           (3*0x0001u)  /* WDT - Timer Interval Select: /512k */
#define WDTIS_4           (4*0x0001u)  /* WDT - Timer Interval Select: /32k */
#define WDTIS_5           (5*0x0001u)  /* WDT - Timer Interval Select: /8192 */
#define WDTIS_6           (6*0x0001u)  /* WDT - Timer Interval Select: /512 */
#define WDTIS_7           (7*0x0001u)  /* WDT - Timer Interval Select: /64 */
#define WDTIS__2G         (0*0x0001u)  /* WDT - Timer Interval Select: /2G */
#define WDTIS__128M       (1*0x0001u)  /* WDT - Timer Interval Select: /128M */
#define WDTIS__8192K      (2*0x0001u)  /* WDT - Timer Interval Select: /8192k */
#define WDTIS__512K       (3*0x0001u)  /* WDT - Timer Interval Select: /512k */
#define WDTIS__32K        (4*0x0001u)  /* WDT - Timer Interval Select: /32k */
#define WDTIS__8192       (5*0x0001u)  /* WDT - Timer Interval Select: /8192 */
#define WDTIS__512        (6*0x0001u)  /* WDT - Timer Interval Select: /512 */
#define WDTIS__64         (7*0x0001u)  /* WDT - Timer Interval Select: /64 */

#define WDTSSEL_0         (0*0x0020u)  /* WDT - Timer Clock Source Select: SMCLK */
#define WDTSSEL_1         (1*0x0020u)  /* WDT - Timer Clock Source Select: ACLK */
#define WDTSSEL_2         (2*0x0020u)  /* WDT - Timer Clock Source Select: VLO_CLK */
#define WDTSSEL_3         (3*0x0020u)  /* WDT - Timer Clock Source Select: reserved */
#define WDTSSEL__SMCLK    (0*0x0020u)  /* WDT - Timer Clock Source Select: SMCLK */
#define WDTSSEL__ACLK     (1*0x0020u)  /* WDT - Timer Clock Source Select: ACLK */
#define WDTSSEL__VLO      (2*0x0020u)  /* WDT - Timer Clock Source Select: VLO_CLK */

/**********I2C*********/
REGISTER UCB0CTLW0;
REGISTER UCB0CTLW1;
REGISTER UCB0BRW;
REGISTER UCB0STATW;
REGISTER UCB0TBCNT;
REGISTER UCB0RXBUF;
REGISTER UCB0TXBUF;
REGISTER UCB0I2COA0;
REGISTER UCB0I2COA1;
REGISTER UCB0I2COA2;
REGISTER UCB0I2COA3;
REGISTER UCB0ADDRX;
REGISTER UCB0ADDMASK;
REGISTER UCB0I2CSA;
REGISTER UCB0IE;
REGISTER UCB0IFG;
REGISTER UCB0IV;


// I2C Register Byte Mapping

// UCAxCTLW0 UART-Mode Control Bits
#define UCPEN               (0x8000u)    /* Async. Mode: Parity enable */
#define UCPAR               (0x4000u)    /* Async. Mode: Parity     0:odd / 1:even */
#define UCMSB               (0x2000u)    /* Async. Mode: MSB first  0:LSB / 1:MSB */
#define UC7BIT              (0x1000u)    /* Async. Mode: Data Bits  0:8-bits / 1:7-bits */
#define UCSPB               (0x0800u)    /* Async. Mode: Stop Bits  0:one / 1: two */
#define UCMODE1             (0x0400u)    /* Async. Mode: USCI Mode 1 */
#define UCMODE0             (0x0200u)    /* Async. Mode: USCI Mode 0 */
#define UCSYNC              (0x0100u)    /* Sync-Mode  0:UART-Mode / 1:SPI-Mode */
#define UCSSEL1             (0x0080u)    /* USCI 0 Clock Source Select 1 */
#define UCSSEL0             (0x0040u)    /* USCI 0 Clock Source Select 0 */
#define UCRXEIE             (0x0020u)    /* RX Error interrupt enable */
#define UCBRKIE             (0x0010u)    /* Break interrupt enable */
#define UCDORM              (0x0008u)    /* Dormant (Sleep) Mode */
#define UCTXADDR            (0x0004u)    /* Send next Data as Address */
#define UCTXBRK             (0x0002u)    /* Send next Data as Break */
#define UCSWRST             (0x0001u)    /* USCI Software Reset */

// UCAxCTLW0 UART-Mode Control Bits
#define UCSSEL1_L           (0x0080u)    /* USCI 0 Clock Source Select 1 */
#define UCSSEL0_L           (0x0040u)    /* USCI 0 Clock Source Select 0 */
#define UCRXEIE_L           (0x0020u)    /* RX Error interrupt enable */
#define UCBRKIE_L           (0x0010u)    /* Break interrupt enable */
#define UCDORM_L            (0x0008u)    /* Dormant (Sleep) Mode */
#define UCTXADDR_L          (0x0004u)    /* Send next Data as Address */
#define UCTXBRK_L           (0x0002u)    /* Send next Data as Break */
#define UCSWRST_L           (0x0001u)    /* USCI Software Reset */

// UCAxCTLW0 UART-Mode Control Bits
#define UCPEN_H             (0x0080u)    /* Async. Mode: Parity enable */
#define UCPAR_H             (0x0040u)    /* Async. Mode: Parity     0:odd / 1:even */
#define UCMSB_H             (0x0020u)    /* Async. Mode: MSB first  0:LSB / 1:MSB */
#define UC7BIT_H            (0x0010u)    /* Async. Mode: Data Bits  0:8-bits / 1:7-bits */
#define UCSPB_H             (0x0008u)    /* Async. Mode: Stop Bits  0:one / 1: two */
#define UCMODE1_H           (0x0004u)    /* Async. Mode: USCI Mode 1 */
#define UCMODE0_H           (0x0002u)    /* Async. Mode: USCI Mode 0 */
#define UCSYNC_H            (0x0001u)    /* Sync-Mode  0:UART-Mode / 1:SPI-Mode */

// UCxxCTLW0 SPI-Mode Control Bits
#define UCCKPH              (0x8000u)    /* Sync. Mode: Clock Phase */
#define UCCKPL              (0x4000u)    /* Sync. Mode: Clock Polarity */
#define UCMST               (0x0800u)    /* Sync. Mode: Master Select */
//#define res               (0x0020u)    /* reserved */
//#define res               (0x0010u)    /* reserved */
//#define res               (0x0008u)    /* reserved */
//#define res               (0x0004u)    /* reserved */
#define UCSTEM             (0x0002u)    /* USCI STE Mode */

// UCBxCTLW0 I2C-Mode Control Bits
#define UCA10               (0x8000u)    /* 10-bit Address Mode */
#define UCSLA10             (0x4000u)    /* 10-bit Slave Address Mode */
#define UCMM                (0x2000u)    /* Multi-Master Environment */
//#define res               (0x1000u)    /* reserved */
//#define res               (0x0100u)    /* reserved */
#define UCTXACK             (0x0020u)    /* Transmit ACK */
#define UCTR                (0x0010u)    /* Transmit/Receive Select/Flag */
#define UCTXNACK            (0x0008u)    /* Transmit NACK */
#define UCTXSTP             (0x0004u)    /* Transmit STOP */
#define UCTXSTT             (0x0002u)    /* Transmit START */

// UCBxCTLW0 I2C-Mode Control Bits
//#define res               (0x1000u)    /* reserved */
//#define res               (0x0100u)    /* reserved */
#define UCTXACK_L           (0x0020u)    /* Transmit ACK */
#define UCTR_L              (0x0010u)    /* Transmit/Receive Select/Flag */
#define UCTXNACK_L          (0x0008u)    /* Transmit NACK */
#define UCTXSTP_L           (0x0004u)    /* Transmit STOP */
#define UCTXSTT_L           (0x0002u)    /* Transmit START */

// UCBxCTLW0 I2C-Mode Control Bits
#define UCA10_H             (0x0080u)    /* 10-bit Address Mode */
#define UCSLA10_H           (0x0040u)    /* 10-bit Slave Address Mode */
#define UCMM_H              (0x0020u)    /* Multi-Master Environment */
//#define res               (0x1000u)    /* reserved */
//#define res               (0x0100u)    /* reserved */

#define UCMODE_0            (0x0000u)    /* Sync. Mode: USCI Mode: 0 */
#define UCMODE_1            (0x0200u)    /* Sync. Mode: USCI Mode: 1 */
#define UCMODE_2            (0x0400u)    /* Sync. Mode: USCI Mode: 2 */
#define UCMODE_3            (0x0600u)    /* Sync. Mode: USCI Mode: 3 */

#define UCSSEL_0            (0x0000u)    /* USCI 0 Clock Source: 0 */
#define UCSSEL_1            (0x0040u)    /* USCI 0 Clock Source: 1 */
#define UCSSEL_2            (0x0080u)    /* USCI 0 Clock Source: 2 */
#define UCSSEL_3            (0x00C0u)    /* USCI 0 Clock Source: 3 */
#define UCSSEL__UCLK        (0x0000u)    /* USCI 0 Clock Source: UCLK */
#define UCSSEL__ACLK        (0x0040u)    /* USCI 0 Clock Source: ACLK */
#define UCSSEL__SMCLK       (0x0080u)    /* USCI 0 Clock Source: SMCLK */

/* UCBxIE I2C Control Bits */
#define UCBIT9IE            (0x4000u)  /* I2C Bit 9 Position Interrupt Enable 3 */
#define UCTXIE3             (0x2000u)  /* I2C Transmit Interrupt Enable 3 */
#define UCRXIE3             (0x1000u)  /* I2C Receive Interrupt Enable 3 */
#define UCTXIE2             (0x0800u)  /* I2C Transmit Interrupt Enable 2 */
#define UCRXIE2             (0x0400u)  /* I2C Receive Interrupt Enable 2 */
#define UCTXIE1             (0x0200u)  /* I2C Transmit Interrupt Enable 1 */
#define UCRXIE1             (0x0100u)  /* I2C Receive Interrupt Enable 1 */
#define UCCLTOIE            (0x0080u)  /* I2C Clock Low Timeout interrupt enable */
#define UCBCNTIE            (0x0040u)  /* I2C Automatic stop assertion interrupt enable */
#define UCNACKIE            (0x0020u)  /* I2C NACK Condition interrupt enable */
#define UCALIE              (0x0010u)  /* I2C Arbitration Lost interrupt enable */
#define UCSTPIE             (0x0008u)  /* I2C STOP Condition interrupt enable */
#define UCSTTIE             (0x0004u)  /* I2C START Condition interrupt enable */
#define UCTXIE0             (0x0002u)  /* I2C Transmit Interrupt Enable 0 */
#define UCRXIE0             (0x0001u)  /* I2C Receive Interrupt Enable 0 */

// UCBxCTLW1 I2C-Mode Control Bits
#define UCETXINT_H          (0x0001u)    /* USCI Early UCTXIFG0 */

#define UCGLIT_0            (0x0000u)    /* USCI Deglitch time: 0 */
#define UCGLIT_1            (0x0001u)    /* USCI Deglitch time: 1 */
#define UCGLIT_2            (0x0002u)    /* USCI Deglitch time: 2 */
#define UCGLIT_3            (0x0003u)    /* USCI Deglitch time: 3 */

#define UCASTP_0            (0x0000u)    /* USCI Automatic Stop condition generation: 0 */
#define UCASTP_1            (0x0004u)    /* USCI Automatic Stop condition generation: 1 */
#define UCASTP_2            (0x0008u)    /* USCI Automatic Stop condition generation: 2 */
#define UCASTP_3            (0x000Cu)    /* USCI Automatic Stop condition generation: 3 */

#define UCCLTO_0            (0x0000u)    /* USCI Clock low timeout: 0 */
#define UCCLTO_1            (0x0040u)    /* USCI Clock low timeout: 1 */
#define UCCLTO_2            (0x0080u)    /* USCI Clock low timeout: 2 */
#define UCCLTO_3            (0x00C0u)    /* USCI Clock low timeout: 3 */




/*********************** GPIO ********************************/
REGISTER P1IFG;
REGISTER P2IFG;
REGISTER P3IFG;
REGISTER P4IFG;

REGISTER P1IES;
REGISTER P2IES;
REGISTER P3IES;
REGISTER P4IES;

REGISTER P1IE;
REGISTER P2IE;
REGISTER P3IE;
REGISTER P4IE;
#endif
