#ifndef _CPU_H_
#define _CPU_H_

class cpu;

//CPU Initialization: http://wiki.nesdev.com/w/index.php/CPU_power_up_state
#define STARTUP_PC		0b1100000000000000;
#define STARTUP_STATUS	0b110100;

class cpu {
public:
	cpu();
	void start();
private:
	bool is_running;

	unsigned char *memory;

	//Registers
	//http://codebase64.org/doku.php?id=base:6502_registers
	unsigned short reg_pc; //Program Counter, 16 bits
	unsigned char reg_sp; //Stack Pointer, 8 bits
	unsigned char reg_acc; //Accumulator, 8 bits
	unsigned char reg_status; //Processor Status, 8 bits
	unsigned char reg_index_x; //Index Register X, 8 bits
	unsigned char reg_index_y; //Index Register Y, 8 bits
}

#endif