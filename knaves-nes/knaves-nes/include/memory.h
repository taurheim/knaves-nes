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




class memory {
public: 

	memory();

	static void write(unsigned short address, unsigned char value);
	static unsigned char read(unsigned short address);
	static void logMemory();
	static unsigned char * RAM;

private:
	static std::string hexFromLong(long i);
};

#endif