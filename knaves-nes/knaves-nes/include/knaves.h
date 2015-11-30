#ifndef _KNAVES_H_
#define _KNAVES_H_

class Memory;
class cpu;
class Cartridge;

class Knaves {
public:
	Knaves();
	~Knaves(); //Destructor
	void init(char * filename);
	void run();
	void stop();
	void reset();
private:
	cpu * _cpu;
	Memory * _memory;
	Cartridge * _cartridge;

	bool isRunning;
};

#endif