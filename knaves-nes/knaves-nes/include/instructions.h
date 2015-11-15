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

#define PHP		0x08
#define PLP		0x28
#define PHA		0x48
#define PLA		0x68

#define JSR		0x20	
#define JMP_ABS		0x4C
#define JMP_IND		0x6C

#define BIT_ZERO	0x24
#define BIT_ABS		0x2C

#define SEI		0x78
#define SED		0xF8
#define SEC		0x38

#define CLC		0x18
#define CLD		0xD8
#define CLV		0xB8
#define CLI		0x58

#define TAX		0xAA
#define TAY		0xA8
#define TSX		0xBA
#define TXA		0x8A
#define TXS		0x9A
#define TYA		0x98

#define LAX_IND_X	0xA3
#define LAX_ZERO	0xA7
#define LAX_IMM		0xAB
#define LAX_ABS		0xAF
#define LAX_IND_Y	0xB3
#define LAX_ZERO_Y	0xB7
#define LAX_ABS_Y	0xBF

#define SAX_IND_X	0x83
#define SAX_ZERO	0x87
#define SAX_ABS		0x8F
#define SAX_ZERO_Y	0x97

#define DCP_IND_X	0xC3
#define DCP_ZERO	0xC7
#define DCP_ABS		0xCF
#define DCP_IND_Y	0xD3
#define DCP_ZERO_X	0xD7
#define DCP_ABS_Y	0xDB
#define DCP_ABS_X	0xDF

#define ISC_IND_X	0xE3
#define ISC_ZERO	0xE7
#define ISC_ABS		0xEF
#define ISC_IND_Y	0xF3
#define ISC_ZERO_X	0xF7
#define ISC_ABS_Y	0xFB
#define ISC_ABS_X	0xFF

#define SLO_IND_X	0x03
#define SLO_ZERO	0x07
#define SLO_ABS		0x0F
#define SLO_IND_Y	0x13
#define SLO_ZERO_X	0x17
#define SLO_ABS_Y	0x1B
#define SLO_ABS_X	0x1F

#define RLA_IND_X	0x23
#define RLA_ZERO	0x27
#define RLA_ABS		0x2F
#define RLA_IND_Y	0x33
#define RLA_ZERO_X	0x37
#define RLA_ABS_Y	0x3B
#define RLA_ABS_X	0x3F

#define SRE_IND_X	0x43
#define SRE_ZERO	0x47
#define SRE_ABS		0x4F
#define SRE_IND_Y	0x53
#define SRE_ZERO_X	0x57
#define SRE_ABS_Y	0x5B
#define SRE_ABS_X	0x5F

#define RRA_IND_X	0x63
#define RRA_ZERO	0x67
#define RRA_ABS		0x6F
#define RRA_IND_Y	0x73
#define RRA_ZERO_X	0x77
#define RRA_ABS_Y	0x7B
#define RRA_ABS_X	0x7F

#define ANC_IMM		0x0B
#define ANC_IMM2	0x2B

#define ALR_IMM		0x4B
#define ARR_IMM		0x6B

#define AXS_IMM		0xCB

#define SHY_ABS_X	0x9C
#define SHX_ABS_Y	0x9E

#define BPL		0x10
#define BNE		0xD0
#define BEQ		0xF0
#define BCC		0x90
#define BCS		0xB0
#define BMI		0x30
#define BVC		0x50
#define BVS		0x70

#define RTS		0x60
#define RTI		0x40

#define BRK		0x00



#endif