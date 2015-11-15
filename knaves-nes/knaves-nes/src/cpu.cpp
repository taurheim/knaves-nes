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

		{ROR_ACC, instruction	{"ROR_ACC", &cpu::funcRotateRightToAccumulator, Mode::IMMEDIATE, 1, 2, false, true}},
		{ROR_ZERO, instruction	{"ROR_ZERO", &cpu::funcRotateRightToMemory, Mode::ABSOLUTE_ZERO_PAGE, 2, 5, false, true}},
		{ROR_ZERO_X, instruction {"ROR_ZERO_X", &cpu::funcRotateRightToMemory, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 6, false, true}},
		{ROR_ABS, instruction	{"ROR_ABS", &cpu::funcRotateRightToMemory, Mode::ABSOLUTE, 3, 6, false, true}},
		{ROR_ABS_X, instruction {"ROR_ABS_X", &cpu::funcRotateRightToMemory, Mode::ABSOLUTE_X, 3, 7, false, true}},

		{ROL_ACC, instruction {"ROL_ACC", &cpu::funcRotateLeftToAccumulator, Mode::IMMEDIATE, 1, 2, false, true}},
		{ROL_ZERO, instruction	{"ROL_ZERO", &cpu::funcRotateLeftToMemory, Mode::ABSOLUTE_ZERO_PAGE, 2, 5, false, true}},
		{ROL_ZERO_X, instruction	{"ROL_ZERO_X", &cpu::funcRotateLeftToMemory, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 6, false, true}},
		{ROL_ABS, instruction	{"ROL_ABS", &cpu::funcRotateLeftToMemory, Mode::ABSOLUTE, 3, 6, false, true}},
		{ROL_ABS_X, instruction	{"ROL_ABS_X", &cpu::funcRotateLeftToMemory, Mode::ABSOLUTE_X, 3, 7, false, true}},

		{ADC_IMM, 	instruction {"ADC_IMM", &cpu::funcADC, Mode::IMMEDIATE, 2, 2, false, true}},
		{ADC_ZERO,	instruction	{"ADC_ZERO", &cpu::funcADC, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{ADC_ZERO_X, instruction{"ADC_ZERO_X", &cpu::funcADC, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 4, false, true}},
		{ADC_ABS,  	instruction{"ADC_ABS", &cpu::funcADC, Mode::ABSOLUTE, 3, 4, false, true}},
		{ADC_ABS_X, instruction	{"ADC_ABS_X", &cpu::funcADC, Mode::ABSOLUTE_X, 3, 4, true, true}},
		{ADC_ABS_Y, instruction	{"ADC_ABS_Y", &cpu::funcADC, Mode::ABSOLUTE_Y, 3, 4, true, true}},
		{ADC_IND_X, instruction	{"ADC_IND_X", &cpu::funcADC, Mode::PRE_INDIRECT_X, 2, 6, false, true}},
		{ADC_IND_Y, instruction	{"ADC_IND_Y", &cpu::funcADC, Mode::POST_INDIRECT_Y, 2, 5, true, true}},

		{SBC_IMM, 	instruction {"SBC_IMM", &cpu::funcSBC, Mode::IMMEDIATE, 2, 2, false, true}},
		{SBC_IMM2,	instruction {"SBC_IMM2", &cpu::funcSBC, Mode::IMMEDIATE, 2, 2, false, true}},
		{SBC_ZERO, 	instruction	{"SBC_ZERO", &cpu::funcSBC, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{SBC_ZERO_X, instruction{"SBC_ZERO_X", &cpu::funcSBC, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 4, false, true}},
		{SBC_ABS, 	instruction {"SBC_ABS", &cpu::funcSBC, Mode::ABSOLUTE, 3, 4, false, true}},
		{SBC_ABS_X, instruction	{"SBC_ABS_X", &cpu::funcSBC, Mode::ABSOLUTE_X, 3, 4, true, true}},
		{SBC_ABS_Y, instruction	{"SBC_ABS_Y", &cpu::funcSBC, Mode::ABSOLUTE_Y, 3, 4, true, true}},
		{SBC_IND_X, instruction	{"SBC_IND_X", &cpu::funcSBC, Mode::PRE_INDIRECT_X, 2, 6, false, true}},
		{SBC_IND_Y, instruction	{"SBC_IND_Y", &cpu::funcSBC, Mode::POST_INDIRECT_Y, 2, 5, true, true}},

		{PHP, 	instruction {"PHP", &cpu::funcPushStatusToStack, Mode::IMPLIED, 1, 3, false, true}},
		{PLP, 	instruction {"PLP", &cpu::funcPopStatusFromStack, Mode::IMPLIED, 1, 4, false, true}},
		{PHA, 	instruction {"PHA", &cpu::funcPushAccumulatorToStack, Mode::IMPLIED, 1, 3, false, true}},
		{PLA, 	instruction {"PLA", &cpu::funcPopAccumulatorFromStack, Mode::IMPLIED, 1, 4, false, true}},


		{JSR, 	instruction {"JSR", &cpu::funcJumpSaveReturnAddress, Mode::ABSOLUTE, 3, 6, false, false}},
		{JMP_ABS,instruction {"JMP_ABS", &cpu::funcJump, Mode::ABSOLUTE, 3, 3, false, false}},
		{JMP_IND,instruction {"JMP_IND", &cpu::funcJump, Mode::INDIRECT, 3, 5, false, false}},


		{BIT_ZERO,instruction {"BIT_ZERO", &cpu::funcBit, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{BIT_ABS, instruction {"BIT_ABS", &cpu::funcBit, Mode::ABSOLUTE, 3, 4, false, true}},


		{SEC, 	instruction {"SEC", &cpu::funcSetCarryFlag, Mode::IMPLIED, 1, 2, false, true}},
		{SED, 	instruction {"SED", &cpu::funcSetDecimalMode, Mode::IMPLIED, 1, 2, false, true}},
		{SEI, 	instruction {"SEI", &cpu::funcSetInterruptDisable, Mode::IMPLIED, 1, 2, false, true}},

		{CLC, 	instruction {"CLC", &cpu::funcClearCarryFlag, Mode::IMPLIED, 1, 2, false, true}},
		{CLD, 	instruction {"CLD", &cpu::funcClearDecimalMode, Mode::IMPLIED, 1, 2, false, true}},
		{CLI, 	instruction {"CLI", &cpu::funcClearInterruptDisable, Mode::IMPLIED, 1, 2, false, true}},
		{CLV, 	instruction {"CLV", &cpu::funcClearOverflowFlag, Mode::IMPLIED, 1, 2, false, true}},


		{TXS, 	instruction {"TXS", &cpu::funcTransferIndexXToStackPointer, Mode::IMPLIED, 1, 2, false, true}},
		{TXA, 	instruction {"TXA", &cpu::funcTransferIndexXToAccumulator, Mode::IMPLIED, 1, 2, false, true}},
		{TSX, 	instruction {"TSX", &cpu::funcTransferStackPointerToIndexX, Mode::IMPLIED, 1, 2, false, true}},
		{TAY, 	instruction {"TAY", &cpu::funcTransferAccumulatorToIndexY, Mode::IMPLIED, 1, 2, false, true}},
		{TAX, 	instruction {"TAX", &cpu::funcTransferAccumulatorToIndexX, Mode::IMPLIED, 1, 2, false, true}},
		{TYA, 	instruction {"TYA", &cpu::funcTransferIndexYToAccumulator, Mode::IMPLIED, 1, 2, false, true}},

		{LAX_IND_X,	instruction {"LAX_IND_X", &cpu::funcLAX, Mode::PRE_INDIRECT_X, 2, 6, false, true}},
		{LAX_ZERO,	instruction {"LAX_ZERO", &cpu::funcLAX, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{LAX_IMM,	instruction {"LAX_IMM", &cpu::funcLAX, Mode::IMMEDIATE, 2, 2, false, true}},
		{LAX_ABS,	instruction {"LAX_ABS", &cpu::funcLAX, Mode::ABSOLUTE, 3, 4, false, true}},
		{LAX_IND_Y,	instruction {"LAX_IND_Y", &cpu::funcLAX, Mode::POST_INDIRECT_Y, 2, 5, true, true}},
		{LAX_ZERO_Y,instruction {"LAX_ZERO_Y", &cpu::funcLAX, Mode::ABSOLUTE_Y_ZERO_PAGE, 2, 4, false, true}},
		{LAX_ABS_Y,	instruction {"LAX_ABS_Y", &cpu::funcLAX, Mode::ABSOLUTE_Y, 3, 4, true, true}},

		{SAX_IND_X,	instruction {"SAX_IND_X", &cpu::funcSAX, Mode::PRE_INDIRECT_X, 2, 6, false, true}},
		{SAX_ZERO,	instruction {"SAX_ZERO", &cpu::funcSAX, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{SAX_ABS,	instruction {"SAX_ABS", &cpu::funcSAX, Mode::ABSOLUTE, 3, 4, false, true}},
		{SAX_ZERO_Y,instruction {"SAX_ZERO_Y", &cpu::funcSAX, Mode::ABSOLUTE_Y_ZERO_PAGE, 2, 4, false, true}},

		{DCP_IND_X,	instruction {"DCP_IND_X", &cpu::funcDCP, Mode::PRE_INDIRECT_X, 2, 8, false, true}},
		{DCP_ZERO,	instruction {"DCP_ZERO", &cpu::funcDCP, Mode::ABSOLUTE_ZERO_PAGE, 2, 5, false, true}},
		{DCP_ABS,	instruction {"DCP_ABS", &cpu::funcDCP, Mode::ABSOLUTE, 3, 6, false, true}},
		{DCP_IND_Y,	instruction {"DCP_IND_Y", &cpu::funcDCP, Mode::POST_INDIRECT_Y, 2, 8, false, true}},
		{DCP_ZERO_X,instruction {"DCP_ZERO_X", &cpu::funcDCP, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 6, false, true}},
		{DCP_ABS_Y,	instruction {"DCP_ABS_Y", &cpu::funcDCP, Mode::ABSOLUTE_Y, 3, 7, false, true}},
		{DCP_ABS_X,	instruction {"DCP_ABS_X", &cpu::funcDCP, Mode::ABSOLUTE_X, 3, 7, false, true}},

		{ISC_IND_X,	instruction {"ISC_IND_X", &cpu::funcISC, Mode::PRE_INDIRECT_X, 2, 8, false, true}},
		{ISC_ZERO,	instruction {"ISC_ZERO", &cpu::funcISC, Mode::ABSOLUTE_ZERO_PAGE, 2, 5, false, true}},
		{ISC_ABS,	instruction {"ISC_ABS", &cpu::funcISC, Mode::ABSOLUTE, 3, 6, false, true}},
		{ISC_IND_Y,	instruction {"ISC_IND_Y", &cpu::funcISC, Mode::POST_INDIRECT_Y, 2, 8, false, true}},
		{ISC_ZERO_X,instruction {"ISC_ZERO_X", &cpu::funcISC, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 6, false, true}},
		{ISC_ABS_Y,	instruction {"ISC_ABS_Y", &cpu::funcISC, Mode::ABSOLUTE_Y, 3, 7, false, true}},
		{ISC_ABS_X,	instruction {"ISC_ABS_X", &cpu::funcISC, Mode::ABSOLUTE_X, 3, 7, false, true}},

		{SLO_IND_X,	instruction {"SLO_IND_X", &cpu::funcSLO, Mode::PRE_INDIRECT_X, 2, 8, false, true}},
		{SLO_ZERO,	instruction {"SLO_ZERO", &cpu::funcSLO, Mode::ABSOLUTE_ZERO_PAGE, 2, 5, false, true}},
		{SLO_ABS,	instruction {"SLO_ABS", &cpu::funcSLO, Mode::ABSOLUTE, 3, 6, false, true}},
		{SLO_IND_Y,	instruction {"SLO_IND_Y", &cpu::funcSLO, Mode::POST_INDIRECT_Y, 2, 8, false, true}},
		{SLO_ZERO_X,instruction {"SLO_ZERO_X", &cpu::funcSLO, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 6, false, true}},
		{SLO_ABS_Y,	instruction {"SLO_ABS_Y", &cpu::funcSLO, Mode::ABSOLUTE_Y, 3, 7, false, true}},
		{SLO_ABS_X,	instruction {"SLO_ABS_X", &cpu::funcSLO, Mode::ABSOLUTE_X, 3, 7, false, true}},

		{RLA_IND_X,	instruction {"RLA_IND_X", &cpu::funcRLA, Mode::PRE_INDIRECT_X, 2, 8, false, true}},
		{RLA_ZERO,	instruction {"RLA_ZERO", &cpu::funcRLA, Mode::ABSOLUTE_ZERO_PAGE, 2, 5, false, true}},
		{RLA_ABS,	instruction {"RLA_ABS", &cpu::funcRLA, Mode::ABSOLUTE, 3, 6, false, true}},
		{RLA_IND_Y,	instruction {"RLA_IND_Y", &cpu::funcRLA, Mode::POST_INDIRECT_Y, 2, 8, false, true}},
		{RLA_ZERO_X,instruction {"RLA_ZERO_X", &cpu::funcRLA, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 6, false, true}},
		{RLA_ABS_Y,	instruction {"RLA_ABS_Y", &cpu::funcRLA, Mode::ABSOLUTE_Y, 3, 7, false, true}},
		{RLA_ABS_X,	instruction {"RLA_ABS_X", &cpu::funcRLA, Mode::ABSOLUTE_X, 3, 7, false, true}},

		{SRE_IND_X,	instruction {"SRE_IND_X", &cpu::funcSRE, Mode::PRE_INDIRECT_X, 2, 8, false, true}},
		{SRE_ZERO,	instruction {"SRE_ZERO", &cpu::funcSRE, Mode::ABSOLUTE_ZERO_PAGE, 2, 5, false, true}},
		{SRE_ABS,	instruction {"SRE_ABS", &cpu::funcSRE, Mode::ABSOLUTE, 3, 6, false, true}},
		{SRE_IND_Y,	instruction {"SRE_IND_Y", &cpu::funcSRE, Mode::POST_INDIRECT_Y, 2, 8, false, true}},
		{SRE_ZERO_X,instruction {"SRE_ZERO_X", &cpu::funcSRE, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 6, false, true}},
		{SRE_ABS_Y,	instruction {"SRE_ABS_Y", &cpu::funcSRE, Mode::ABSOLUTE_Y, 3, 7, false, true}},
		{SRE_ABS_X,	instruction {"SRE_ABS_X", &cpu::funcSRE, Mode::ABSOLUTE_X, 3, 7, false, true}},

		{RRA_IND_X,	instruction {"RRA_IND_X", &cpu::funcRRA, Mode::PRE_INDIRECT_X, 2, 8, false, true}},
		{RRA_ZERO,	instruction {"RRA_ZERO", &cpu::funcRRA, Mode::ABSOLUTE_ZERO_PAGE, 2, 5, false, true}},
		{RRA_ABS,	instruction {"RRA_ABS", &cpu::funcRRA, Mode::ABSOLUTE, 3, 6, false, true}},
		{RRA_IND_Y,	instruction {"RRA_IND_Y", &cpu::funcRRA, Mode::POST_INDIRECT_Y, 2, 8, false, true}},
		{RRA_ZERO_X,instruction {"RRA_ZERO_X", &cpu::funcRRA, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 6, false, true}},
		{RRA_ABS_Y,	instruction {"RRA_ABS_Y", &cpu::funcRRA, Mode::ABSOLUTE_Y, 3, 7, false, true}},
		{RRA_ABS_X,	instruction {"RRA_ABS_X", &cpu::funcRRA, Mode::ABSOLUTE_X, 3, 7, false, true}},

		{ANC_IMM,	instruction {"ANC_IMM", &cpu::funcANC, Mode::IMMEDIATE, 2, 2, false, true}},
		{ANC_IMM2,	instruction {"ANC_IMM", &cpu::funcANC, Mode::IMMEDIATE, 2, 2, false, true}},

		{ALR_IMM,	instruction {"ALR_IMM", &cpu::funcALR, Mode::IMMEDIATE, 2, 2, false, true}},
 
		{ARR_IMM,	instruction {"ARR_IMM", &cpu::funcARR, Mode::IMMEDIATE, 2, 2, false, true}},

		{AXS_IMM,	instruction {"AXS_IMM", &cpu::funcAXS, Mode::IMMEDIATE, 2, 2, false, true}},

		{SHY_ABS_X,	instruction {"SHY_ABS_X", &cpu::funcSHY, Mode::ABSOLUTE_X, 3, 5, false, true}},
		{SHX_ABS_Y,	instruction {"SHX_ABS_Y", &cpu::funcSHX, Mode::ABSOLUTE_Y, 3, 5, false, true}},

		{BNE, 		instruction {"BNE", &cpu::funcBranchResultNotZero, Mode::RELATIVE, 2, 2, true, true}},
		{BEQ, 		instruction {"BEQ", &cpu::funcBranchResultZero, Mode::RELATIVE, 2, 2, true, true}},
		{BCS, 		instruction {"BCS", &cpu::funcBranchCarrySet, Mode::RELATIVE, 2, 2, true, true}},
		{BCC, 		instruction {"BCC", &cpu::funcBranchCarryClear, Mode::RELATIVE, 2, 2, true, true}},
		{BMI, 		instruction {"BMI", &cpu::funcBranchResultMinus, Mode::RELATIVE, 2, 2, true, true}},
		{BPL, 		instruction {"BPL", &cpu::funcBranchResultPlus, Mode::RELATIVE, 2, 2, true, true}},
		{BVC, 		instruction {"BVC", &cpu::funcBranchOverflowClear, Mode::RELATIVE, 2, 2, true, true}},
		{BVS, 		instruction {"BVS", &cpu::funcBranchOverflowSet, Mode::RELATIVE, 2, 2, true, true}},

		{RTS, 		instruction {"RTS", &cpu::funcReturnFromSubroutine, Mode::IMPLIED, 1, 6, false, false}},
		{RTI, 		instruction {"RTI", &cpu::funcReturnFromInterrupt, Mode::IMPLIED, 1, 6, false, false}},
		{BRK, 		instruction {"BRK", &cpu::funcBreak, Mode::IMPLIED, 1, 7, false, false}},

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
	unsigned short value = (reg_acc <<= 1);
	updateStatusCarry(value);
	updateStatusZero(value);
	updateStatusSign(value);
	reg_acc = value;
	return 0;
}

int cpu::funcShiftLeftToMemory(unsigned short src) {
	unsigned short value = (src <<= 1);
	updateStatusCarry(value);
	updateStatusZero(value);
	updateStatusSign(value);
	_memory -> write(src, value);
	return 0;
}

int cpu::funcRotateRightToAccumulator(unsigned short src) {
	unsigned short value = reg_acc;
	unsigned short result = (value >> 1) | ((reg_status & STATUS_CARRY) << 7);

	updateStatusCarry(value);
	updateStatusZero(result);
	updateStatusSign(result);
	reg_acc = result;
	return 0;
}

int cpu::funcRotateRightToMemory(unsigned short src) {
	unsigned short value = src;
	unsigned short result = (value >> 1) | ((reg_status & STATUS_CARRY) << 7);

	updateStatusCarry(value);
	updateStatusZero(result);
	updateStatusSign(result);
	_memory -> write(src, result);
	return 0;
}

int cpu::funcRotateLeftToAccumulator(unsigned short src) {
    unsigned short value = reg_acc;
	unsigned short result = (value << 1) | (reg_status & STATUS_CARRY);

	updateStatusCarry(result);
	updateStatusZero(result);
	updateStatusSign(result);
	reg_acc = result;
	return 0;
}

int cpu::funcRotateLeftToMemory(unsigned short src) {
	unsigned short value = src;
	unsigned short result = (value << 1) | (reg_status & STATUS_CARRY);

	updateStatusCarry(result);
	updateStatusZero(result);
	updateStatusSign(result);
	_memory -> write(src, result);
	return 0;
}

int cpu::funcADC(unsigned short src) {
	unsigned short value = src;
	unsigned short result = reg_acc + value + (hasStatusFlag(STATUS_CARRY) ? 1 : 0);
	updateStatusOverflow(~(reg_acc ^ value) & (reg_acc ^ result));
	updateStatusCarry(result);
	reg_acc = result & 0xFF;
	updateStatusZero(reg_acc);
	updateStatusSign(reg_acc >> 7);
	return 0;
}

int cpu::funcSBC(unsigned short src) {
	unsigned short value = src;
	unsigned short result = reg_acc + ~value + (hasStatusFlag(STATUS_CARRY) ? 1 : 0);
	updateStatusOverflow((reg_acc ^ value) & (reg_acc ^ result));
	updateStatusCarry(!result);//to be checked by niko
	reg_acc = result & 0xFF;
	updateStatusZero(reg_acc);
	updateStatusSign(reg_acc >> 7);
	return 0; 
}

int cpu::funcPushStatusToStack(unsigned short src) {
	pushStack(reg_status | STATUS_BRK | STATUS_EMPTY);
	return 0;
}

int cpu::funcPopStatusFromStack(unsigned short src) {
	reg_status = popStack();
	clearStatusFlag(STATUS_BRK);
	return 0;
}

int cpu::funcPushAccumulatorToStack(unsigned short src) {
	pushStack(reg_acc);
	return 0;
}

int cpu::funcPopAccumulatorFromStack(unsigned short src) {
	reg_acc = popStack();
	updateStatusZero(reg_acc);
	updateStatusSign(reg_acc);
	return 0;
}

int cpu::funcJumpSaveReturnAddress(unsigned short src) {
	unsigned short ret = reg_pc + 2;
	pushStack((ret >> 8) & 0xFF);
	pushStack(ret & 0xFF);
	reg_pc = src;
	return 0;
}

int cpu::funcJump(unsigned short src) {
	reg_pc = src;
	return 0;
}

int cpu::funcBit(unsigned short src) {
	unsigned short value = src;
	updateStatusOverflow(value >> 6);
	updateStatusSign(value >> 7);
	updateStatusZero(value & reg_acc);
	return 0;
}

int cpu::funcSetCarryFlag(unsigned short src) {
	setStatusFlag(STATUS_CARRY);
	return 0;
}

int cpu::funcSetDecimalMode(unsigned short src) {
	setStatusFlag(STATUS_DECIMAL);
	return 0;
}

int cpu::funcSetInterruptDisable(unsigned short src) {
	setStatusFlag(STATUS_INTERRUPT);
	return 0;
}

int cpu::funcClearCarryFlag(unsigned short src) {
	clearStatusFlag(STATUS_CARRY);
	return 0;
}

int cpu::funcClearDecimalMode(unsigned short src) {
	clearStatusFlag(STATUS_DECIMAL);
	return 0;
}

int cpu::funcClearInterruptDisable(unsigned short src) {
	clearStatusFlag(STATUS_INTERRUPT);
	return 0;
}

int cpu::funcClearOverflowFlag(unsigned short src) {
	clearStatusFlag(STATUS_OVERFLOW);
	return 0;
}

int cpu::funcTransferIndexXToStackPointer(unsigned short src) {
	reg_sp = reg_index_x;
	return 0; 
}

int cpu::funcTransferIndexXToAccumulator(unsigned short src) {
	reg_acc = reg_index_x;
	updateStatusZero(reg_index_x);
	updateStatusSign(reg_index_x);
	return 0; 
}

int cpu::funcTransferStackPointerToIndexX(unsigned short src) {
	reg_index_x = reg_sp;
	updateStatusZero(reg_index_x);
	updateStatusSign(reg_index_x);
	return 0; 
}

int cpu::funcTransferAccumulatorToIndexY(unsigned short src) {
	reg_index_y = reg_acc;
	updateStatusZero(reg_index_y);
	updateStatusSign(reg_index_y);
	return 0; 
}

int cpu::funcTransferAccumulatorToIndexX(unsigned short src) {
	reg_index_x = reg_acc;
	updateStatusZero(reg_index_x);
	updateStatusSign(reg_index_x);
	return 0; 
}

int cpu::funcTransferIndexYToAccumulator(unsigned short src) {
	reg_acc = reg_index_y;
	updateStatusZero(reg_index_y);
	updateStatusSign(reg_index_y);
	return 0; 
}

int cpu::funcLAX(unsigned short src) {
	funcLoadAccumulator(src);
	funcLoadRegisterX(); //function todo
	return 0;
}

int cpu::funcSAX(unsigned short src) {
	_memory -> write(src, reg_acc & reg_index_x);
	return 0;
}

int cpu::funcDCP(unsigned short src) {
	funcDecreaseMemory(); //function todo
	funcCompareMemory(src);
	return 0;
}

int cpu::funcISC(unsigned short src) {
	funcIncreaseMemory(); //function todo
	funcSBC();
	return 0;
}

int cpu::funcSLO(unsigned short src) {
	funcShiftLeftToMemory(src);
	funcOr(src);
	return 0;
}

int cpu::funcRLA(unsigned short src) {
	funcRotateLeftToMemory(src);
	funcAnd(src);
	return 0;
}

int cpu::funcSRE(unsigned short src) {
	funcShiftRightToMemory(src);
	funcXor(src);
	return 0;
}

int cpu::funcRRA(unsigned short src){
	funcRotateRightToMemory(src);
	funcADC(src);
	return 0;
}


int cpu::funcANC(unsigned short src) {
	funcAnd(src);
	reg_status &= ~STATUS_CARRY;

	if (result & STATUS_SIGN)
	{
		reg_status |= STATUS_CARRY;
	}
	return 0;
}

int cpu::funcALR(unsigned short src) {
	reg_acc &= src;
	updateStatusCarry(reg_acc);
	reg_acc >>= 1;
	updateStatusZero(reg_acc);
	clearStatusFlag(STATUS_SIGN);
	return 0;
}

int cpu::funcARR(unsigned short src) {
	reg_acc = ((reg_acc & src) >> 1) | (hasStatusFlag(STATUS_CARRY) << 7);
	updateStatusZero(reg_acc);
	updateStatusSign(reg_acc >> 7);
	updateStatusCarry(reg_acc >> 6);
	testAndSet(hasStatusFlag(STATUS_CARRY) ^ ((reg_acc >> 5) & 1), STATUS_OVERFLOW);//not sure about this one
	return 0;
}

int cpu::funcAXS(unsigned short src) {
	int result = (reg_acc & reg_index_x) - src;
	updateStatusCarry(result);
	updateStatusZero(result);
	reg_index_x = result & 0xFF;
	updateStatusSign(result >> 7);

	return 0;
}

int cpu::funcSHY(unsigned short src) {
	unsigned short result = (reg_index_y & ((src >> 8) + 1)) & 0xFF;
	unsigned short value = (src - reg_index_x) & 0xFF;

	if ((reg_index_x + value) <= 0xFF) {
		_memory->write(src, result);
	}
	else
	{
		_memory->write(src, src);
	}

	return 0;
}


int cpu::funcSHX(unsigned short src) {
	unsigned short result = (reg_index_x & ((src >> 8) + 1)) & 0xFF;
	unsigned short value = (src - reg_index_y) & 0xFF;

	if ((reg_index_y + value) <= 0xFF) {
		_memory->write(src, result);
	}
	else
	{
		_memory->write(src, src);
	}

	return 0;
}

unsigned short cpu::modeRelative(){
	signed short offset = (signed char)read(reg_pc + 1);
	address = reg_pc;
	result = address + offset;
	setPageBoundaryCrossed(address, result);
	return result;
}


int cpu::funcBranchResultNotZero(unsigned short src) {
	if (!hasStatusFlag(STATUS_ZERO))
	{
		branchTaken = true;
		reg_pc = modeRelative();
	}
	return 0;
}

int cpu::funcBranchResultZero(unsigned short src) {
	if (hasStatusFlag(STATUS_ZERO))
	{
		branchTaken = true;
		reg_pc = modeRelative();
	}
	return 0;
}

int cpu::funcBranchCarrySet(unsigned short src) {
	if (hasStatusFlag(STATUS_CARRY))
	{
		branchTaken = true;
		reg_pc = modeRelative();
	}
	return 0;
}

int cpu::funcBranchCarryClear(unsigned short src) {
	if (!hasStatusFlag(STATUS_CARRY))
	{
		branchTaken = true;
		reg_pc = modeRelative();
	}
	return 0;
}

int cpu::funcBranchResultMinus(unsigned short src) {
	if (hasStatusFlag(STATUS_SIGN))
	{
		branchTaken = true;
		reg_pc = modeRelative();
	}
	return 0;
}

int cpu::funcBranchResultPlus(unsigned short src) {
	if (!hasStatusFlag(STATUS_SIGN))
	{
		branchTaken = true;
		reg_pc = modeRelative();
	}
	return 0;
}

int cpu::funcBranchOverflowClear(unsigned short src) {
	if (!hasStatusFlag(STATUS_OVERFLOW))
	{
		branchTaken = true;
		reg_pc = modeRelative();
	}
	return 0;
}

int cpu::funcBranchOverflowSet(unsigned short src) {
	if (hasStatusFlag(STATUS_OVERFLOW))
	{
		branchTaken = true;
		reg_pc = modeRelative();
	}
	return 0;
}

int cpu::funcReturnFromSubroutine(unsigned short src) {
	unsigned char low = popStack();
	unsigned char high = popStack();
	reg_pc = ((high << 8) | low) + 1;
	return 0;
}

int cpu::funcReturnFromInterrupt(unsigned short src) {
	reg_status = popStack();
	unsigned char low = popStack() & 0xFF;
	unsigned char high = popStack() & 0xFF;
	reg_pc = (high << 8) | low;
	return 0;
}

int cpu::funcBreak(unsigned short src)
{
    executeInterrupt(Interrupt::BRK); 
	return 0;
}
