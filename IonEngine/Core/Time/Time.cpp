#include "Time.h"

#include <chrono>

using namespace IonEngine;
namespace chrn = std::chrono;



static chrn::high_resolution_clock::time_point startTime;

void Time::initializeTime() {
	startTime = chrn::high_resolution_clock::now();
}

double Time::getTime() {
	chrn::high_resolution_clock::time_point nTime = chrn::high_resolution_clock::now();
	return chrn::duration<double>(nTime - startTime).count();
}
