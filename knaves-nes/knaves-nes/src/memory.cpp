#include "memory.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <ctime>

unsigned char *memory::RAM;

/*
Default constructor for memory
*/
memory::memory(){
	RAM = new unsigned char[2000];
}

/*
Write a value to the RAM
*/
void memory::write(unsigned short address, unsigned char value) {
	memory::RAM[address] = value;
	return;
}

int main(int argc, char** argv) {
	memory myMem;

	for (int i = 0; i < 2000; i++) {
		myMem.write(i, 0x32);
	}
	myMem.logMemory();
}
/*
 Read value from RAM
*/
unsigned char memory::read(unsigned short address) {

	if (address >= ROM_LOWER_ADDRESS && address <= ROM_UPPER_ADDRESS) {
		//Read from ROM
	}

	//Check for special cases (registers)
	switch (address) {
	case PPU_STATUS:
		break;
	case PPU_OAM_DATA:
		break;
	case PPU_VRAM_DATA:
		break;
	case CONTROLLER_1:
		break;
	case CONTROLLER_2:
		break;
	default:
		return memory::RAM[address];
	}
}

void memory::logMemory() {
	std::ofstream logFile;
	unsigned long int time = std::time(0);
	std::string timeString = std::to_string(time) + ".log";
	logFile.open("Memory - " + timeString);
	
	long memorySize = 2000;
	std::string tab = std::string("\t");
	std::string newLine = std::string("\n");
	std::string equalsChar = std::string("=");


	//Memory will be mapped in a 16x2000 grid
	//Each row will contain the 16 memory values for each multiple of 16
	int column = 0;
	logFile << tab;
	for (long i = 0; i < 0x10; i++) { 
		std::string columnNumber = memory::hexFromLong(i);
		logFile << tab + columnNumber;
	}
	logFile << newLine + tab + tab;
	for (long i = 0; i < 0x10 * 5 + 0x10 * 10; i++) {
		logFile << equalsChar;
	}

	logFile << newLine;

	for (long i = 0; i < memorySize; i++) {
		std::string memAddress = memory::hexFromLong(i);
		//Left hand side will have the row's address
		column++;

		if (column == 1) {
			logFile << memAddress;
		}

		std::string memValue = memory::hexFromLong(memory::RAM[i]);
		logFile << tab + memValue;
		if (column == 16) {
			logFile << newLine;
			column = 0;
		}

	}
	logFile.close();

}


std::string memory::hexFromLong(long i) {
	std::stringstream stream;
	stream << "0x"
		<< std::setfill('0') << std::setw(sizeof(i) * 2)
		<< std::hex << i;
	return stream.str();
}

