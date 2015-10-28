#ifndef _KNAVES_H_
#define _KNAVES_H_

class Memory;
class cpu;

class Knaves {
public:
	Knaves();
	~Knaves(); //Destructor
	void init(std::string filename);
	void run();
	void stop();
	void reset();
private:
	cpu * _cpu;
	Memory * _memory;

	bool isRunning;
};

#endif