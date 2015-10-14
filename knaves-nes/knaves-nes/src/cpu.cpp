#include <iostream>

#include "cpu.h"

void cpu::start() {
	is_running = true;

	//Set Registers on startup
	reg_pc = STARTUP_PC;
	reg_status = STARTUP_STATUS;
	reg_acc = 0;
	reg_index_x = 0;
	reg_index_y = 0;

}

/*
 Read the next op code from the program counter
 @return The # of cycles the opcode took to run
 */
unsigned short cpu::executeInstruction() {
	if(cpu::checkInterrupts()){
		//If an interrupt ran, it took 7 cycles
		return INTERRUPT_CYCLES;
	}

	branch_taken = false;
	page_boundary_crossed = false;
	

	//Fetch the opcode
	unsigned char opcode = readAddress(reg_pc);
	reg_status = STATUS_EMPTY;

	//Get the instruction associated with the opcode
	std::map<char, instruction>::const_iterator current_instruction = instructions.find(opcode);

	//Invalid opcode
	if (current_instruction == instructions.end()) {
		//throw InvalidOpcodeException(current_opcode);
	}

	//Actually execute the opcode instruction
	instruction entry = current_instruction->second;

	unsigned short cycles_used = entry.cycles;

	if (branch_taken) {
		cycles_used++;
	}

	if (branch_taken && page_boundary_crossed) { //Might not need the second check here...
		cycles_used++;
	}

	if (entry.cycles_extra && page_boundary_crossed) {
		cycles_used++;
	}

	if (entry.skip_bytes) {
		reg_pc += entry.bytes;
	}

	return cycles_used;
}

/*
 Execute an interrupt
*/
unsigned short executeInterrupt(const enum Interrupt &interrupt) {

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
		reg_pc = executeInterrupt(interrupts.front());
		
		//Remove the interrupt from the queue
		interrupts.pop_front();
		
		return true;
	}
}

/*
 Read an address from memory
 @param address	The address to read from
*/
unsigned char cpu::readAddress(unsigned short address) {
	unsigned char value = 0;

	//Not sure what to do here...
}

/*
 Check if the cpu status register includes a given status
*/
bool cpu::hasStatusFlag(unsigned char flag) {
	return (reg_status & flag);
}