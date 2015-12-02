#include <iostream>
#include <stdlib.h>

#include "knaves.h"

int main(int argc, char * argv[]) {
	Knaves _knaves;

	if (argc > 1) {
		std::cout << "Attempting to Open: " << argv[1] << std::endl << std::endl;
		bool should_log = true;
		if (argc > 2 && std::string(argv[2]) == "-perf") {
			should_log = false;
		}
		_knaves.init(argv[1], should_log);
	}
	else {
		_knaves.init("mario.nes", false);
	}

	_knaves.run();

	return 0;
}