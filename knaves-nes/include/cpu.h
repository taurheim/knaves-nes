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
#define STARTUP_STACK		0xFD

//Memory Map http://nesdev.com/NESDoc.pdf
#define STACK_START			0x0100
#define STACK_END			0x0200
#define RAM_START			0x0200
#define RAM_END				0x0800
#define REGISTER_START		0x2000

//CPU Status
#define STATUS_CARRY		0b00000001
#define STATUS_ZERO		    0b00000010
#define STATUS_INTERRUPT	0b00000100
#define STATUS_DECIMAL		0b00001000
#define STATUS_BRK		    0b00010000
#define STATUS_EMPTY		0b00100000
#define STATUS_OVERFLOW		0b01000000
#define STATUS_SIGN		    0b10000000

//Interrupts
#define INTERRUPT_CYCLES		7
#define RESET_VECTOR			0xFFFC
#define IRQ_BRK_VECTOR			0xFFFE
#define NMI_VECTOR				0xFFFA
#define INTERRUPT_RESET_VECTOR_LOW	0xFFFC
#define INTERRUPT_RESET_VECTOR_HIGH 0xFFFD

//Enums
enum Interrupt {NMI, BREAK, IRQ, RESET};

class cpu {
public:
	cpu();

	void init(Memory * memory, bool show_log);
	void start();
	void stop();
	void reset();
	unsigned short executeInstruction();
private:
	bool is_running;
	bool log_instructions;

	Memory *_memory;

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
	unsigned short getSource(Mode mode);
	int branch(signed short offset);

	//Status
	bool hasStatusFlag(unsigned char flag);
	void setStatusFlag(unsigned int flag);
	void clearStatusFlag(unsigned int flag);
	void updateStatusZero(unsigned short val);
	void updateStatusSign(unsigned short val);
	void updateStatusOverflow(unsigned short val, unsigned short mem);
	void updateStatusCarry(unsigned short result);
	void updateStatusBasedOnExpression(bool result, unsigned char flag);
	
	//Memory Management
	unsigned char readAddress(unsigned short address);
	void pushStack(unsigned char byte);
	void pushStackDouble(unsigned short bytebyte);
	unsigned char popStack();

	//Modes
	unsigned short modeRelative();

	//Functions (instructions call these)
	//Naming convention: http://www.thealmightyguru.com/Games/Hacking/Wiki/index.php?title=6502_Opcodes
	
	inline int funcLoadAccumulator(unsigned short src);

	inline int funcStoreAccumulator(unsigned short src);

	inline int funcLoadRegisterX(unsigned short src);
	inline int funcStoreRegisterX(unsigned short src);
	inline int funcLoadRegisterY(unsigned short src);
	inline int funcStoreRegisterY(unsigned short src);

	inline int funcIncreaseRegisterX(unsigned short src);
	inline int funcIncreaseRegisterY(unsigned short src);
	inline int funcDecreaseRegisterX(unsigned short src);
	inline int funcDecreaseRegisterY(unsigned short src);

	inline int funcIncreaseMemory(unsigned short src);
	inline int funcDecreaseMemory(unsigned short src);

	inline int funcCompareRegisterX(unsigned short src);
	inline int funcCompareRegisterY(unsigned short src);


	inline int funcAddWithCarry(unsigned short src);
	inline int funcTransferAccumulatorToX(unsigned short src);
	inline int funcCompareMemory(unsigned short src);

	inline int funcBranchOnResultPlus(unsigned short src);
	inline int funcBranchOnResultMinus(unsigned short src);
	inline int funcBranchOnOverflowClear(unsigned short src);
	inline int funcBranchOnOverflowSet(unsigned short src);
	inline int funcBranchOnCarryClear(unsigned short src);
	inline int funcBranchOnCarrySet(unsigned short src);
	inline int funcBranchOnResultNotZero(unsigned short src);
	inline int funcBranchOnResultZero(unsigned short src);
	
	inline int funcAnd(unsigned short src);
	inline int funcOr(unsigned short src);
	inline int funcXor(unsigned short src);

	inline int funcShiftRightToAccumulator(unsigned short src);
	inline int funcShiftRightToMemory(unsigned short src);

	inline int funcShiftLeftToAccumulator(unsigned short src);
	inline int funcShiftLeftToMemory(unsigned short src);

	inline int funcRotateRightToAccumulator(unsigned short src);
	inline int funcRotateRightToMemory(unsigned short src);

	inline int funcRotateLeftToAccumulator(unsigned short src);
	inline int funcRotateLeftToMemory(unsigned short src);

	inline int funcADC(unsigned short src);
	inline int funcSBC(unsigned short src);

	inline int funcPushStatusToStack(unsigned short src);
	inline int funcPopStatusFromStack(unsigned short src);
	inline int funcPushAccumulatorToStack(unsigned short src);
	inline int funcPopAccumulatorFromStack(unsigned short src);
 
	inline int funcJumpSaveReturnAddress(unsigned short src);
	inline int funcJump(unsigned short src);

	inline int funcBit(unsigned short src);

	inline int funcSetCarryFlag(unsigned short src);
	inline int funcSetDecimalMode(unsigned short src);
	inline int funcSetInterruptDisable(unsigned short src);

	inline int funcClearCarryFlag(unsigned short src);
	inline int funcClearDecimalMode(unsigned short src);
	inline int funcClearInterruptDisable(unsigned short src);
	inline int funcClearOverflowFlag(unsigned short src);

	inline int funcTransferIndexXToStackPointer(unsigned short src);
	inline int funcTransferIndexXToAccumulator(unsigned short src);
	inline int funcTransferStackPointerToIndexX(unsigned short src);
	inline int funcTransferAccumulatorToIndexY(unsigned short src);
	inline int funcTransferAccumulatorToIndexX(unsigned short src);
	inline int funcTransferIndexYToAccumulator(unsigned short src);

	inline int funcLAX(unsigned short src);
	inline int funcSAX(unsigned short src);
	inline int funcDCP(unsigned short src);

	inline int funcISC(unsigned short src);
	inline int funcSLO(unsigned short src);
	inline int funcRLA(unsigned short src);
	inline int funcSRE(unsigned short src);

	inline int funcRRA(unsigned short src);
	inline int funcANC(unsigned short src);
	inline int funcALR(unsigned short src);
	inline int funcARR(unsigned short src);
	inline int funcAXS(unsigned short src);
	inline int funcSHY(unsigned short src);
	inline int funcSHX(unsigned short src);

	inline int funcBranchResultNotZero (unsigned short src);
	inline int funcBranchResultZero(unsigned short src);
	inline int funcBranchCarrySet(unsigned short src);
	inline int funcBranchCarryClear(unsigned short src);
	inline int funcBranchResultMinus(unsigned short src);
	inline int funcBranchResultPlus(unsigned short src);
	inline int funcBranchOverflowClear (unsigned short src);
	inline int funcBranchOverflowSet(unsigned short src);

	inline int funcReturnFromSubroutine(unsigned short src);
	inline int funcReturnFromInterrupt(unsigned short src);
	inline int funcBreak(unsigned short src);

	inline int funcNop(unsigned short src);

	inline int funcKill(unsigned short src);

};

#endif