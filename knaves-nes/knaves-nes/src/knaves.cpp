#include <iostream>
#include <stdlib.h>

#include "knaves.h"
#include "cpu.h"

Knaves::Knaves() {

	isRunning = false;

	_cpu = new cpu();
	_memory = new Memory();
	_memory->readFromRom();
	_memory->logMemory();
}

Knaves::~Knaves() {
	delete _cpu;
	delete _memory;
}

void Knaves::init(std::string filename) {
	//Load the ROM
	
	//Try/catch eventually
	_cpu->init(_memory);
}

void Knaves::run() {
	isRunning = true;

	//Start the emulated components
	_cpu->start();

	while (isRunning) {
		unsigned short cycles = _cpu->executeInstruction();
		
		if (cycles == 0) {
			stop();
		}
	}
}

void Knaves::stop() {
	isRunning = false;
	getchar();
}