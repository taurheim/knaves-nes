#ifndef _CPU_H_
#define _CPU_H_

#include <list>
#include <map>

#include "instruction.h";
#include "memory.h"

class cpu;
class Memory;

//CPU Initialization: http://wiki.nesdev.com/w/index.php/CPU_power_up_state
#define STARTUP_PC			0b1100000000000000
#define STARTUP_STATUS		0b110100 //STATUS_SIGN, STATUS_OVERFLOW, STATUS_BRK

//Memory Map http://nesdev.com/NESDoc.pdf
#define STACK_START			0x0100
#define STACK_END			0x0200
#define RAM_START			0x0200
#define RAM_END				0x0800
#define REGISTER_START		0x2000

//CPU Status
#define STATUS_CARRY		0x00000001
#define STATUS_ZERO		    0x00000010
#define STATUS_INTERRUPT	0x00000100
#define STATUS_DECIMAL		0x00001000
#define STATUS_BRK		    0x00010000
#define STATUS_EMPTY		0x00100000
#define STATUS_OVERFLOW		0x01000000
#define STATUS_SIGN		    0x10000000

//Interrupts
#define INTERRUPT_CYCLES		7
#define RESET_VECTOR			0xFFFC
#define IRQ_BRK_VECTOR			0xFFFE
#define NMI_VECTOR				0xFFFA

//Enums
enum Interrupt {NMI, BRK, IRQ, RESET};

class cpu {
public:
	cpu();

	void init(Memory * memory);
	void start();
	void stop();
	void reset();
	unsigned short executeInstruction();
private:
	bool is_running;

	Memory *_memory;

	//Runtime
	bool branch_taken;
	bool page_boundary_crossed;

	//Registers
	//http://codebase64.org/doku.php?id=base:6502_registers
	unsigned short reg_pc; //Program Counter, 16 bits
	unsigned char reg_sp; //Stack Pointer, 8 bits
	unsigned char reg_acc; //Accumulator, 8 bits
	unsigned char reg_status; //Processor Status, 8 bits
	unsigned char reg_index_x; //Index Register X, 8 bits
	unsigned char reg_index_y; //Index Register Y, 8 bits

	std::list<Interrupt> interrupts;
	std::map<char, instruction> instructions;

	//Interrupts
	void executeInterrupt(const enum Interrupt &interrupt);
	bool checkInterrupts();

	//Instructions
	void setMode(Mode mode);

	//Helpers
	bool hasStatusFlag(unsigned char flag);
	
	//Memory Management
	unsigned char readAddress(unsigned short address);
	void pushStack(unsigned char byte);
	void pushStack(unsigned short bytebyte);

	//Functions (instructions call these)
	inline void funcLoadAccumulator();
	inline void funcStoreAccumulator();
	inline void funcAddWithCarry();
	inline void funcTransferAccumulatorToX();
	inline void funcCompareMemory();
	inline void funcBranchNotEqual();
};

#endif