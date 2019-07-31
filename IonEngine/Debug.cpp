#include "Debug.h"

#include <sstream>



std::stringstream* Debug::out;
Debug::StaticConstructor Debug::staticConstructor;

Debug::StaticConstructor::StaticConstructor() {
	out = new std::stringstream();
	std::cout.rdbuf(out->rdbuf());
}