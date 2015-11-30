#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "cpu.h"

#define PPU_STATUS			0x2002
#define PPU_OAM_DATA		0x2004
#define PPU_VRAM_DATA		0x2007
#define CONTROLLER_1		0x4016
#define CONTROLLER_2		0x4017

#define ROM_LOWER_ADDRESS	0x8000
#define ROM_UPPER_ADDRESS	0xFFFF




class Memory {
public: 

	Memory();
	void write(unsigned short address, unsigned char value);
	unsigned char read(unsigned short address);
	void logMemory();

private:
	static std::string hexFromLong(long i);
	unsigned char * RAM;
};

#endif