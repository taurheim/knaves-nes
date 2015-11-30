#include <iostream>
#include <stdlib.h>

#include "knaves.h"
#include "cpu.h"
#include "cartridge.h"

Knaves::Knaves() {

	isRunning = false;

	_cpu = new cpu();
	_memory = new Memory();
	_cartridge = new Cartridge();
}

Knaves::~Knaves() {
	delete _cpu;
	delete _memory;
}

void Knaves::init(char * fileName) {
	//Load the ROM
	
	//Try/catch eventually
	_cpu->init(_memory);
	_cartridge->init(_memory);

	_cartridge->loadFromFile(fileName);
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