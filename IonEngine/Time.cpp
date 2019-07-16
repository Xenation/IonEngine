#include "Time.h"

using namespace std::chrono;



float Time::time = 0.f;
float Time::deltaTime = 0.f;

high_resolution_clock::time_point Time::startTime = high_resolution_clock::now();
high_resolution_clock::time_point Time::currentTime = high_resolution_clock::now();
duration<float> Time::durationSinceStart;
duration<float> Time::durationFrame;

SimpleList<high_resolution_clock::time_point> Time::timeStack = SimpleList<high_resolution_clock::time_point>(4, 4);

void Time::computeFrameTimes() {
	high_resolution_clock::time_point nTime = high_resolution_clock::now();
	durationSinceStart = duration_cast<duration<float>>(nTime - startTime);
	durationFrame = duration_cast<duration<float>>(nTime - currentTime);
	currentTime = nTime;
	time = durationSinceStart.count();
	deltaTime = durationFrame.count();
}

float Time::getExactTime() {
	high_resolution_clock::time_point cTime = high_resolution_clock::now();
	duration<float> sinceStart = duration_cast<duration<float>>(cTime - startTime);
	return sinceStart.count();
}

void Time::beginTimeMesure() {
	timeStack.add(high_resolution_clock::now());
}

float Time::endTimeMesure() {
	float dt = duration_cast<duration<float>>(high_resolution_clock::now() - timeStack[timeStack.count - 1]).count();
	timeStack.count--;
	return dt;
}
