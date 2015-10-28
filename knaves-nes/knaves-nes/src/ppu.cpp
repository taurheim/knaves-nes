#include "ppu.h"
#include "memory.h"


ppu::ppu() {
	//TODO
}

void ppu::init(Memory * memory) {
	_memory = memory;
}

void ppu::writeRegister(unsigned short regAddr, unsigned char value) {
	_memory->write(regAddr, value);
}

unsigned char ppu::readRegister(unsigned short regAddr) {
	return _memory->read(regAddr);
}
