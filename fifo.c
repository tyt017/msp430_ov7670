///*
// * fifo.c
// *
// *  Created on: 2024/12/20
// *      Author: USER
// */
//
//#include <msp430.h>
//#include "pins.h"
//
//unsigned int ov_sta = 0;
//
//// FIFO module to save camera data
//#pragma vector = PORT1_VECTOR // external interrupt to catch frame
//__interrupt void PORT1_B0_ISR(void)
//{
//    if(P1IV == 2)
//    {
//        FIFO_WRST_L;
//        FIFO_WRST_H;
//        if(ov_sta == 0)
//        {
//            FIFO_WR_H;
//            ov_sta = 1;
//        }
//        else if(ov_sta == 1)
//        {
//            FIFO_WR_L;
//            ov_sta = 2;
//        }
//    }
//    P1IFG = 0;
//}
