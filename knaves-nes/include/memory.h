#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "cpu.h"

#define PPU_CONTROL			0x2000
#define PPU_MASK			0x2001
#define PPU_STATUS			0x2002
#define PPU_OAM_ADDR		0x2003
#define PPU_OAM_DATA		0x2004
#define PPU_SCROLL			0x2005
#define PPU_ADDR			0x2006
#define PPU_DATA			0x2007

#define PPU_OAM_DMA			0x4014
#define CONTROLLER_1		0x4016
#define CONTROLLER_2		0x4017

#define ROM_LOWER_ADDRESS	0x8000
#define ROM_UPPER_ADDRESS	0xFFFF

#define INPUT_PORT_1		0x4016
#define INPUT_PORT_2		0x4017



class Memory {
public: 

	Memory();
	void write(unsigned short address, unsigned char value);
	unsigned char read(unsigned short address);
	void logMemory(std::string title);

private:
	static std::string hexFromLong(long i);
	unsigned char * RAM;
};

#endif