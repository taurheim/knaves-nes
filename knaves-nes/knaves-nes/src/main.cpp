#include <iostream>
#include <stdlib.h>

#include "knaves.h"

int main(int argc, char **argv) {
	Knaves _knaves;

	_knaves.init("untitled.nes");

	_knaves.run();

	return 0;
}