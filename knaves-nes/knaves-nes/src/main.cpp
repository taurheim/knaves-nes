#include <iostream>
#include <stdlib.h>

#include "knaves.h"

int main(int argc, char * argv[]) {
	Knaves _knaves;

	if (argv[1]) {
		std::cout << "Attempting to Open: " << argv[1] << std::endl << std::endl;
		bool should_log = true;
		if (std::string(argv[2]) == "-perf") {
			should_log = false;
		}
		_knaves.init(argv[1], should_log);
	}
	else {
		_knaves.init("appendix2.nes", true);
	}

	_knaves.run();

	return 0;
}