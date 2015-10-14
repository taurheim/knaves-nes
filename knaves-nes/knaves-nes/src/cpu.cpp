#include <iostream>

#include "cpu.h"

void cpu::start() {
	is_running = true;

	//Set Registers on startup
	reg_pc = STARTUP_PC;
	reg_status = STARTUP_STATUS;
	reg_acc = 0;
	reg_index_x = 0;
	reg_index_y = 0;


}