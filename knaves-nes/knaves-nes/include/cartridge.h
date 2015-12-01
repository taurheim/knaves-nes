#ifndef _CARTRIDGE_H_
#define _CARTRIDGE_H_

#include "memory.h"

class Cartridge
{
public:
	Cartridge();
	void init(Memory * memory, bool show_log);
	void loadFromFile(char * fileName);
private:
	Memory * _memory;
};

#endif