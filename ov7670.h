

#ifndef OV7670_H_
#define OV7670_H_

#include <msp430.h>
#include "SCCB.h"

#define pixel_w 320
#define pixel_h 240



unsigned char ov7670_init(void);
unsigned char wr_sensor_reg(unsigned char regID, unsigned char *regDat);
unsigned char rd_sensor_reg(unsigned char regID, unsigned char *regDat);
void ov7670_windowSet(unsigned int sx, unsigned int sy, unsigned int width, unsigned int height);


#endif /* OV7670_H_ */
