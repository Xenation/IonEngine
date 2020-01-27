#pragma once

namespace IonEngine {
	class Debug {
	public:
		enum Severity : u32 {
			INF = 0b0001,
			WAR = 0b0010,
			ERR = 0b0100
		};

		enum Category : u32 {
			NONE = 0b00000000'00000000'00000000'00000001,
			CORE = 0b00000000'00000000'00000000'00000010,
			DISPLAY = 0b00000000'00000000'00000000'00000100,
			RENDER = 0b00000000'00000000'00000000'00001000,
		};

		Debug() = delete;
		Debug(const Debug&) = delete;
		void operator=(const Debug&) = delete;

		static u32 severityFilter;
		static u32 categoryFilter;

		static void log(Severity severity, Category category, const char* format, ...);
		static void logInfo(Category category, const char* format, ...);
		static void logInfo(const char* format, ...);
		static void logWarning(Category category, const char* format, ...);
		static void logWarning(const char* format, ...);
		static void logError(Category category, const char* format, ...);
		static void logError(const char* format, ...);
		static void fail(Category category, const char* format, ...);
		static void fail(const char* format, ...);
	};
}
