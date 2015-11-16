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
	RELATIVE,
	S_ABSOLUTE,
	S_ABSOLUTE_X,
	S_ABSOLUTE_Y,
	S_ABSOLUTE_ZERO_PAGE,
	S_ABSOLUTE_X_ZERO_PAGE,
	S_ABSOLUTE_Y_ZERO_PAGE,
	S_PRE_INDIRECT_X,
	S_PRE_INDIRECT_Y
};

typedef struct {
	//Readable string for print statements
	std::string name;

	//This points to the function that will get run for the instruction. It returns the # of (extra) cycles it took
	int (cpu::*functionPtr)(unsigned short src);

	//Addressing Mode.
	//This tells the CPU where to get the instruction's operands from.
	Mode mode;

	//How large is the instruction?
	char bytes;

	//How many cycles does it take for this instruction to execute?
	char cycles;

	//If this is true, then there is a possibility for this instruction to need an extra cycle to compute the address
	bool cycles_extra;

	//If this is true, then we need to move the pc forward
	//We might be able to remove this, the only places where it's false is JMP and BRK
	bool skip_bytes;
} instruction;



#endif