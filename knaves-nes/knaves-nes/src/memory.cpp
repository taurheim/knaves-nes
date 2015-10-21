#include "memory.h"

/*
 Write a value to the RAM
*/
void memory::write(unsigned short address, unsigned char value) {
	memory::RAM[address] = value;
	return;
}

/*
 Read value from RAM
*/
unsigned char memory::read(unsigned short address) {

	if (address >= ROM_LOWER_ADDRESS && address <= ROM_UPPER_ADDRESS) {
		//Read from ROM
	}

	//Check for special cases (registers)
	switch (address) {
	case PPU_STATUS:
		break;
	case PPU_OAM_DATA:
		break;
	case PPU_VRAM_DATA:
		break;
	case CONTROLLER_1:
		break;
	case CONTROLLER_2:
		break;
	default:
		return memory::RAM[address];
	}
}