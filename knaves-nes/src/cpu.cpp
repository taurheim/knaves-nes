#include <iostream>

#include "cpu.h"
#include "instructions.h"

cpu::cpu() {
	//Set up opcodes
	instructions = {
		//Load into Accumulator Register
		{LDA_IMM, instruction { "LDA_IMM",&cpu::funcLoadAccumulator,Mode::IMMEDIATE,2,2,false,true } },
		{LDA_ZERO, instruction {"LDA_ZERO", &cpu::funcLoadAccumulator,Mode::ABSOLUTE_ZERO_PAGE,2,3,false,true}},
		{LDA_ZERO_X, instruction {"LDA_ZERO_X", &cpu::funcLoadAccumulator,Mode::ABSOLUTE_X_ZERO_PAGE,2,4,false,true}},
		{LDA_ABS, instruction {"LDA_ABS", &cpu::funcLoadAccumulator, Mode::ABSOLUTE, 3, 4, false, true}},
		{LDA_ABS_X, instruction {"LDA_ABS_X", &cpu::funcLoadAccumulator, Mode::ABSOLUTE_X, 3, 4, true, true}},
		{LDA_ABS_Y, instruction {"LDA_ABS_Y", &cpu::funcLoadAccumulator, Mode::ABSOLUTE_Y, 3, 4, true, true}},
		{LDA_IND_X, instruction {"LDA_IND_X", &cpu::funcLoadAccumulator, Mode::INDIRECT_X, 2, 6, false, true}},
		{LDA_IND_Y, instruction {"LDA_IND_Y", &cpu::funcLoadAccumulator, Mode::INDIRECT_Y, 2, 5, true, true}},

		//Store value from Accumulator Register
		{STA_ZERO,	instruction{"STA_ZERO", &cpu::funcStoreAccumulator, Mode::S_ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{STA_ZERO_X,instruction	{"STA_ZERO_X", &cpu::funcStoreAccumulator, Mode::S_ABSOLUTE_X_ZERO_PAGE, 2, 4, false, true}},
		{STA_ABS,	instruction{"STA_ABS2", &cpu::funcStoreAccumulator, Mode::S_ABSOLUTE, 3, 4, false, true}},
		{STA_ABS_X, instruction	{"STA_ABS_X", &cpu::funcStoreAccumulator, Mode::S_ABSOLUTE_X, 3, 5, false, true}},
		{STA_ABS_Y, instruction	{"STA_ABS_Y", &cpu::funcStoreAccumulator, Mode::S_ABSOLUTE_Y, 3, 5, false, true}},
		{STA_IND_X, instruction	{"STA_IND_X", &cpu::funcStoreAccumulator, Mode::S_INDIRECT_X, 2, 6, false, true}},
		{STA_IND_Y, instruction	{"STA_IND_Y", &cpu::funcStoreAccumulator, Mode::S_INDIRECT_Y, 2, 6, false, true}},

		//Branching
		{BPL, instruction {"BPL", &cpu::funcBranchOnResultPlus, Mode::RELATIVE, 2, 2, true, true}},
		{BMI, instruction {"BMI", &cpu::funcBranchOnResultMinus, Mode::RELATIVE, 2, 2, true, true}},
		{BVC, instruction {"BVC", &cpu::funcBranchOnOverflowClear, Mode::RELATIVE, 2, 2, true, true}},
		{BVS, instruction {"BVS", &cpu::funcBranchOnOverflowSet, Mode::RELATIVE, 2, 2, true, true}},
		{BCC, instruction {"BCC", &cpu::funcBranchOnCarryClear, Mode::RELATIVE, 2, 2, true, true}},
		{BCS, instruction {"BCS", &cpu::funcBranchOnCarrySet, Mode::RELATIVE, 2, 2, true, true}},
		{BNE, instruction {"BNE", &cpu::funcBranchOnResultNotZero, Mode::RELATIVE, 2, 2, true, true}},
		{BEQ, instruction {"BEQ", &cpu::funcBranchOnResultZero, Mode::RELATIVE, 2, 2, true, true}},

		//Load into X Register
		{LDX_IMM,	instruction{"LDX_IMM", &cpu::funcLoadRegisterX, Mode::IMMEDIATE, 2, 2, false, true}},
		{LDX_ZERO,	instruction{"LDX_ZERO", &cpu::funcLoadRegisterX, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{LDX_ZERO_Y,instruction	{"LDX_ZERO_Y", &cpu::funcLoadRegisterX, Mode::ABSOLUTE_Y_ZERO_PAGE, 2, 4, false, true}},
		{LDX_ABS, 	instruction{"LDX_ABS", &cpu::funcLoadRegisterX, Mode::ABSOLUTE, 3, 4, false, true}},
		{LDX_ABS_Y, instruction	{"LDX_ABS_Y", &cpu::funcLoadRegisterX, Mode::ABSOLUTE_Y, 3, 4, true, true}},

		//Store value from X Register
		{STX_ZERO,	instruction{"STX_ZERO", &cpu::funcStoreRegisterX, Mode::S_ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{STX_ZERO_Y,instruction	{"STX_ZERO_Y", &cpu::funcStoreRegisterX, Mode::S_ABSOLUTE_Y_ZERO_PAGE, 2, 4, false, true}},
		{STX_ABS, 	instruction{"STX_ABS", &cpu::funcStoreRegisterX, Mode::S_ABSOLUTE, 3, 4, false, true}},

		//Load into Y Register
		{LDY_IMM,	instruction{"LDY_IMM", &cpu::funcLoadRegisterY, Mode::IMMEDIATE, 2, 2, false, true}},
		{LDY_ZERO,	instruction{"LDY_ZERO", &cpu::funcLoadRegisterY, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{LDY_ZERO_X,instruction	{"LDY_ZERO_X", &cpu::funcLoadRegisterY, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 4, false, true}},
		{LDY_ABS, 	instruction{"LDY_ABS", &cpu::funcLoadRegisterY, Mode::ABSOLUTE, 3, 4, false, true}},
		{LDY_ABS_X, instruction	{"LDY_ABS_Y", &cpu::funcLoadRegisterY, Mode::ABSOLUTE_X, 3, 4, true, true}},

		//Store value from Y Register
		{STY_ZERO,	instruction{"STY_ZERO", &cpu::funcStoreRegisterY, Mode::S_ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{STY_ZERO_X,instruction	{"STY_ZERO_X", &cpu::funcStoreRegisterY, Mode::S_ABSOLUTE_X_ZERO_PAGE, 2, 4, false, true}},
		{STY_ABS, 	instruction{"STY_ABS", &cpu::funcStoreRegisterY, Mode::S_ABSOLUTE, 3, 4, false, true}},

		//Increment/Decrement X&Y
		{INX,		instruction{"INX", &cpu::funcIncreaseRegisterX, Mode::IMPLIED, 1, 2, false, true}},
		{DEX,		instruction{"DEX", &cpu::funcDecreaseRegisterX, Mode::IMPLIED, 1, 2, false, true}},
		{INY,		instruction{"INY", &cpu::funcIncreaseRegisterY, Mode::IMPLIED, 1, 2, false, true}},
		{DEY,		instruction{"DEY", &cpu::funcDecreaseRegisterY, Mode::IMPLIED, 1, 2, false, true}},

		//Increment value at memory location
		{INC_ZERO,	instruction{"INC_ZERO", &cpu::funcIncreaseMemory, Mode::ABSOLUTE_ZERO_PAGE, 2, 5, false, true}},
		{INC_ZERO_X,instruction	{"INC_ZERO_X", &cpu::funcIncreaseMemory, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 6, false, true}},
		{INC_ABS, 	instruction{"INC_ABS", &cpu::funcIncreaseMemory, Mode::ABSOLUTE, 3, 6, false, true}},
		{INC_ABS_X, instruction	{"INC_ABS_X", &cpu::funcIncreaseMemory, Mode::ABSOLUTE_X, 3, 7, false, true}},

		//
		{DEC_ZERO,	instruction{"DEC_ZERO", &cpu::funcDecreaseMemory, Mode::ABSOLUTE_ZERO_PAGE, 2, 5, false, true}},
		{DEC_ZERO_X,instruction	{"DEC_ZERO_X", &cpu::funcDecreaseMemory, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 6, false, true}},
		{DEC_ABS, 	instruction{"DEC_ABS", &cpu::funcDecreaseMemory, Mode::ABSOLUTE, 3, 6, false, true}},
		{DEC_ABS_X, instruction	{"DEC_ABS_X", &cpu::funcDecreaseMemory, Mode::ABSOLUTE_X, 3, 7, false, true}},

		{CPX_IMM,	instruction{"CPX_IMM", &cpu::funcCompareRegisterX, Mode::IMMEDIATE, 2, 2, false, true}},
		{CPX_ZERO,	instruction{"CPX_ZERO", &cpu::funcCompareRegisterX, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{CPX_ABS,	instruction{"CPX_ABS", &cpu::funcCompareRegisterX, Mode::ABSOLUTE, 3, 4, false, true}},

		{CPY_IMM,	instruction{"CPY_IMM", &cpu::funcCompareRegisterY, Mode::IMMEDIATE, 2, 2, false, true}},
		{CPY_ZERO,	instruction{"CPY_ZERO", &cpu::funcCompareRegisterY, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{CPY_ABS,	instruction{"CPY_ABS", &cpu::funcCompareRegisterY, Mode::ABSOLUTE, 3, 4, false, true}},

		{CMP_IMM,	instruction{"CMP_IMM", &cpu::funcCompareMemory, Mode::IMMEDIATE, 2, 2, false, true}},
		{CMP_ZERO,	instruction{"CMP_ZERO", &cpu::funcCompareMemory, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{CMP_ZERO_X,instruction	{"CMP_ZERO_X", &cpu::funcCompareMemory, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 4, false, true}},
		{CMP_ABS, 	instruction{"CMP_ABS", &cpu::funcCompareMemory, Mode::ABSOLUTE, 3, 4, false, true}},
		{CMP_ABS_X, instruction	{"CMP_ABS_X", &cpu::funcCompareMemory, Mode::ABSOLUTE_X, 3, 4, true, true}},
		{CMP_ABS_Y, instruction	{"CMP_ABS_Y", &cpu::funcCompareMemory, Mode::ABSOLUTE_Y, 3, 4, true, true}},
		{CMP_IND_X, instruction	{"CMP_IND_X", &cpu::funcCompareMemory, Mode::INDIRECT_X, 2, 6, false, true}},
		{CMP_IND_Y, instruction	{"CMP_IND_Y", &cpu::funcCompareMemory, Mode::INDIRECT_Y, 2, 5, true, true}},



		{AND_IMM, instruction { "AND_IMM", &cpu::funcAnd, Mode::IMMEDIATE, 2, 2, false, true} },
		{AND_ZERO, instruction { "AND_ZERO", &cpu::funcAnd, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true } },
		{AND_ZERO_X, instruction { "AND_ZERO_X", &cpu::funcAnd, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 4, false, true } },
		{AND_ABS, instruction { "AND_ABS", &cpu::funcAnd, Mode::ABSOLUTE, 3, 4, false, true } },
		{AND_ABS_X,  instruction { "AND_ABS_X", &cpu::funcAnd,Mode::ABSOLUTE_X, 3, 4, true, true } },
		{AND_ABS_Y, instruction { "AND_ABS_Y", &cpu::funcAnd,Mode::ABSOLUTE_Y, 3, 4, true, true } },
		{AND_IND_X, instruction { "AND_IND_X", &cpu::funcAnd, Mode::INDIRECT_X, 2, 6, false, true } },
		{AND_IND_Y, instruction { "AND_IND_Y", &cpu::funcAnd, Mode::INDIRECT_Y, 2, 5, true, true } },


		{OR_IMM, instruction {"OR_IMM", &cpu::funcOr, Mode::IMMEDIATE, 2, 2, false, true}},
		{OR_ZERO, instruction {"OR_ZERO", &cpu::funcOr, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{OR_ZERO_X, instruction {"OR_ZERO_X", &cpu::funcOr, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 4, false, true}},
		{OR_ABS,  instruction {"OR_ABS", &cpu::funcOr, Mode::ABSOLUTE, 3, 4, false, true}},
		{OR_ABS_X,  instruction {"OR_ABS_X", &cpu::funcOr, Mode::ABSOLUTE_X, 3, 4, true, true}},
		{OR_ABS_Y,  instruction {"OR_ABS_Y", &cpu::funcOr, Mode::ABSOLUTE_Y, 3, 4, true, true}},
		{OR_IND_X,  instruction {"OR_IND_X", &cpu::funcOr, Mode::INDIRECT_X, 2, 6, false, true}},
		{OR_IND_Y,  instruction {"OR_IND_Y", &cpu::funcOr, Mode::INDIRECT_Y, 2, 5, true, true}},

		{XOR_IMM,	 instruction {"XOR_IMM", &cpu::funcXor, Mode::IMMEDIATE, 2, 2, false, true}},
		{XOR_ZERO,	 instruction {"XOR_ZERO", &cpu::funcXor, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{XOR_ZERO_X, instruction {"XOR_ZERO_X", &cpu::funcXor, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 4, false, true}},
		{XOR_ABS, 	 instruction {"XOR_ABS", &cpu::funcXor, Mode::ABSOLUTE, 3, 4, false, true}},
		{XOR_ABS_X,  instruction {"XOR_ABS_X", &cpu::funcXor, Mode::ABSOLUTE_X, 3, 4, true, true}},
		{XOR_ABS_Y,  instruction {"XOR_ABS_Y", &cpu::funcXor, Mode::ABSOLUTE_Y, 3, 4, true, true}},
		{XOR_IND_X,  instruction {"XOR_IND_X", &cpu::funcXor, Mode::INDIRECT_X, 2, 6, false, true}},
		{XOR_IND_Y,  instruction {"XOR_IND_Y", &cpu::funcXor, Mode::INDIRECT_Y, 2, 5, true, true}},

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
		{ADC_IND_X, instruction	{"ADC_IND_X", &cpu::funcADC, Mode::INDIRECT_X, 2, 6, false, true}},
		{ADC_IND_Y, instruction	{"ADC_IND_Y", &cpu::funcADC, Mode::INDIRECT_Y, 2, 5, true, true}},

		{SBC_IMM, 	instruction {"SBC_IMM", &cpu::funcSBC, Mode::IMMEDIATE, 2, 2, false, true}},
		{SBC_IMM2,	instruction {"SBC_IMM2", &cpu::funcSBC, Mode::IMMEDIATE, 2, 2, false, true}},
		{SBC_ZERO, 	instruction	{"SBC_ZERO", &cpu::funcSBC, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{SBC_ZERO_X, instruction{"SBC_ZERO_X", &cpu::funcSBC, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 4, false, true}},
		{SBC_ABS, 	instruction {"SBC_ABS", &cpu::funcSBC, Mode::ABSOLUTE, 3, 4, false, true}},
		{SBC_ABS_X, instruction	{"SBC_ABS_X", &cpu::funcSBC, Mode::ABSOLUTE_X, 3, 4, true, true}},
		{SBC_ABS_Y, instruction	{"SBC_ABS_Y", &cpu::funcSBC, Mode::ABSOLUTE_Y, 3, 4, true, true}},
		{SBC_IND_X, instruction	{"SBC_IND_X", &cpu::funcSBC, Mode::INDIRECT_X, 2, 6, false, true}},
		{SBC_IND_Y, instruction	{"SBC_IND_Y", &cpu::funcSBC, Mode::INDIRECT_Y, 2, 5, true, true}},

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

		{LAX_IND_X,	instruction {"LAX_IND_X", &cpu::funcLAX, Mode::INDIRECT_X, 2, 6, false, true}},
		{LAX_ZERO,	instruction {"LAX_ZERO", &cpu::funcLAX, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{LAX_IMM,	instruction {"LAX_IMM", &cpu::funcLAX, Mode::IMMEDIATE, 2, 2, false, true}},
		{LAX_ABS,	instruction {"LAX_ABS", &cpu::funcLAX, Mode::ABSOLUTE, 3, 4, false, true}},
		{LAX_IND_Y,	instruction {"LAX_IND_Y", &cpu::funcLAX, Mode::INDIRECT_Y, 2, 5, true, true}},
		{LAX_ZERO_Y,instruction {"LAX_ZERO_Y", &cpu::funcLAX, Mode::ABSOLUTE_Y_ZERO_PAGE, 2, 4, false, true}},
		{LAX_ABS_Y,	instruction {"LAX_ABS_Y", &cpu::funcLAX, Mode::ABSOLUTE_Y, 3, 4, true, true}},

		{SAX_IND_X,	instruction {"SAX_IND_X", &cpu::funcSAX, Mode::INDIRECT_X, 2, 6, false, true}},
		{SAX_ZERO,	instruction {"SAX_ZERO", &cpu::funcSAX, Mode::ABSOLUTE_ZERO_PAGE, 2, 3, false, true}},
		{SAX_ABS,	instruction {"SAX_ABS", &cpu::funcSAX, Mode::ABSOLUTE, 3, 4, false, true}},
		{SAX_ZERO_Y,instruction {"SAX_ZERO_Y", &cpu::funcSAX, Mode::ABSOLUTE_Y_ZERO_PAGE, 2, 4, false, true}},

		{DCP_IND_X,	instruction {"DCP_IND_X", &cpu::funcDCP, Mode::INDIRECT_X, 2, 8, false, true}},
		{DCP_ZERO,	instruction {"DCP_ZERO", &cpu::funcDCP, Mode::ABSOLUTE_ZERO_PAGE, 2, 5, false, true}},
		{DCP_ABS,	instruction {"DCP_ABS", &cpu::funcDCP, Mode::ABSOLUTE, 3, 6, false, true}},
		{DCP_IND_Y,	instruction {"DCP_IND_Y", &cpu::funcDCP, Mode::INDIRECT_Y, 2, 8, false, true}},
		{DCP_ZERO_X,instruction {"DCP_ZERO_X", &cpu::funcDCP, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 6, false, true}},
		{DCP_ABS_Y,	instruction {"DCP_ABS_Y", &cpu::funcDCP, Mode::ABSOLUTE_Y, 3, 7, false, true}},
		{DCP_ABS_X,	instruction {"DCP_ABS_X", &cpu::funcDCP, Mode::ABSOLUTE_X, 3, 7, false, true}},

		{ISC_IND_X,	instruction {"ISC_IND_X", &cpu::funcISC, Mode::INDIRECT_X, 2, 8, false, true}},
		{ISC_ZERO,	instruction {"ISC_ZERO", &cpu::funcISC, Mode::ABSOLUTE_ZERO_PAGE, 2, 5, false, true}},
		{ISC_ABS,	instruction {"ISC_ABS", &cpu::funcISC, Mode::ABSOLUTE, 3, 6, false, true}},
		{ISC_IND_Y,	instruction {"ISC_IND_Y", &cpu::funcISC, Mode::INDIRECT_Y, 2, 8, false, true}},
		{ISC_ZERO_X,instruction {"ISC_ZERO_X", &cpu::funcISC, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 6, false, true}},
		{ISC_ABS_Y,	instruction {"ISC_ABS_Y", &cpu::funcISC, Mode::ABSOLUTE_Y, 3, 7, false, true}},
		{ISC_ABS_X,	instruction {"ISC_ABS_X", &cpu::funcISC, Mode::ABSOLUTE_X, 3, 7, false, true}},

		{SLO_IND_X,	instruction {"SLO_IND_X", &cpu::funcSLO, Mode::INDIRECT_X, 2, 8, false, true}},
		{SLO_ZERO,	instruction {"SLO_ZERO", &cpu::funcSLO, Mode::ABSOLUTE_ZERO_PAGE, 2, 5, false, true}},
		{SLO_ABS,	instruction {"SLO_ABS", &cpu::funcSLO, Mode::ABSOLUTE, 3, 6, false, true}},
		{SLO_IND_Y,	instruction {"SLO_IND_Y", &cpu::funcSLO, Mode::INDIRECT_Y, 2, 8, false, true}},
		{SLO_ZERO_X,instruction {"SLO_ZERO_X", &cpu::funcSLO, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 6, false, true}},
		{SLO_ABS_Y,	instruction {"SLO_ABS_Y", &cpu::funcSLO, Mode::ABSOLUTE_Y, 3, 7, false, true}},
		{SLO_ABS_X,	instruction {"SLO_ABS_X", &cpu::funcSLO, Mode::ABSOLUTE_X, 3, 7, false, true}},

		{RLA_IND_X,	instruction {"RLA_IND_X", &cpu::funcRLA, Mode::INDIRECT_X, 2, 8, false, true}},
		{RLA_ZERO,	instruction {"RLA_ZERO", &cpu::funcRLA, Mode::ABSOLUTE_ZERO_PAGE, 2, 5, false, true}},
		{RLA_ABS,	instruction {"RLA_ABS", &cpu::funcRLA, Mode::ABSOLUTE, 3, 6, false, true}},
		{RLA_IND_Y,	instruction {"RLA_IND_Y", &cpu::funcRLA, Mode::INDIRECT_Y, 2, 8, false, true}},
		{RLA_ZERO_X,instruction {"RLA_ZERO_X", &cpu::funcRLA, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 6, false, true}},
		{RLA_ABS_Y,	instruction {"RLA_ABS_Y", &cpu::funcRLA, Mode::ABSOLUTE_Y, 3, 7, false, true}},
		{RLA_ABS_X,	instruction {"RLA_ABS_X", &cpu::funcRLA, Mode::ABSOLUTE_X, 3, 7, false, true}},

		{SRE_IND_X,	instruction {"SRE_IND_X", &cpu::funcSRE, Mode::INDIRECT_X, 2, 8, false, true}},
		{SRE_ZERO,	instruction {"SRE_ZERO", &cpu::funcSRE, Mode::ABSOLUTE_ZERO_PAGE, 2, 5, false, true}},
		{SRE_ABS,	instruction {"SRE_ABS", &cpu::funcSRE, Mode::ABSOLUTE, 3, 6, false, true}},
		{SRE_IND_Y,	instruction {"SRE_IND_Y", &cpu::funcSRE, Mode::INDIRECT_Y, 2, 8, false, true}},
		{SRE_ZERO_X,instruction {"SRE_ZERO_X", &cpu::funcSRE, Mode::ABSOLUTE_X_ZERO_PAGE, 2, 6, false, true}},
		{SRE_ABS_Y,	instruction {"SRE_ABS_Y", &cpu::funcSRE, Mode::ABSOLUTE_Y, 3, 7, false, true}},
		{SRE_ABS_X,	instruction {"SRE_ABS_X", &cpu::funcSRE, Mode::ABSOLUTE_X, 3, 7, false, true}},

		{RRA_IND_X,	instruction {"RRA_IND_X", &cpu::funcRRA, Mode::INDIRECT_X, 2, 8, false, true}},
		{RRA_ZERO,	instruction {"RRA_ZERO", &cpu::funcRRA, Mode::ABSOLUTE_ZERO_PAGE, 2, 5, false, true}},
		{RRA_ABS,	instruction {"RRA_ABS", &cpu::funcRRA, Mode::ABSOLUTE, 3, 6, false, true}},
		{RRA_IND_Y,	instruction {"RRA_IND_Y", &cpu::funcRRA, Mode::INDIRECT_Y, 2, 8, false, true}},
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

		{RTS, 		instruction {"RTS", &cpu::funcReturnFromSubroutine, Mode::IMPLIED, 1, 6, false, false}},
		{RTI, 		instruction {"RTI", &cpu::funcReturnFromInterrupt, Mode::IMPLIED, 1, 6, false, false}},
		//{BRK, 		instruction {"BRK", &cpu::funcBreak, Mode::IMPLIED, 1, 7, false, false}},

		{NOP, 	instruction {"NOP", &cpu::funcNop, Mode::IMPLIED, 1, 2, false, true}},
		{NOP2, 	instruction {"NOP", &cpu::funcNop, Mode::IMPLIED, 2, 3, false, true}},
		{NOP3, 	instruction {"NOP", &cpu::funcNop, Mode::IMPLIED, 2, 3, false, true}},
		{NOP4, 	instruction {"NOP", &cpu::funcNop, Mode::IMPLIED, 2, 3, false, true}},
		{NOP5, 	instruction {"NOP", &cpu::funcNop, Mode::IMPLIED, 1, 2, false, true}},
		{NOP6, 	instruction {"NOP", &cpu::funcNop, Mode::IMPLIED, 1, 2, false, true}},
		{NOP7, 	instruction {"NOP", &cpu::funcNop, Mode::IMPLIED, 1, 2, false, true}},
		{NOP8, 	instruction {"NOP", &cpu::funcNop, Mode::IMPLIED, 1, 2, false, true}},
		{NOP9, 	instruction {"NOP", &cpu::funcNop, Mode::IMPLIED, 1, 2, false, true}},
		{NOP10, instruction {"NOP", &cpu::funcNop, Mode::IMPLIED, 1, 2, false, true}},
		{NOP11, instruction {"NOP", &cpu::funcNop, Mode::IMPLIED, 3, 4, false, true}},
		{NOP12, instruction {"NOP", &cpu::funcNop, Mode::IMPLIED, 2, 4, false, true}},
		{NOP13, instruction {"NOP", &cpu::funcNop, Mode::IMPLIED, 2, 4, false, true}},
		{NOP14, instruction {"NOP", &cpu::funcNop, Mode::IMPLIED, 2, 4, false, true}},
		{NOP15, instruction {"NOP", &cpu::funcNop, Mode::IMPLIED, 2, 4, false, true}},
		{NOP16, instruction {"NOP", &cpu::funcNop, Mode::IMPLIED, 2, 4, false, true}},
		{NOP17, instruction {"NOP", &cpu::funcNop, Mode::IMPLIED, 2, 4, false, true}},
		{NOP18, instruction {"NOP", &cpu::funcNop, Mode::IMPLIED, 2, 2, false, true}},
		{NOP19, instruction {"NOP", &cpu::funcNop, Mode::ABSOLUTE_X, 3, 4, true, true}},
		{NOP20, instruction {"NOP", &cpu::funcNop, Mode::ABSOLUTE_X, 3, 4, true, true}},
		{NOP21, instruction {"NOP", &cpu::funcNop, Mode::ABSOLUTE_X, 3, 4, true, true}},
		{NOP22, instruction {"NOP", &cpu::funcNop, Mode::ABSOLUTE_X, 3, 4, true, true}},
		{NOP23, instruction {"NOP", &cpu::funcNop, Mode::ABSOLUTE_X, 3, 4, true, true}},
		{NOP24, instruction {"NOP", &cpu::funcNop, Mode::ABSOLUTE_X, 3, 4, true, true}},
		{NOP25, instruction {"NOP", &cpu::funcNop, Mode::ABSOLUTE_X, 2, 2, false, true}},
		{NOP26, instruction {"NOP", &cpu::funcNop, Mode::ABSOLUTE_X, 2, 2, false, true}},
		{NOP27, instruction {"NOP", &cpu::funcNop, Mode::ABSOLUTE_X, 2, 2, false, true}},
		{NOP28, instruction {"NOP", &cpu::funcNop, Mode::ABSOLUTE_X, 2, 2, false, true}},

		{KIL1, instruction {"KIL", &cpu::funcKill, Mode::IMMEDIATE,0,0,false,false}}
	};
}

void cpu::init(Memory * memory, bool show_log) {
	_memory = memory;
	log_instructions = show_log;
}

void cpu::start() {
	is_running = true;

	//Set Registers on startup

	//OVERRIDING, FIX THIS LATER
	reg_pc = 0x8010;
	reg_status = STARTUP_STATUS;
	reg_sp = STARTUP_STACK;
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

	return false;
}

/*
 Execute an interrupt
 @param enum interrupt	The interrupt to execute
 @return short			New program counter (this is the location of the instructions executed upon interrupt)
*/
void cpu::executeInterrupt(const enum Interrupt &interrupt) {
	//Push program counter & status register to stack
	pushStackDouble(reg_pc);
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
	case Interrupt::BREAK:
		reg_pc = IRQ_BRK_VECTOR;
		break;
	case Interrupt::RESET:
		reg_pc = (_memory->read(INTERRUPT_RESET_VECTOR_HIGH) << 8) | _memory->read(INTERRUPT_RESET_VECTOR_LOW);
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
	if (cpu::checkInterrupts()) {
		//If an interrupt ran, it took 7 cycles
		return INTERRUPT_CYCLES;
	}

	//Fetch the opcode
	unsigned char opcode = readAddress(reg_pc);

	//Get the instruction associated with the opcode
	std::map<char, instruction>::const_iterator instruction_row = instructions.find(opcode);

	//Invalid opcode
	if (instruction_row == instructions.end()) {
		if(log_instructions) std::cout << "\nInvalid Instruction: " << std::hex << (int)opcode;
		//_memory->logMemory();
		//throw InvalidOpcodeException(current_opcode);
		return 0;
	}


	//Actually execute the opcode instruction
	instruction current_instruction = instruction_row->second;
	if (log_instructions) std::cout << "\n[" << std::hex << reg_pc << "] Executing instruction " + current_instruction.name + "\t\t(0x" << std::hex << (int) opcode << " ";

	//Get the source 
	unsigned int src = getSource(current_instruction.mode);
	if (log_instructions) std::cout << "0x" << std::hex << (int)src;

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
		case Mode::IMMEDIATE: {
			//Return whatever is beside the instruction in memory
			return _memory->read(reg_pc + 1);
			break;
		}

		case Mode::IMPLIED: {
			//This means that there isn't actually an argument (it's implied with the instruction)
			//e.g. DEX (decrement X register)
			return 0;
			break;
		}

		case Mode::ABSOLUTE: {
			//Load from a 16bit memory address
			//The next 2 bytes of memory define a pointer. Our argument is the value being pointed to
			unsigned char first = _memory->read(reg_pc + 2);
			unsigned char second = _memory->read(reg_pc + 1);
			return _memory->read(first << 8 | second);
			break;
		}

		case Mode::ABSOLUTE_X: {
			//First get the pointer as if it was absolute, then add the x register's value to that pointer
			//Find the value at our new pointer
			unsigned short absolute_address = getSource(Mode::ABSOLUTE);
			return _memory->read(absolute_address + reg_index_x);
			break;
		}

		case Mode::ABSOLUTE_Y: {
			//First get the pointer as if it was absolute, then add the y register's value to that pointer
			//Find the value at our new pointer
			unsigned short absolute_address = getSource(Mode::ABSOLUTE);
			return _memory->read(absolute_address + reg_index_y);
			break;
		}

		case ABSOLUTE_ZERO_PAGE: {
			//Same thing as absolute, except it's always going to be in the zero page (first 1 byte of memory values)
			unsigned short absolute_address = _memory->read(reg_pc + 1);
			return _memory->read(absolute_address & 0xff);
			break;
		}

		case ABSOLUTE_X_ZERO_PAGE: {
			//Same thing as ABSOLUTE_ZERO_PAGE, except before making it zero page, we also add the x register
			unsigned short absolute_address = getSource(Mode::ABSOLUTE);
			return _memory->read((absolute_address + reg_index_x) & 0xff);
			break;
		}

		case ABSOLUTE_Y_ZERO_PAGE: {
			//Same thing as ABSOLUTE_ZERO_PAGE, except before making it zero page, we also add the y register
			unsigned short absolute_address = getSource(Mode::ABSOLUTE);
			return _memory->read((absolute_address + reg_index_y) & 0xff);
			break;
		}

		case INDIRECT: {
			//Used for JMP.
			//The memory value specified in the next 2 bytes points to the low byte of the value
			//The byte after the low byte is the high byte
			unsigned short absolute_address = getSource(Mode::ABSOLUTE);
			unsigned short second = _memory->read(absolute_address);
			unsigned short first = _memory->read(absolute_address + 1);
			return (first << 8 | second);
		}

		case INDIRECT_X: {
			//val = PEEK(PEEK((arg + X) % 256) + PEEK((arg + X + 1) % 256) * 256)
			unsigned short reference_1 = (_memory->read(reg_pc + 1) + reg_index_x) & 0xff;
			unsigned short reference_2 = (reference_1 + 1) & 0xff;
			unsigned short second = _memory->read(reference_1);
			unsigned short first = _memory->read(reference_2);
			return (first << 8 | second);
		}

		case INDIRECT_Y: {
			//val = PEEK(PEEK(arg) + PEEK((arg + 1) % 256) + y)
			unsigned short reference_1 = _memory->read(reg_pc + 1) & 0xff;
			unsigned short reference_2 = _memory->read(reference_1);
			return _memory->read(reference_2 + reg_index_y);
		}

		case Mode::RELATIVE: {
			//Used for branching instructions. Essentially the byte
			//after the instruction tells the CPU how many bytes to skip if
			//the branch happens
			return _memory->read(reg_pc + 1);
			break;
		}

		case Mode::S_ABSOLUTE: {
			//Store into value
			unsigned char first = _memory->read(reg_pc + 2);
			unsigned char second = _memory->read(reg_pc + 1);
			return (first << 8 | second);
		}

		case Mode::S_ABSOLUTE_ZERO_PAGE: {
			unsigned short absolute_address = getSource(Mode::S_ABSOLUTE);
			return absolute_address & 0xff;
		}

		case Mode::S_INDIRECT_Y: {
			unsigned short reference_1 = _memory->read(reg_pc + 1) & 0xff;
			unsigned short reference_2 = _memory->read(reference_1);
			return reference_2 + reg_index_y;
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
	return _memory->read(address);
}

/*
 Push a byte to the stack
*/
void cpu::pushStack(unsigned char byte) {
	//Decrement stack pointer because stack is stored top-down
	reg_sp--;
	_memory->write(STACK_START + reg_sp, byte);
}

/*
 Push 16 bits to the stack (2 bytes)
*/

void cpu::pushStackDouble(unsigned short bytebyte) {
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
	unsigned char item = _memory->read(STACK_START + reg_sp);
	reg_sp++;
	return item;
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
void cpu::setStatusFlag(unsigned int flag) {
	reg_status |= flag;
}

/*
 Remove a status flag from the cpu status register
*/
void cpu::clearStatusFlag(unsigned int flag) {
	reg_status &= ~flag;
}

/*
 If necessary, set the Sign status register
 @param val		8 bit value
*/
void cpu::updateStatusSign(unsigned short val) {
	// & 128 to figure out if it's positive or negative (check the first bit)
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

void cpu::updateStatusBasedOnExpression(bool result, unsigned char flag) {
	if (result) {
		setStatusFlag(flag);
	}
	else {
		clearStatusFlag(flag);
	}
}

/************
  OPERATIONS
*************/

//Load a value from an address to the accumulator
int cpu::funcLoadAccumulator(unsigned short src) {
	if (log_instructions) std::cout << " Loaded " << std::hex << src;
	reg_acc = src;
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

int cpu::funcLoadRegisterX(unsigned short src)
{
	unsigned short value = src;
	reg_index_x = value;
	updateStatusSign(reg_index_x);
	updateStatusZero(reg_index_x);
	return 0;
}

int cpu::funcLoadRegisterY(unsigned short src)
{
	unsigned short value = src;
	reg_index_y = value;
	updateStatusSign(reg_index_y);
	updateStatusZero(reg_index_y);
	return 0;
}


int cpu::funcStoreRegisterX(unsigned short src) {
	if (log_instructions) std::cout << "Storing " << std::hex << (int) reg_index_x;
	_memory->write(src, reg_index_x);
	return 0;
}

int cpu::funcStoreRegisterY(unsigned short src) {
	_memory->write(src, reg_index_y);
	return 0;
}

int cpu::funcIncreaseRegisterX(unsigned short src)
{
	reg_index_x++;
	updateStatusSign(reg_index_x);
	updateStatusZero(reg_index_x);
	return 0;
}

int cpu::funcIncreaseRegisterY(unsigned short src)
{
	reg_index_y++;
	updateStatusSign(reg_index_y);
	updateStatusZero(reg_index_y);
	return 0;
}

int cpu::funcDecreaseRegisterX(unsigned short src)
{
	reg_index_x--;
	updateStatusZero(reg_index_x);
	updateStatusSign(reg_index_x);
	return 0;
}

int cpu::funcDecreaseRegisterY(unsigned short src)
{
	reg_index_y--;
	updateStatusZero(reg_index_y);
	updateStatusSign(reg_index_y);
	return 0;
}

int cpu::funcIncreaseMemory(unsigned short src)
{
	_memory->write(src, src + 1);
	updateStatusSign(src);
	updateStatusZero(src);
	return 0;
}

int cpu::funcDecreaseMemory(unsigned short src)
{
	_memory->write(src, src - 1);
	updateStatusZero(src);
	updateStatusSign(src);
	return 0;
}

int cpu::funcCompareRegisterX(unsigned short src)
{
	unsigned short value = src;
	unsigned short result = reg_index_x - value;
	updateStatusBasedOnExpression(reg_index_x >= (value & 0xFF), STATUS_CARRY); //to be changed
	updateStatusBasedOnExpression(reg_index_x == (value & 0xFF), STATUS_ZERO);
	updateStatusSign(result);
	return 0;
}

int cpu::funcCompareRegisterY(unsigned short src)
{
	unsigned short value = src;
	unsigned short result = reg_index_y - value;
	updateStatusBasedOnExpression(reg_index_y >= (value & 0xFF), STATUS_CARRY); //to be changed
	updateStatusBasedOnExpression(reg_index_y == (value & 0xFF), STATUS_ZERO);
	updateStatusSign(result);
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
	unsigned short result = reg_acc - src;
	if (log_instructions) std::cout << " Comparing " << std::hex << (int) reg_acc << " and " << src;

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
	updateStatusBasedOnExpression(~(reg_acc ^ value) & (reg_acc ^ result) & 0x80, STATUS_OVERFLOW);
	updateStatusCarry(result);
	reg_acc = result & 0xFF;
	updateStatusZero(reg_acc);
	updateStatusSign(reg_acc);
	return 0;
}

int cpu::funcSBC(unsigned short src) {
	unsigned short value = src;
	unsigned short result = reg_acc + ~value + (hasStatusFlag(STATUS_CARRY) ? 1 : 0);
	updateStatusBasedOnExpression((reg_acc ^ value) & (reg_acc ^ result) & 0x80, STATUS_OVERFLOW); 
	updateStatusBasedOnExpression(!(result & 0x100), STATUS_CARRY);
	reg_acc = result & 0xFF;
	updateStatusZero(reg_acc);
	updateStatusSign(reg_acc);
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
	if (log_instructions) std::cout << " Pushing to stack: " << std::hex << (int) reg_acc;
	pushStack(reg_acc);
	return 0;
}

int cpu::funcPopAccumulatorFromStack(unsigned short src) {
	reg_acc = popStack();
	if (log_instructions) std::cout << " Popped " << std::hex << (int) reg_acc;
	updateStatusZero(reg_acc);
	updateStatusSign(reg_acc);
	return 0;
}

int cpu::funcJumpSaveReturnAddress(unsigned short src) {
	if (log_instructions) std::cout << " Jumping to " << src;
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
	updateStatusBasedOnExpression(value >> 6,STATUS_OVERFLOW);
	updateStatusSign(value);
	updateStatusZero(value);
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
	if (log_instructions) std::cout << " Transferring " << (int) reg_acc << " to reg_x";
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
	funcLoadRegisterX(src); 
	return 0;
}

int cpu::funcSAX(unsigned short src) {
	_memory -> write(src, reg_acc & reg_index_x);
	return 0;
}

int cpu::funcDCP(unsigned short src) {
	funcDecreaseMemory(src); 
	funcCompareMemory(src);
	return 0;
}

int cpu::funcISC(unsigned short src) {
	funcIncreaseMemory(src);
	funcSBC(src);
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

	if (reg_acc & STATUS_SIGN)
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
	updateStatusSign(reg_acc);
	updateStatusCarry(reg_acc >> 6);
	updateStatusBasedOnExpression(hasStatusFlag(STATUS_CARRY) ^ ((reg_acc >> 5) & 1), STATUS_OVERFLOW);
	return 0;
}

int cpu::funcAXS(unsigned short src) {
	int result = (reg_acc & reg_index_x) - src;
	updateStatusCarry(result);
	updateStatusZero(result);
	reg_index_x = result & 0xFF;
	updateStatusSign(result);

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

int cpu::funcReturnFromSubroutine(unsigned short src) {
	unsigned char low = popStack();
	unsigned char high = popStack();
	reg_pc = ((high << 8) | low) + 1;
	if (log_instructions) std::cout << "\nReturning From Subroutine to: " << reg_pc;
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
    executeInterrupt(Interrupt::BREAK); 
	return 0;
}

int cpu::funcNop(unsigned short src)
{
	_memory->logMemory("mid");
	return 0;
}

int cpu::funcKill(unsigned short src)
{
	is_running = false;
	return 0;
}
