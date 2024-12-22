/*
 * SCCB.c
 *
 *  Created on: 2024/12/20
 *      Author: USER
 */
#include "SCCB.h"

void delay_us(unsigned int us)
{
    while(us--)
    {
        __delay_cycles(1);
    }
}

void SCCB_init(void)
{
    // SIO_D-P7.0, pullup input
    P7DIR &= ~BIT0;
    P7REN |= BIT0;
    P7OUT |= BIT0;

    // SIO_C-P7.1, output
    P7DIR |= BIT1;
    P7OUT |= BIT1;

    SIO_D_OUT;
}

void startSCCB(void) // SCCB start signal
{
    SIO_D_H();
    SIO_C_H();
    delay_us(50);
    SIO_D_L();
    delay_us(50);
    SIO_C_L();
}

void stopSCCB(void) // SCCB stop signal
{
    SIO_D_L();
    delay_us(50);
    SIO_C_H();
    delay_us(50);
    SIO_D_H();
    delay_us(50);
}

void noAck(void)
{
    delay_us(50);
    SIO_D_H();
    SIO_C_H();
    delay_us(50);
    SIO_C_L();
    delay_us(50);
    SIO_D_L();
    delay_us(50);
}

unsigned int SCCBwriteByte(unsigned int m_data)
{
    unsigned char j, tem;

    for(j = 0; j < 8; j++)
    {
        if(m_data & 0x80)
            SIO_D_H();
        else
            SIO_D_L();
        m_data <<= 1;
        delay_us(50);
        SIO_C_H();
        delay_us(50);
        SIO_C_L();
    }
    SIO_D_IN;
    delay_us(50);
    SIO_C_H();
    delay_us(50);

    if(SIO_D_STATE) // SDA = 1, sending failed, return 0
        tem = 0;
    else // SDA = 0, sending successful, reutrn 1
        tem = 1;
    SIO_C_L();
    SIO_D_OUT; // set SDA as output

    return tem;
}

unsigned char SCCBreadByte(void)
{
    unsigned char read, j;
    read = 0x00;
    SIO_D_IN; // set SDA as input
    for(j = 8; j > 0; j--)
    {
        delay_us(50);
        SIO_C_H();
        read = read << 1;
        if(SIO_D_STATE)
        {
            read++;
        }
        delay_us(50);
        SIO_C_L();
    }
    SIO_D_OUT; // set SDA as output
    return read;
}
