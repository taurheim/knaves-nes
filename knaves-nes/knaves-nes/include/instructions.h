#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

//Load into Accumulator
#define LDA_IMM		0xA9
#define LDA_ZERO	0xA5
#define LDA_ZERO_X	0xB5
#define LDA_ABS		0xAD
#define LDA_ABS_X	0xBD
#define LDA_ABS_Y	0xB9
#define LDA_IND_X	0xA1
#define LDA_IND_Y	0xB1

#define STA_ABS		0x8D

#define ADC_ABS		0x6D

#define TAX			0xAA

#define CMP_IMM		0xC9

//Branching
#define BPL			0x10
#define BMI			0x30
#define BVC			0x50
#define BVS			0x70
#define BCC			0x90
#define BCS			0xB0
#define BNE			0xD0
#define BEQ			0xF0

#define AND_IMM		0x29
#define AND_ZERO	0x25
#define AND_ZERO_X	0x35
#define AND_ABS		0x2D
#define AND_ABS_X	0x3D
#define AND_ABS_Y	0x39
#define AND_IND_X	0x21
#define AND_IND_Y	0x31

#endif