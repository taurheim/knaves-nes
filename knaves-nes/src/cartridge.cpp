#include <iostream>
#include <string>
#include <fstream>
#include "cartridge.h"


Cartridge::Cartridge()
{
}

void Cartridge::init(Memory * mem, bool show_log) {
	_memory = mem;
}

void Cartridge::loadFromFile(char * fileName) {
	std::cout << "Opening file... " << fileName << std::endl;
	std::ifstream input(fileName, std::ios::in | std::ios::binary);

	unsigned short currentByteAddress = 0;
	while (input.good()) {
		unsigned char x;
		input.read(reinterpret_cast<char *>(&x), sizeof(unsigned char));
		_memory->write(currentByteAddress + ROM_LOWER_ADDRESS -0x10, x);
		if (currentByteAddress + ROM_LOWER_ADDRESS == 0xFFFC) {
			std::cout << "Upper byte start: " << std::hex << (int)x << std::endl;
		}
		if (currentByteAddress + ROM_LOWER_ADDRESS == 0xFFFD) {
			std::cout << "Lower byte start: " << std::hex << (int) x << std::endl;
		}
		currentByteAddress++;
	}

	std::cout << "All done reading ROM. \n";
}