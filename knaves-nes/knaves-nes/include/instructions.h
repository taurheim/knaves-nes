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

#define OR_IMM		0x09
#define OR_ZERO		0x05
#define OR_ZERO_X	0x15
#define OR_ABS		0x0D
#define OR_ABS_X	0x1D
#define OR_ABS_Y	0x19
#define OR_IND_X	0x01
#define OR_IND_Y	0x11

#define XOR_IMM		0x49
#define XOR_ZERO	0x45
#define XOR_ZERO_X	0x55
#define XOR_ABS		0x4D
#define XOR_ABS_X	0x5D
#define XOR_ABS_Y	0x59
#define XOR_IND_X	0x41
#define XOR_IND_Y	0x51

#define LSR_ACC		0x4A
#define LSR_ZERO	0x46
#define LSR_ZERO_X	0x56
#define LSR_ABS		0x4E
#define LSR_ABS_X	0x5E

#define ASL_ACC		0x0A
#define ASL_ZERO	0x06
#define ASL_ZERO_X	0x16
#define ASL_ABS		0x0E
#define ASL_ABS_X	0x1E

#define ROR_ACC		0x6A
#define ROR_ZERO	0x66
#define ROR_ZERO_X	0x76
#define ROR_ABS		0x6E
#define ROR_ABS_X	0x7E

#define ROL_ACC		0x2A
#define ROL_ZERO	0x26
#define ROL_ZERO_X	0x36
#define ROL_ABS		0x2E
#define ROL_ABS_X	0x3E

#define ADC_IMM		0x69
#define ADC_ZERO	0x65
#define ADC_ZERO_X	0x75
#define ADC_ABS		0x6D
#define ADC_ABS_X	0x7D
#define ADC_ABS_Y	0x79
#define ADC_IND_X	0x61
#define ADC_IND_Y	0x71

#define SBC_IMM		0xE9
#define SBC_IMM2	0xEB
#define SBC_ZERO	0xE5
#define SBC_ZERO_X	0xF5
#define SBC_ABS		0xED
#define SBC_ABS_X	0xFD
#define SBC_ABS_Y	0xF9
#define SBC_IND_X	0xE1
#define SBC_IND_Y	0xF1

#endif