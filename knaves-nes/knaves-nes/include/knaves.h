#ifndef _KNAVES_H_
#define _KNAVES_H_

#include <chrono>

class Memory;
class cpu;
class Cartridge;

typedef std::chrono::high_resolution_clock Clock;

class Knaves {
public:
	Knaves();
	~Knaves(); //Destructor
	void init(char * filename, bool show_log);
	void run();
	void stop();
	void reset();
private:
	cpu * _cpu;
	Memory * _memory;
	Cartridge * _cartridge;

	bool isRunning;

	int sleep_time;
};

#endif