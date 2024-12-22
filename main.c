#include <stdio.h>
#include <msp430.h>
#include <stdint.h>
#include "pins.h"
#include "ov7670.h"

#define IMAGE_SIZE (160 * 120)

unsigned int ov_sta = 0;

//#pragma LOCATION(data_fifo, 0x20000)
#pragma PERSISTENT(data_fifo)
unsigned char data_fifo[IMAGE_SIZE] = {0};

volatile uint8_t FIFO_1 = 0;
volatile uint8_t FIFO_2 = 0;
volatile uint8_t FIFO_data = 0;

// FIFO module to save camera data
#pragma vector = PORT1_VECTOR // external interrupt to catch frame
__interrupt void PORT1_B0_ISR(void)
{
    if(P1IV == 2)
    {
        FIFO_WRST_L;
        FIFO_WRST_H;
        if(ov_sta == 0)
        {
            FIFO_WR_H;
            ov_sta = 1; // start capture the frame
        }
        else if(ov_sta == 1)
        {
            FIFO_WR_L;
            ov_sta = 2; // finishing
        }
    }
    P1IFG = 0;
}



int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;

	unsigned int p = 0, j = 0;
	FIFO_OE_L;
	if(ov_sta == 2)
	{
	    P1IE &= ~BIT0;
	    ov7670_windowSet(180, 10, pixel_w, pixel_h);

	    FIFO_RRST_L;
	    FIFO_RCK_L;
	    FIFO_RCK_H;
	    FIFO_RCK_L;
	    FIFO_RRST_H;
	    FIFO_RCK_H;

	    for(p = 0; p < pixel_h; p++)
	    {
	        for(j = 0; j< pixel_w; j++)
	        {
	            FIFO_RCK_L;
	            FIFO_1 = P3IN & 0x0f;
	            FIFO_2 = P3IN & 0xf0;
	            FIFO_data = (FIFO_1 >> 4) | FIFO_2;
	            data_fifo[0] = FIFO_data;
	            FIFO_RCK_H;
	            FIFO_RCK_L;
	            FIFO_1 = P3IN & 0x0f;
                FIFO_2 = P3IN & 0xf0;
                FIFO_data = (FIFO_1 >> 4) | FIFO_2;
                data_fifo[1] = FIFO_data;
                FIFO_RCK_H;
	        }
	    }
	    ov_sta = 0;
	    P1IE |= BIT0;
	}

	return 0;
}
