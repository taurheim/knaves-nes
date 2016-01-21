#include "memory.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <ctime>

using namespace std;

/*
Default constructor for memory
*/
Memory::Memory(){
	RAM = new unsigned char[0xFFFF];
	memset(RAM, 0, 0xFFFF);

}

/*
Write a value to the RAM
See http://wiki.nesdev.com/w/index.php/CPU_memory_map for a detailed memory map
*/
void Memory::write(unsigned short address, unsigned char value) {
	if (address >= 0x0000 && address <= 0x1FFF) {
		//Writing to internal RAM
		//Write to all mirrors
		unsigned short real_address = address & 0x07FF;
		RAM[real_address] = value;
		/* Not sure if this is actually necessary for the emulator
		RAM[real_address + 0x0800] = value;
		RAM[real_address + 0x1000] = value;
		RAM[real_address + 0x1800] = value;
		*/
	} else if (address >= 0x2000 && address <= 0x3FFF) {
		//Writing to PPU Registers
		unsigned short real_address = address & 0x2007;
		/* Not sure if this is actually necessary for the emulator
		real_address += 0x0008
		while(real_address < 0x3FFF){
			//Same case statement as above
			real_address += 0x0008
		}
		*/

	} else if (address >= 4000 && address <= 0x401F) {
		//NES APU and I/O Registers
		switch (address) {
		case INPUT_PORT_1:
			//CPU stuff
			break;
		case INPUT_PORT_2:
			//CPU stuff
			break;
		case PPU_OAM_DMA:
			//PPU stuff
			break;
		}
	} else if (address >= 4020 && address <= 0xFFFF) {
		//Cartridge Space: PRG ROM, PRG RAM, mapper
		RAM[address] = value;
	} else {
		RAM[address] = value;
	}
	return;
}


/*
 Read value from RAM
*/
unsigned char Memory::read(unsigned short address) {

	return RAM[address];
}

void Memory::logMemory(std::string title) {
	std::cout << "Saving Memory \"" << title << "\" ...";
	std::ofstream logFile;
	logFile.open("Memory - " + title + ".log");

	long memorySize = 0xFFFF;
	std::string tab = std::string("\t");
	std::string newLine = std::string("\n");
	std::string equalsChar = std::string("=");


	//Memory will be mapped in a 16x2000 grid
	//Each row will contain the 16 memory values for each multiple of 16
	int column = 0;
	logFile << tab;
	for (long i = 0; i < 0x10; i++) {
		std::string columnNumber = hexFromLong(i);
		logFile << tab + columnNumber;
	}
	logFile << newLine + tab + tab;
	for (long i = 0; i < 0x10 * 5 + 0x10 * 10; i++) {
		logFile << equalsChar;
	}

	logFile << newLine;

	for (long i = 0; i < memorySize; i++) {
		std::string memAddress = hexFromLong(i);
		//Left hand side will have the row's address
		column++;

		if (column == 1) {
			logFile << memAddress;
		}

		std::string memValue = hexFromLong(RAM[i]);

		logFile << tab + memValue;
		if (column == 16) {
			logFile << newLine;
			column = 0;
		}
	}
	logFile.close();

	std::cout << "Finished." << std::endl;
}


std::string Memory::hexFromLong(long i) {
	std::stringstream stream;
	stream << "0x"
		<< std::setfill('0') << std::setw(2)
		<< std::hex << i;
	return stream.str();
}

