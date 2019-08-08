#pragma once
#include <chrono>
#include "SimpleList.h"

namespace IonEngine {
	class Time {
	public:
		static float time;
		static float deltaTime;
		static int frameCount;

		static void computeFrameTimes();
		static float getExactTime();
		static void beginTimeMesure();
		static float endTimeMesure();
		static int getInstantFPS();

		Time() = delete;
		Time(const Time&) = delete;
		~Time() = delete;

	private:
		static std::chrono::high_resolution_clock::time_point startTime;
		static std::chrono::high_resolution_clock::time_point currentTime;
		static std::chrono::duration<float> durationSinceStart;
		static std::chrono::duration<float> durationFrame;

		static SimpleList<std::chrono::high_resolution_clock::time_point> timeStack;
	};
}
