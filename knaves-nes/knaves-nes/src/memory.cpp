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
	return memory::RAM[address];
}