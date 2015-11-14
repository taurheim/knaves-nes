#include <iostream>

#include "cpu.h"
#include "instructions.h"

cpu::cpu() {
	//Set up opcodes
	instructions = {
		//Load into Accumulator
		{LDA_IMM, instruction { "LDA_IMM",&cpu::funcLoadAccumulator,Mode::IMMEDIATE,2,2,false,true } },
		{LDA_ZERO, instruction {"LDA_ZERO", &cpu::funcLoadAccumulator,Mode::ABSOLUTE_ZERO_PAGE,2,3,false,true}},
		{LDA_ZERO_X, instruction {"LDA_ZERO_X", &cpu::funcLoadAccumulator,Mode::ABSOLUTE_X_ZERO_PAGE,2,4,false,true}},
		{LDA_ABS, instruction {"LDA_ABS", &cpu::funcLoadAccumulator, Mode::ABSOLUTE, 3, 4, false, true}},
		{LDA_ABS_X, instruction {"LDA_ABS_X", &cpu::funcLoadAccumulator, Mode::ABSOLUTE_X, 3, 4, true, true}},
		{LDA_ABS_Y, instruction {"LDA_ABS_Y", &cpu::funcLoadAccumulator, Mode::ABSOLUTE_Y, 3, 4, true, true}},
		{LDA_IND_X, instruction {"LDA_IND_X", &cpu::funcLoadAccumulator, Mode::PRE_INDIRECT_X, 2, 6, false, true}},
		{LDA_IND_Y, instruction {"LDA_IND_Y", &cpu::funcLoadAccumulator, Mode::PRE_INDIRECT_Y, 2, 5, true, true}},

		{STA_ABS, instruction { "STA_ABS",&cpu::funcStoreAccumulator,Mode::ABSOLUTE,3,4,false,true} },

		{ADC_ABS, instruction { "ADC_ABS",&cpu::funcAddWithCarry,Mode::ABSOLUTE,3,4,false,true}},

		{TAX, instruction {"TAX",&cpu::funcTransferAccumulatorToX,Mode::IMPLIED,1,2,false,true}},

		{CMP_IMM, instruction {"CMP_IMM",&cpu::funcCompareMemory,Mode::IMMEDIATE,2,2,false,true}},

		//Branching
		{BPL, instruction {"BPL", &cpu::funcBranchOnResultPlus, Mode::RELATIVE, 2, 2, true, true}},
		{BMI, instruction {"BMI", &cpu::funcBranchOnResultMinus, Mode::RELATIVE, 2, 2, true, true}},
		{BVC, instruction {"BVC", &cpu::funcBranchOnOverflowClear, Mode::RELATIVE, 2, 2, true, true}},
		{BVS, instruction {"BVS", &cpu::funcBranchOnOverflowSet, Mode::RELATIVE, 2, 2, true, true}},
		{BCC, instruction {"BCC", &cpu::funcBranchOnCarryClear, Mode::RELATIVE, 2, 2, true, true}},
		{BCS, instruction {"BCS", &cpu::funcBranchOnCarrySet, Mode::RELATIVE, 2, 2, true, true}},
		{BNE, instruction {"BNE", &cpu::funcBranchOnResultNotZero, Mode::RELATIVE, 2, 2, true, true}},
		{BEQ, instruction {"BEQ", &cpu::funcBranchOnResultZero, Mode::RELATIVE, 2, 2, true, true}},


		{AND_IMM, instruction { "AND_IMM", &cpu::funcAnd, Mode::IMMEDIATE, 2, 2, false, true} },
		{AND_ZERO, instruction { "AND_ZERO", &cpu::funcAnd, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true } },
		{AND_ZERO_X, instruction { "AND_ZERO_X", &cpu::funcAnd, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 4, false, true } },
		{AND_ABS, instruction { "AND_ABS", &cpu::funcAnd, Mode::ABSOLUTE, 3, 4, false, true } },
		{AND_ABS_X,  instruction { "AND_ABS_X", &cpu::funcAnd,Mode::ABSOLUTE_X, 3, 4, true, true } },
		{AND_ABS_Y, instruction { "AND_ABS_Y", &cpu::funcAnd,Mode::ABSOLUTE_Y, 3, 4, true, true } },
		{AND_IND_X, instruction { "AND_IND_X", &cpu::funcAnd, Mode::PRE_INDIRECT_X, 2, 6, false, true } },
		{AND_IND_Y, instruction { "AND_IND_Y", &cpu::funcAnd, Mode::PRE_INDIRECT_Y, 2, 5, true, true } },


		{OR_IMM, instruction {"OR_IMM", &cpu::funcOr, Mode::IMMEDIATE, 2, 2, false, true}},
		{OR_ZERO, instruction {"OR_ZERO", &cpu::funcOr, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{OR_ZERO_X, instruction {"OR_ZERO_X", &cpu::funcOr, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 4, false, true}},
		{OR_ABS,  instruction {"OR_ABS", &cpu::funcOr, Mode::ABSOLUTE, 3, 4, false, true}},
		{OR_ABS_X,  instruction {"OR_ABS_X", &cpu::funcOr, Mode::ABSOLUTE_X, 3, 4, true, true}},
		{OR_ABS_Y,  instruction {"OR_ABS_Y", &cpu::funcOr, Mode::ABSOLUTE_Y, 3, 4, true, true}},
		{OR_IND_X,  instruction {"OR_IND_X", &cpu::funcOr, Mode::PRE_INDIRECT_X, 2, 6, false, true}},
		{OR_IND_Y,  instruction {"OR_IND_Y", &cpu::funcOr, Mode::PRE_INDIRECT_Y, 2, 5, true, true}},

		{XOR_IMM,	 instruction {"XOR_IMM", &cpu::funcXor, Mode::IMMEDIATE, 2, 2, false, true}},
		{XOR_ZERO,	 instruction {"XOR_ZERO", &cpu::funcXor, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{XOR_ZERO_X, instruction {"XOR_ZERO_X", &cpu::funcXor, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 4, false, true}},
		{XOR_ABS, 	 instruction {"XOR_ABS", &cpu::funcXor, Mode::ABSOLUTE, 3, 4, false, true}},
		{XOR_ABS_X,  instruction {"XOR_ABS_X", &cpu::funcXor, Mode::ABSOLUTE_X, 3, 4, true, true}},
		{XOR_ABS_Y,  instruction {"XOR_ABS_Y", &cpu::funcXor, Mode::ABSOLUTE_Y, 3, 4, true, true}},
		{XOR_IND_X,  instruction {"XOR_IND_X", &cpu::funcXor, Mode::POST_INDIRECT_X, 2, 6, false, true}},
		{XOR_IND_Y,  instruction {"XOR_IND_Y", &cpu::funcXor, Mode::POST_INDIRECT_Y, 2, 5, true, true}},

		{LSR_ACC, instruction {"LSR_ACC", &cpu::funcShiftRightToAccumulator, Mode::IMMEDIATE, 1, 2, false, true}},
		{LSR_ZERO, instruction {"LSR_ZERO", &cpu::funcShiftRightToMemory, Mode::ABSOLUTE_ZERO_PAGE, 2, 5, false, true}},
		{LSR_ZERO_X, instruction {"LSR_ZERO_X", &cpu::funcShiftRightToMemory, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 6, false, true}},
		{LSR_ABS, 	instruction {"LSR_ABS", &cpu::funcShiftRightToMemory, Mode::ABSOLUTE, 3, 6, false, true}},
		{LSR_ABS_X, instruction {"LSR_ABS_X", &cpu::funcShiftRightToMemory, Mode::ABSOLUTE_X, 3, 7, false, true}},

		{ASL_ACC, instruction {"ASL_ACC", &cpu::funcShiftLeftToAccumulator, Mode::IMMEDIATE, 1, 2, false, true}},
		{ASL_ZERO, instruction	{"ASL_ZERO", &cpu::funcShiftLeftToMemory, Mode::ABSOLUTE_ZERO_PAGE, 2, 5, false, true}},
		{ASL_ZERO_X, instruction	{"ASL_ZERO_X", &cpu::funcShiftLeftToMemory, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 6, false, true}},
		{ASL_ABS, instruction	{"ASL_ABS", &cpu::funcShiftLeftToMemory, Mode::ABSOLUTE, 3, 6, false, true}},
		{ASL_ABS_X, instruction	{"ASL_ABS_X", &cpu::funcShiftLeftToMemory, Mode::ABSOLUTE_X, 3, 7, false, true}},
	};
}

void cpu::init(Memory * memory) {
	_memory = memory;
}

void cpu::start() {
	is_running = true;

	//Set Registers on startup
	reg_pc = STARTUP_PC;
	reg_status = STARTUP_STATUS;
	reg_acc = 0;
	reg_index_x = 0;
	reg_index_y = 0;

	//Execute RESET Interrupt on startup
	executeInterrupt(Interrupt::RESET);
}

void cpu::reset() {
	executeInterrupt(Interrupt::RESET);
	reg_sp -= 3;
	interrupts.clear();
}

/************
INTERRUPTS
*************/

/*
Check to see if we should execute an interrupt
@return True if an interrupt was run, false otherwise
*/
bool cpu::checkInterrupts() {
	if (interrupts.size() > 0) {

		//Ignore the interrupt if cpu has STATUS_INTERRUPT
		//If it's an NMI interrupt, always run it.
		if (hasStatusFlag(STATUS_INTERRUPT) && interrupts.front() != Interrupt::NMI) {
			return false;
		}

		//Execute the interrupt
		executeInterrupt(interrupts.front());

		//Remove the interrupt from the queue
		interrupts.pop_front();

		return true;
	}
}

/*
 Execute an interrupt
 @param enum interrupt	The interrupt to execute
 @return short			New program counter (this is the location of the instructions executed upon interrupt)
*/
void cpu::executeInterrupt(const enum Interrupt &interrupt) {
	//Push program counter & status register to stack
	pushStack(reg_pc);
	pushStack(reg_status);

	//Add the interrupt status flag (this prevents further interrupts)
	setStatusFlag(STATUS_INTERRUPT);

	//Move the Program Counter to the area of memory with instructions for each interrupt
	switch (interrupt) {
	case Interrupt::NMI:
		reg_pc = NMI_VECTOR;
		break;
	case Interrupt::IRQ:
		reg_pc = IRQ_BRK_VECTOR;
		break;
	case Interrupt::BRK:
		reg_pc = IRQ_BRK_VECTOR;
		break;
	case Interrupt::RESET:
		reg_pc = RESET_VECTOR;
		break;
	default:
		break;
	}
}

/************
INSTRUCTIONS
*************/

/*
Read the next op code from the program counter
@return The # of cycles the opcode took to run
*/
unsigned short cpu::executeInstruction() {
	std::cout << "Executing instruction ";
	if (cpu::checkInterrupts()) {
		//If an interrupt ran, it took 7 cycles
		return INTERRUPT_CYCLES;
	}

	//Fetch the opcode
	unsigned char opcode = readAddress(reg_pc);
	reg_status = STATUS_EMPTY;

	//Get the instruction associated with the opcode
	std::map<char, instruction>::const_iterator instruction_row = instructions.find(opcode);

	//Invalid opcode
	if (instruction_row == instructions.end()) {
		std::cout << "Invalid Instruction";
		//throw InvalidOpcodeException(current_opcode);
	}

	//Actually execute the opcode instruction
	instruction current_instruction = instruction_row->second;

	//Get the source 
	unsigned short src = getSource(current_instruction.mode);

	//Run the function and determine how many cycles were needed
	unsigned short cycles_used = current_instruction.cycles;
	unsigned short extra_cycles = (this->*current_instruction.functionPtr)(src);

	cycles_used += extra_cycles;

	//If it takes extra cycles, add those
	if (current_instruction.cycles_extra) {
		cycles_used++;
	}

	if (current_instruction.skip_bytes) {
		reg_pc += current_instruction.bytes;
	}
	return cycles_used;
}

/*
 Instructions need operands to work on. The different methods of
 getting these operands are called addressing modes.
 @param Mode				Mode to use
 @return unsigned short		Operand for the instruction about to be executed.
*/
unsigned short cpu::getSource(Mode mode) {
	switch (mode) {
	case Mode::ABSOLUTE: {
		//Load from a 16bit memory address
		unsigned char first = _memory->read(reg_pc + 1);
		unsigned char second = _memory->read(reg_pc + 2);
		return (first << 8 | second);
		break;
	}

	case Mode::IMMEDIATE: {
		return (reg_pc + 1);
		break;
	}

						  //Used for branching instructions. Essentially the byte
						  //after the instruction tells the CPU how many bytes to skip if
						  //the branch happens
	case Mode::RELATIVE: {
		return _memory->read(reg_pc + 1);
		break;
	}
	}
}

/*
 Branch to a different memory location.
*/
int cpu::branch(signed short offset) {
	int cycles_used = 1;

	unsigned short before = reg_pc;
	unsigned short after = reg_pc + offset;

	//Check for page boundary crossing (If we're not in the zero page, then it's going to take an extra cycle)
	if ((before ^ after) & 0xFF00) {
		cycles_used++;
	}

	//Do the branch!
	reg_pc = after;

	return cycles_used;
}


/************
MEMORY FUNCTIONS
*************/

unsigned char cpu::readAddress(unsigned short address)
{
	return 0;
}

/*n
 Push a byte to the stack
*/
void cpu::pushStack(unsigned char byte) {
	//Decrement stack pointer because stack is stored top-down
	_memory->write(STACK_START + reg_sp--, byte);
}

/*
 Push 16 bits to the stack (2 bytes)
*/
void cpu::pushStack(unsigned short bytebyte) {
	unsigned char first = (bytebyte >> 8) & 0b11111111;
	unsigned char second = bytebyte & 0b11111111;

	pushStack(first);
	pushStack(second);

	return;
}

/*
 Pop off the stack
*/
unsigned char cpu::popStack() {
	return _memory->read(STACK_START + reg_sp++);
}

/************
 STATUS FLAGS
*************/

/*
 Check if the cpu status register includes a given status
*/
bool cpu::hasStatusFlag(unsigned char flag) {
	return (reg_status & flag);
}

/*
 Add a status flag to the cpu status register
*/
void cpu::setStatusFlag(unsigned char flag) {
	reg_status |= flag;
}

/*
 Remove a status flag from the cpu status register
*/
void cpu::clearStatusFlag(unsigned char flag) {
	reg_status &= !flag;
}

/*
 If necessary, set the Sign status register
 @param val		8 bit value
*/
void cpu::updateStatusSign(unsigned short val) {
	// & 128 to figure out if it's positive or negative
	if (val & 0x0080) {
		setStatusFlag(STATUS_SIGN);
	}
	else {
		clearStatusFlag(STATUS_SIGN);
	}
}

/*
 If necessary, set the Zero status register
 (if val is 0 then set it to 0, otherwise clear the flag)
*/
void cpu::updateStatusZero(unsigned short val) {
	// & 11111111 to figure out if it's zero or not
	if (val & 0x00FF) {
		clearStatusFlag(STATUS_ZERO);
	}
	else {
		setStatusFlag(STATUS_ZERO);
	}
}

/*
 If necessary, set the overflow status register
*/
void cpu::updateStatusOverflow(unsigned short value, unsigned short result) {
	//If (+) + (+) = (-) OR (-) - (-) = (+)
	if (~(reg_acc ^ value) & (reg_acc ^ result) & 0x80) {
		setStatusFlag(STATUS_OVERFLOW);
	}
	else {
		clearStatusFlag(STATUS_OVERFLOW);
	}
}

/*
 If necessary, set the overflow bit to 1
*/
void cpu::updateStatusCarry(unsigned short result) {
	if (result & 0x100) {
		setStatusFlag(STATUS_CARRY);
	}
	else {
		clearStatusFlag(STATUS_CARRY);
	}
}


/************
  OPERATIONS
*************/

//Load a value from an address to the accumulator
int cpu::funcLoadAccumulator(unsigned short src) {
	unsigned short value = _memory->read(src);
	reg_acc = value;
	updateStatusZero(reg_acc);
	updateStatusZero(reg_acc);
	return 0;
}

//Add whatever is in the accumulator to the value at src
int cpu::funcAddWithCarry(unsigned short src) {
	int carry_val = hasStatusFlag(STATUS_CARRY) ? 1 : 0;
	unsigned short result = reg_acc + src + carry_val;

	updateStatusOverflow(src, result);
	updateStatusCarry(result);
	updateStatusSign(result);
	updateStatusZero(result);

	reg_acc = result & 0xFF;
	return 0;
}

int cpu::funcBranchOnResultPlus(unsigned short src) {
	if (!hasStatusFlag(STATUS_SIGN)) {
		signed short branch_to = (signed char)src;
		return branch(branch_to);
	}
	return 0;
}

int cpu::funcBranchOnResultMinus(unsigned short src) {
	if (hasStatusFlag(STATUS_SIGN)) {
		signed short branch_to = (signed char)src;
		return branch(branch_to);
	}
	return 0;
}

int cpu::funcBranchOnCarrySet(unsigned short src) {
	if (hasStatusFlag(STATUS_CARRY)) {
		signed short branch_to = (signed char)src;
		return branch(branch_to);
	}
	return 0;
}

int cpu::funcBranchOnCarryClear(unsigned short src) {
	if (!hasStatusFlag(STATUS_CARRY)) {
		signed short branch_to = (signed char)src;
		return branch(branch_to);
	}
	return 0;
}

int cpu::funcBranchOnOverflowSet(unsigned short src) {
	if (hasStatusFlag(STATUS_OVERFLOW)) {
		signed short branch_to = (signed char)src;
		return branch(branch_to);
	}
	return 0;
}

int cpu::funcBranchOnOverflowClear(unsigned short src) {
	if (!hasStatusFlag(STATUS_OVERFLOW)) {
		signed short branch_to = (signed char)src;
		return branch(branch_to);
	}
	return 0;
}

//If the result of the previous arithmetic operation is not zero, then branch
int cpu::funcBranchOnResultNotZero(unsigned short src) {
	if (!hasStatusFlag(STATUS_ZERO)) {
		signed short branch_to = (signed char)src;
		return branch(branch_to);
	}
	return 0;
}

int cpu::funcBranchOnResultZero(unsigned short src) {
	if (hasStatusFlag(STATUS_ZERO)) {
		signed short branch_to = (signed char)src;
		return branch(branch_to);
	}
	return 0;
}

//Check the current reg_acc value against src
int cpu::funcCompareMemory(unsigned short src) {
	unsigned short value = _memory->read(src);
	unsigned short result = reg_acc - value;

	updateStatusCarry(result);
	updateStatusSign(result);
	updateStatusZero(result);
	return 0;
}

//Store the value currently in the accumulator
int cpu::funcStoreAccumulator(unsigned short src) {
	_memory->write(src, reg_acc);
	return 0;
}


int cpu::funcTransferAccumulatorToX(unsigned short src) {
	reg_index_x = reg_acc;
	updateStatusZero(reg_index_x);
	updateStatusSign(reg_index_x);
	return 0;
}

int cpu::funcAnd(unsigned short src) {
	unsigned short result = src & reg_acc;
	updateStatusZero(result);
	updateStatusSign(result);
	reg_acc = result & 0xFF;
	return 0;
}

int cpu::funcOr(unsigned short src) {
	unsigned short result = src | reg_acc;
	updateStatusZero(result);
	updateStatusSign(result);
	reg_acc = result;
	return 0;
}

int cpu::funcXor(unsigned short src) {
	unsigned short result = reg_acc ^ src;
	updateStatusZero(result);
	updateStatusSign(result);
	reg_acc = result;
	return 0;
}

int cpu::funcShiftRightToAccumulator(unsigned short src) {
	unsigned short value = reg_acc;
	unsigned short result = value >> 1;
	updateStatusCarry(value);
	updateStatusZero(result);
	updateStatusSign(result);
	reg_acc = result;
	return 0;
}

int cpu::funcShiftRightToMemory(unsigned short src) {
	unsigned short result = src >> 1;
	updateStatusCarry(src);
	updateStatusZero(result);
	updateStatusSign(result);
	_memory -> write(src, result);
	return 0;
}

int cpu::funcShiftLeftToAccumulator(unsigned short src) {
	unsigned short value = reg_acc;
	value <<= 1;
	updateStatusCarry(value);
	updateStatusZero(value);
	updateStatusSign(value);
	reg_acc = value;
	return 0;
}

int cpu::funcShiftLeftToMemory(unsigned short src) {
	unsigned short value = src;
	value <<= 1;
	updateStatusCarry(value);
	updateStatusZero(value);
	updateStatusSign(value);
	_memory -> write(src, value);
	return 0;
}
