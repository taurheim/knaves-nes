#include <iostream>
#include <stdlib.h>
#include <regex>

#include "knaves.h"

int main(int argc, char * argv[]) {
	Knaves _knaves;

	if (argc > 1) {
		//Check to make sure first input is of the right type
		std::regex fileChecker("^.*\.nes");
		
		if (std::regex_match(argv[1], fileChecker)) {
			std::cout << "Attempting to Open: " << argv[1] << std::endl << std::endl;
			bool should_log = true;

			if (argc > 2 && std::string(argv[2]) == "-perf") {
				should_log = false;
			}

			_knaves.init(argv[1], should_log);
		} else {
			std::cout << "Incorrect file name. Please make sure you're running an NES ROM cartridge (should end in .nes)" << std::endl;
			std::getchar();
			return 0;
		}
	}
	else {
		std::cout << "No file provided. Please make the first argument the ROM file you wish to run." << std::endl;
		std::getchar();
		return 0;
		//For testing:
		//_knaves.init("knaves_tests.nes", true);
	}

	_knaves.run();

	return 0;
}