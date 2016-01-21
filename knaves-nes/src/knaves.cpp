#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>

#include "knaves.h"
#include "cpu.h"
#include "cartridge.h"
#include "ppu.h"

Knaves::Knaves() {

	isRunning = false;

	_cpu = new cpu();
	_memory = new Memory();
	_cartridge = new Cartridge();
	_ppu = new ppu();
}

Knaves::~Knaves() {
	delete _cpu;
	delete _memory;
}

void Knaves::init(char * fileName, bool show_log) {
	perfTest = !show_log;
	//Load the ROM
	
	//TODO Add try/catch and error handling system
	_cpu->init(_memory, show_log);
	_cartridge->init(_memory, show_log);

	_cartridge->loadFromFile(fileName);

	if(!perfTest) _memory->logMemory("pre");

	_ppu->init(_memory);

	//Calculate the user's computer speed. The sleep() function incurs its own lag, and if used in increments too small, it will significantly throw off the timing of the CPU
	int total_slept = 0;
	for (int i = 0; i < 10; i++) {
		Clock::time_point start = Clock::now();
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		Clock::time_point end = Clock::now();
		std::chrono::nanoseconds duration = end - start;
		total_slept += duration.count();
	}

	sleep_time = total_slept / 10;
}

void Knaves::run() {
	isRunning = true;

	//Start the emulated components
	_cpu->start();

	//Start tracking clocks
	auto main_clock_start = Clock::now();

	int total_cycles = 0;
	int last_cycles = 0;
	
	//Cycles per second: 1.79 MHz
	double CPU_clock_speed = 1790000;//cycles/sec
	double ns_per_cycle = 1/(CPU_clock_speed / 1000000000); //ns

	while (isRunning) {
		//Modelling NTSC emulator
		//Runs at 21.47MHz = 21,470,000 cycles/sec

		unsigned short cycles = _cpu->executeInstruction();
		_ppu->runCycles(cycles);

		total_cycles += (int) cycles;
		
		//How long should all of this have taken? (check every 1000 cycles or so)
		if (total_cycles - last_cycles > 2500) {
			last_cycles = total_cycles;

			double target_time_taken = total_cycles * ns_per_cycle;
			double actual_time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - main_clock_start).count();
			int difference = target_time_taken - actual_time_taken;

			//The user's computer has an average time that the call to thread.sleep() takes
			if (difference > sleep_time) {
				//std::cout << "\tSleeping for " << std::dec << difference - sleep_time << std::endl;
				std::this_thread::sleep_for(std::chrono::nanoseconds(difference - sleep_time));
			}
		}


		if (cycles == 0) {
			stop();
		}
		
	}

	auto main_clock_end = Clock::now();
	double time_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(main_clock_end - main_clock_start).count();

	//Status Report
	std::cout << "Done" << std::endl;
	std::cout << "Time Passed: " << time_elapsed << " nanoseconds (" << (time_elapsed / 1000000000) << " seconds)" << std::endl;
	std::cout << "Total Cycles: " << std::dec << total_cycles << std::endl;
	std::cout << "Target Nanoseconds Per Cycle: " << ns_per_cycle << std::endl;
	std::cout << "Actual Nanoseconds Per Cycle: " << time_elapsed / total_cycles << std::endl;
	std::cout << "Cycles Per Second: " << (double) total_cycles / (time_elapsed / 1000000000) << " -- Should be 1.79e6" <<std::endl;

	if(!perfTest) _memory->logMemory("post");

	//Press enter to close
	std::getchar();
}

void Knaves::stop() {
	isRunning = false;
}