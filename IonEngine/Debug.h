#pragma once
#include <iostream>
#include <string>

class Debug {
public:
	static void log(const char* msg) {
		std::cout << "[LOG] " << msg << std::endl;
	}
	static void log(const char* label, const char* msg) {
		std::cout << "[LOG]-" << label << ": " << msg << std::endl;
	}
	static void log(const char* label, std::string msg) {
		std::cout << "[LOG]-" << label << ": " << msg << std::endl;
	}
	static void logError(const char* msg) {
		std::cout << "[ERROR] " << msg << std::endl;
	}
	static void logError(const char* label, const char* msg) {
		std::cout << "[ERROR]-" << label << ": " << msg << std::endl;
	}
	static void logError(const char* label, std::string msg) {
		std::cout << "[ERROR]-" << label << ": " << msg << std::endl;
	}
	static void fail(const char* msg) {
		logError(msg);
		logError("Unexpected Termination!");
		system("pause");
		exit(EXIT_FAILURE);
	}
};
