#include <iostream>
#include <string>
#include <fstream>
#include "cartridge.h"


Cartridge::Cartridge()
{
}

void Cartridge::init(Memory * mem) {
	_memory = mem;
}

void Cartridge::loadFromFile(char * fileName) {

	std::ifstream input(fileName, std::ios::in | std::ios::binary);

	unsigned short currentByteAddress = 0;
	while (input.good()) {
		unsigned char x;
		input.read(reinterpret_cast<char *>(&x), sizeof(unsigned char));
		_memory->write(currentByteAddress + ROM_LOWER_ADDRESS, x);
		currentByteAddress++;
	}

	std::cout << "All done reading ROM. \n";
}