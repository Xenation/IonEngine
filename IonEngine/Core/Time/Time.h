#pragma once

namespace IonEngine {
	class Time {
	public:
		Time() = delete;
		Time(const Time&) = delete;
		void operator=(const Time&) = delete;

		static void initializeTime();
		static double getTime();
	};
}
