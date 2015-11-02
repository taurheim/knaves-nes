#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

#define LDA_IMM		0xA9

#define STA_ABS		0x8D

#define ADC_ABS		0x6D

#define TAX			0xAA

#define CMP_IMM		0xC9

#define BNE			0xD0

#define AND_IMM		0x29
#define AND_ZERO	0x25
#define AND_ZERO_X	0x35
#define AND_ABS		0x2D
#define AND_ABS_X	0x3D
#define AND_ABS_Y	0x39
#define AND_IND_X	0x21
#define AND_IND_Y	0x31

#endif