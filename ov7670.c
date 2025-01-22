/*
 * ov7670.c
 *
 *  Created on: 2024/12/16
 *      Author: USER
 */

#include "ov7670.h"

const unsigned char ov7670_reg[][2] = {
    {0x12, 0x80},  // RESET
    {0x11, 0x01},  // CLKRC: Prescaler
    {0x12, 0x14},  // COM7: QVGA 模式
    {0x73, 0xF0},  // DSP_CTRL3: 像素時鐘分頻器（QQVGA）
    {0x70, 0x3A},  // SCALING_XSC: X 軸縮放控制
    {0x71, 0x35},  // SCALING_YSC: Y 軸縮放控制
    {0x72, 0x11},  // SCALING_DCWCTR: 縮放與裁剪控制
    {0x17, 0x11},  // HSTART: 水平起始位置
    {0x18, 0x75},  // HSTOP: 水平結束位置
    {0x19, 0x01},  // VSTART: 垂直起始位置
    {0x1A, 0x97},  // VSTOP: 垂直結束位置
    {0x32, 0x36},  // HREF: 水平邊界設置
    {0x40, 0xD0},  // COM15: RGB565 輸出格式
    {0x3A, 0x04},  // TSLB: 正常輸出順序
    {0x8C, 0x00},  // RGB444: 關閉 RGB444
    {0x00, 0x00}  // END
};

void delay_ms(unsigned int ms)
{
    while(ms--)
    {
        __delay_cycles(1000);
    }
}

unsigned char ov7670_init(void)
{
    unsigned int i = 0;
    unsigned char temp;

    // VSYNC = P4.1
    P4DIR &= ~BIT1;
    P4REN |= BIT1;
    P4OUT |= BIT1; // pullup resistor input

    //FIFO data input D0-D7 = P3.0-P3.7
    P3DIR &= 0x00;
    P3REN |= 0xff;
    P3OUT |= 0xff;

    //output
    //FIFO_RCK
    P4DIR |= BIT4;
    P4OUT |= BIT4;
    //FIFO_OE
    P5DIR |= BIT3;
    P5OUT |= BIT3;
    //FIFO_WR, FIFO_WRST, FIFO_RRST
    P8DIR |= 0x0e;
    P8OUT |= 0x0e;

    SCCB_init();

    if(wr_sensor_reg(0x12, 0x80) != 0) // reset SCCB
        return 1;

    delay_ms(50);

    if(rd_sensor_reg(0x0b, &temp) != 0)
        return 2;

    if(temp == 0x73) //ov7670
    {
        for(i=0; ov7670_reg[i][0] != 0x00; i++) // CHECK THE CODE
        {
            if(wr_sensor_reg(ov7670_reg[i][0], ov7670_reg[i][1]) != 0)
            {
                return 3;
            }
        }
    }
    return 0;
}

unsigned char wr_sensor_reg(unsigned char regID, unsigned char *regDat)
{
    startSCCB();
    if(SCCBwriteByte(0x42) == 0) // write address
    {
        return 1;
    }
    if(SCCBwriteByte(regID) == 0)
    {
        return 2;
    }
    *regDat = SCCBreadByte();
    noAck();
    stopSCCB(); // stop transfer
    return 0;
}

unsigned char rd_sensor_reg(unsigned char regID, unsigned char *regDat)
{
    startSCCB();
    if(SCCBwriteByte(0x42) == 0) // write address
    {
        return 1; // error return
    }
    delay_us(100);
    if(SCCBwriteByte(regID) == 0) // register ID
    {
        return 2; // error return
    }
    delay_us(100);

    stopSCCB(); // stop sending order

    delay_us(100);

    startSCCB();
    if(SCCBwriteByte(0x43) == 0) // read address
    {
        return 3; // error return
    }
    delay_us(100);
    *regDat = SCCBreadByte();
    noAck();
    stopSCCB();
    return 0;
}

void ov7670_windowSet(unsigned int sx, unsigned int sy, unsigned int width, unsigned int height)
{
    unsigned int endx;
    unsigned int endy;
    unsigned char temp;
    endx = (sx + width*2) % 784; // sx: HSTART, endx: HSTOP
    endy = sy + height*2; // sy: VSTART, endy: VSTOP

    rd_sensor_reg(0x32, &temp);
    temp &= 0xc0;
    temp |= ((endx & 0x07) << 3) | (sx & 0x07);
    wr_sensor_reg(0x032, temp);
    wr_sensor_reg(0x17, sx >> 3);
    wr_sensor_reg(0x18, endx >> 3);

    rd_sensor_reg(0x03, &temp);
    temp &= 0xf0;
    temp |= ((endy & 0x03) << 2) | (sy & 0x03);
    wr_sensor_reg(0x03, temp);
    wr_sensor_reg(0x19, sy >> 2);
    wr_sensor_reg(0x1a, endy >> 2);
}
