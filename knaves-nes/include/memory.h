#ifndef _MEMORY_H_
#define _MEMORY_H_

#define PPU_OAM_DMA			0x4014
#define CONTROLLER_1		0x4016
#define CONTROLLER_2		0x4017

#define ROM_LOWER_ADDRESS	0x8000
#define ROM_UPPER_ADDRESS	0xFFFF

#define INPUT_PORT_1		0x4016
#define INPUT_PORT_2		0x4017

#include <string>

class Memory {
public: 

	Memory();
	void write(unsigned short address, unsigned char value);
	unsigned char read(unsigned short address);
	void logMemory(std::string title);

private:
	std::string hexFromLong(long i);
	unsigned char * RAM;
};

#endif