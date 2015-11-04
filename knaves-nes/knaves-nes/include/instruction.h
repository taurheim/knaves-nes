#ifndef _OPCODE_H_
#define _OPCODE_H_

#include <string>

class cpu;

enum Mode {
	IMMEDIATE,
	IMPLIED,
	ABSOLUTE,
	ABSOLUTE_X,
	ABSOLUTE_Y,
	ABSOLUTE_ZERO_PAGE,
	ABSOLUTE_X_ZERO_PAGE,
	ABSOLUTE_Y_ZERO_PAGE,
	INDIRECT,
	PRE_INDIRECT_X,
	PRE_INDIRECT_Y,
	POST_INDIRECT,
	POST_INDIRECT_X,
	POST_INDIRECT_Y,
	RELATIVE
};

typedef struct {
	std::string name;
	int (cpu::*functionPtr)(unsigned short src); //Run this (returns number of cycles)
	Mode mode;
	char bytes;
	char cycles;
	bool cycles_extra;
	bool skip_bytes;
	//Not sure if we need this
	//unsigned char dummy;
} instruction;



#endif