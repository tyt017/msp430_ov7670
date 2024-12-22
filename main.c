#include <stdio.h>
#include <msp430.h>
#include <stdint.h>
#include "pins.h"
#include "ov7670.h"

#define IMAGE_SIZE (320 * 240)

unsigned int ov_sta = 0;

#pragma PERSISTENT(data_fifo)
unsigned char data_fifo[IMAGE_SIZE] = {0};

volatile uint8_t FIFO_1 = 0;
volatile uint8_t FIFO_2 = 0;
volatile uint8_t FIFO_data = 0;

// FIFO module to save camera data
#pragma vector = PORT4_VECTOR // external interrupt to catch frame
__interrupt void PORT4_B1_ISR(void)
{
    if(P4IV == 0x04)
    {

        FIFO_WRST_L;
        delay_us(10);
        FIFO_WRST_H;
        delay_us(10);

        if(ov_sta == 0)
        {
//            FIFO_WR_H;
            ov_sta = 1; // start capture the frame
        }
        else if(ov_sta == 1)
        {
//            FIFO_WR_L;
            ov_sta = 2; // finishing
            P4IE &= ~BIT1; // 禁用 P4.1 中斷
            P4IFG &= ~BIT1;
        }
    }
    P4IFG = 0;
}

void vsync_interrupt_init(void)
{
    P4DIR &= ~BIT1; // P4.1 設為輸入
    P4REN |= BIT1;  // 啟用上拉電阻
    P4OUT |= BIT1;  // 上拉

    P4IES |= BIT1;  // 偵測下降沿
    P4IE  |= BIT1;  // 啟用 P4.1 中斷
    P4IFG &= ~BIT1; // 清除中斷標誌
}

void fifo_init(void)
{
    FIFO_WRST_L;
    FIFO_RRST_L;
    delay_us(10);

    FIFO_WRST_H;
    FIFO_RRST_H;
    delay_us(10);

    FIFO_WR_L;
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5; // unlock GPIO

	unsigned int p = 0, j = 0;

	fifo_init();
	vsync_interrupt_init();

	FIFO_OE_L;

	__enable_interrupt();

	while(1)
	{
	    unsigned int index = 0;
	    if(ov_sta == 2)
        {
            P4IE &= ~BIT1;
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
//                    FIFO_1 = P3IN & 0x0f;
//                    FIFO_2 = P3IN & 0xf0;
//                    FIFO_data = FIFO_1 | FIFO_2;
                    FIFO_data = P3IN;
                    data_fifo[index++] = FIFO_data;
                    FIFO_RCK_H;

                    FIFO_RCK_L;
//                    FIFO_1 = P3IN & 0x0f;
//                    FIFO_2 = P3IN & 0xf0;
//                    FIFO_data = FIFO_1 | FIFO_2;
                    FIFO_data = P3IN;
                    data_fifo[index++] = FIFO_data;
                    FIFO_RCK_H;
                }
            }
            ov_sta = 0;
            P4IE |= BIT1;
        }
	}


	return 0;
}
