#ifndef __PINS_H
#define __PINS_H

#include <msp430.h>

//#define SIO_C BIT1 // P7.1
//#define SIO_D BIT0 // P7.0
//#define VSYNC BIT1 // P4.1
//#define HREF  BIT7 // P5.7
//#define RST   BIT2 // P4.2
//#define FIFO_RCK BIT4 // P4.4
//#define FIFO_WR BIT1 // P8.1
//#define FIFO_OE BIT3 // P5.3
//#define FIFO_WRST BIT2 // P8.2
//#define FIFO_RRST BIT3 // P8.3

// D0 ~ D7 = P3.0 ~ P3.7

#define SIO_C_L() P7OUT |= BIT1 //SIO_C = P7.1
#define SIO_C_H() P7OUT &= ~BIT1

#define SIO_D_L() P7OUT |= BIT0 //SIO_D = P7.0
#define SIO_D_H() P7OUT &= ~BIT0

#define SIO_D_IN P7DIR &= ~BIT0
#define SIO_D_OUT P7DIR |= BIT0
#define SIO_D_STATE P7IN & BIT0

#define FIFO_RCK_L P4OUT &= ~BIT4 //FIFO_RCK = P4.4
#define FIFO_RCK_H P4OUT |= BIT4
#define FIFO_WR_L P8OUT &= ~BIT1 //FIFO_WR = P8.1
#define FIFO_WR_H P8OUT |= BIT1
#define FIFO_OE_L P5OUT &= ~BIT3 //FIFO_OE = P5.3
#define FIFO_OE_H P5OUT |= BIT3
#define FIFO_WRST_L P8OUT &= ~BIT2 //FIFO_WRST = P8.2
#define FIFO_WRST_H P8OUT |= BIT2
#define FIFO_RRST_L P8OUT &= ~BIT3 //FIFO_RRST = 8.3
#define FIFO_RRST_H P8OUT |= BIT3

#endif /* __PINS_H_ */
