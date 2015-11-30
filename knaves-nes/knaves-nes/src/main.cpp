#include <iostream>
#include <stdlib.h>

#include "knaves.h"

int main(int argc, char * argv[]) {
	Knaves _knaves;

	std::cout << "Attempting to Open: " << argv[1] << std::endl << std::endl;

	_knaves.init(argv[1]);

	_knaves.run();

	return 0;
}