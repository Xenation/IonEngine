#pragma once

#include <string>
#include "SimpleList.h"


namespace IonEngine {
	inline void splitString(const std::string& str, char delim, SimpleList<std::string_view>& list) {
		size_t currentPos = 0;
		while (currentPos < str.length()) {
			size_t delimPos = str.find(delim, currentPos);
			if (delimPos == std::string::npos) break;
			list.add(std::string_view(str.data() + currentPos, delimPos - currentPos));
			currentPos = delimPos;
		}
	}

	inline void splitString(const std::string& str, char delim, SimpleList<std::string>& list) {
		size_t currentPos = 0;
		while (currentPos < str.length()) {
			size_t delimPos = str.find(delim, currentPos);
			if (delimPos == std::string::npos) {
				list.add(str.substr(currentPos, str.length() - currentPos));
				break;
			}
			list.add(str.substr(currentPos, delimPos - currentPos));
			currentPos = delimPos + 1;
		}
	}
}
