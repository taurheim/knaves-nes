#include <iostream>
#include <sstream>
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
		Memory::write(currentByteAddress + ROM_LOWER_ADDRESS, x);
		currentByteAddress++;

	}
	cout << "All done reading ROM. \n";
}


Cartridge::~Cartridge()
{
}
