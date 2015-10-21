#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "cpu.h"


class memory;

class memory {
public: 
	static void pushStack(unsigned char value);
	static void write(unsigned short address, unsigned char value);
	static unsigned char read(unsigned short address);
private:
	unsigned char * RAM;
};

#endif