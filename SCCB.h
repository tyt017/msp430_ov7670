/*
 * SCCB.h
 *
 *  Created on: 2024/12/22
 *      Author: USER
 */

#ifndef SCCB_H_
#define SCCB_H_

#include <msp430.h>
#include "pins.h"

void delay_us(unsigned int us);
void SCCB_init(void);
void startSCCB(void);
void stopSCCB(void);
void noAck(void);
unsigned int SCCBwriteByte(unsigned int m_data);
unsigned char SCCBreadByte(void);


#endif /* SCCB_H_ */
