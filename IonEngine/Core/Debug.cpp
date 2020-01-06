#include "Debug.h"

#include <stdio.h>
#include <Windows.h>
using namespace IonEngine;



u32 Debug::severityFilter = 0xffffffff;
u32 Debug::categoryFilter = 0xffffffff;

void vLog(const char* format, va_list argList) {
	const u32 MAX_CHARS = 1024;
	static char s_buffer[MAX_CHARS];

	int writtenChars = vsnprintf(s_buffer, MAX_CHARS - 1, format, argList);

	s_buffer[writtenChars] = '\n';
	s_buffer[writtenChars + 1] = '\0';

	OutputDebugStringA(s_buffer);
}

void vLog(Debug::Severity severity, Debug::Category category, const char* format, va_list argList) {
	if ((severity & Debug::severityFilter) != severity) return;
	if ((category & Debug::categoryFilter) != category) return;

	vLog(format, argList);
}

void Debug::log(Debug::Severity severity, Debug::Category category, const char* format, ...) {
	va_list argList;
	va_start(argList, format);
	
	vLog(severity, category, format, argList);

	va_end(argList);
}

void Debug::logInfo(Debug::Category category, const char* format, ...) {
	va_list argList;
	va_start(argList, format);

	vLog(INF, category, format, argList);

	va_end(argList);
}

void Debug::logInfo(const char* format, ...) {
	va_list argList;
	va_start(argList, format);

	vLog(INF, NONE, format, argList);

	va_end(argList);
}

void Debug::logWarning(Category category, const char* format, ...) {
	va_list argList;
	va_start(argList, format);

	vLog(WAR, category, format, argList);

	va_end(argList);
}

void Debug::logWarning(const char* format, ...) {
	va_list argList;
	va_start(argList, format);

	vLog(WAR, NONE, format, argList);

	va_end(argList);
}

void Debug::logError(Category category, const char* format, ...) {
	va_list argList;
	va_start(argList, format);

	vLog(ERR, category, format, argList);

	va_end(argList);
}

void Debug::logError(const char* format, ...) {
	va_list argList;
	va_start(argList, format);

	vLog(ERR, NONE, format, argList);

	va_end(argList);
}

void Debug::fail(Category category, const char* format, ...) {
	va_list argList;
	va_start(argList, format);

	vLog(ERR, category, format, argList);

	va_end(argList);
}

void Debug::fail(const char* format, ...) {
	va_list argList;
	va_start(argList, format);

	vLog(ERR, NONE, format, argList);

	va_end(argList);
}
