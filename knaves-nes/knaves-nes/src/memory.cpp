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
	RAM = new unsigned char[2000];
	memset(RAM, 0, 2000);
}

/*
Write a value to the RAM
*/
void Memory::write(unsigned short address, unsigned char value) {
	RAM[address] = value;
	return;
}

void Memory::readFromRom() {
	cout << "Reading from ROM";
	std::ifstream file("untitled.nes", ios::in | ios::binary); //load the file 

	std::string str;
	std::string file_contents;
	while (std::getline(file, str))
	{
		file_contents += str;
		file_contents.push_back('\n');
	}

	static const char* const lut = "0123456789ABCDEF"; //convert back to hex
	size_t len = file_contents.length();

	std::string output;
	output.reserve(2 * len);
	for (size_t i = 0; i < len; ++i)
	{
		const unsigned char c = file_contents[i];
		output.push_back(lut[c >> 4]);
		output.push_back(lut[c & 15]);

		const unsigned char c1 = lut[c >> 4];
		const unsigned char c2 = lut[c & 15];
		cout << c1;
		cout << c2;

		Memory::write(i, c1);
		Memory::write(i + 1, c2);
	}

	output.erase(0, 32);
	//cout << output;
	for (int i = 0; i < 100; i++) {
		//cout << file_contents[i];
	}

//	if (address >= ROM_LOWER_ADDRESS && address <= ROM_UPPER_ADDRESS) {
//		cout << output.substr(0, 4);
//		output.erase(0, 4);
		//Read from ROM
}


/*
 Read value from RAM
*/
unsigned char Memory::read(unsigned short address) {

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
		return RAM[address];
	}
}

void Memory::logMemory() {
	std::ofstream logFile;
	unsigned long int time = std::time(0);
	std::string timeString = std::to_string(time) + ".log";
	logFile.open("Memory - " + timeString);
	
	long memorySize = (sizeof(RAM) / sizeof(*RAM));
	for (long i = 0; i < memorySize; i++) {
		std::string memAddress = Memory::hexFromLong(i);
		std::string memValue = Memory::hexFromLong(RAM[i]);
		logFile << memAddress + std::string("\t") + memValue + std::string("\t");
	}
	logFile.close();

}


std::string Memory::hexFromLong(long i) {
	std::stringstream stream;
	stream << "0x"
		<< std::setfill('0') << std::setw(sizeof(i) * 2)
		<< std::hex << i;
	return stream.str();
}

