#include <iostream>

#include "cpu.h"
#include "instructions.h"

cpu::cpu() {
	//Set up opcodes
	instructions = {
		{LDA_IMM, instruction { "LDA_IMM",&cpu::funcLoadAccumulator,Mode::IMMEDIATE,2,2,false,true } },
		{STA_ABS, instruction { "STA_ABS",&cpu::funcStoreAccumulator,Mode::ABSOLUTE,3,4,false,true} },
		{ADC_ABS, instruction { "ADC_ABS",&cpu::funcAddWithCarry,Mode::ABSOLUTE,3,4,false,true}},
		{TAX, instruction {"TAX",&cpu::funcTransferAccumulatorToX,Mode::IMPLIED,1,2,false,true}},
		{CMP_IMM, instruction {"CMP_IMM",&cpu::funcCompareMemory,Mode::IMMEDIATE,2,2,false,true}},
		{BNE, instruction {"BNE",&cpu::funcBranchNotEqualZero,Mode::RELATIVE,2,2,true,true}}
	};
}

void cpu::init(Memory * memory) {
	_memory = memory;
}

void cpu::start() {
	is_running = true;

	//Set Registers on startup

	//OVERRIDING, FIX THIS LATER
	reg_pc = 0x8010;
	reg_status = STARTUP_STATUS;
	reg_acc = 0;
	reg_index_x = 0;
	reg_index_y = 0;

	//Execute RESET Interrupt on startup
	//OVERRIDING, FIX THIS LATER
	//executeInterrupt(Interrupt::RESET);
}

void cpu::reset() {
	executeInterrupt(Interrupt::RESET);
	reg_sp -= 3;
	interrupts.clear();
}

/*
 Read the next op code from the program counter
 @return The # of cycles the opcode took to run
 */
unsigned short cpu::executeInstruction() {
	std::cout << "Executing instruction \n";
	if(cpu::checkInterrupts()){
		//If an interrupt ran, it took 7 cycles
		return INTERRUPT_CYCLES;
	}

	branch_taken = false;
	page_boundary_crossed = false;
	

	//Fetch the opcode
	unsigned char opcode = readAddress(reg_pc);
	reg_status = STATUS_EMPTY;

	std::cout << std::hex << opcode;

	//Get the instruction associated with the opcode
	std::map<char, instruction>::const_iterator instruction_row = instructions.find(opcode);

	//Invalid opcode
	if (instruction_row == instructions.end()) {
		std::cout << "Invalid Instruction";
		//throw InvalidOpcodeException(current_opcode);
		return 0;
	}

	//Actually execute the opcode instruction
	instruction current_instruction = instruction_row->second;

	//Get the source 
	src = getSource(current_instruction.mode);

	//Run the correct function

	(this->*current_instruction.functionPtr)();
	unsigned short cycles_used = current_instruction.cycles;

	if (branch_taken) {
		cycles_used++;
	}

	if (branch_taken && page_boundary_crossed) { //Might not need the second check here...
		cycles_used++;
	}

	if (current_instruction.cycles_extra && page_boundary_crossed) {
		cycles_used++;
	}

	if (current_instruction.skip_bytes) {
		reg_pc += current_instruction.bytes;
	}


	return cycles_used;
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
	}
}


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

unsigned char cpu::readAddress(unsigned short address)
{
	return _memory->read(address);
}

/*
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
	} else {
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
void cpu::funcLoadAccumulator() {
	value = _memory->read(src);
	reg_acc = value;
	updateStatusZero(reg_acc);
	updateStatusZero(reg_acc);
}

//Add whatever is in the accumulator to the value at src
void cpu::funcAddWithCarry() {
	value = _memory->read(src);
	int carry_val = hasStatusFlag(STATUS_CARRY) ? 1 : 0;
	result = reg_acc + value + carry_val;
	updateStatusOverflow(value, result);
	updateStatusCarry(result);
	reg_acc = result & 0xFF;
	updateStatusSign(reg_acc);
	updateStatusZero(reg_acc);
}

//If the result of the previous arithmetic operation is not zero, then branch
void cpu::funcBranchNotEqualZero() {
	if (!hasStatusFlag(STATUS_ZERO)) {
		branch_taken = true;
		//Branch
	}
}

//Check the current reg_acc value against src
void cpu::funcCompareMemory() {
	value = _memory->read(src);
	result = reg_acc - value;

	updateStatusCarry(result);
	updateStatusSign(result);
	updateStatusZero(result);
}

//Store the value currently in the accumulator
void cpu::funcStoreAccumulator() {
	_memory->write(src, reg_acc);
}


void cpu::funcTransferAccumulatorToX() {
	reg_index_x = reg_acc;
	updateStatusZero(reg_index_x);
	updateStatusSign(reg_index_x);
}