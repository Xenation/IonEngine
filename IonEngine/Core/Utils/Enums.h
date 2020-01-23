#pragma once

#ifndef ION_ENUM_MAX_NAME_SIZE
#define ION_ENUM_MAX_NAME_SIZE 64
#endif


template<typename T, size_t N>
struct __ConstexprArray {
	static constexpr size_t count = N;

	constexpr T& operator[](size_t index) {
		return arr[index];
	}

	constexpr const T& operator[](size_t index) const {
		return arr[index];
	}

private:
	T arr[N];
};

template<int StrMaxSize, size_t N>
struct __ConstexprStringArray {
	static constexpr size_t count = N;

	constexpr void setString(size_t strIdx, const char* start, size_t length) {
		if (length >= StrMaxSize) {
			length = StrMaxSize - 1;
		}
		for (size_t i = 0; i < length; i++) {
			arr[strIdx * StrMaxSize + i] = start[i];
		}
		arr[strIdx * StrMaxSize + length] = '\0';
	}

	constexpr const char* operator[](size_t index) {
		return arr + index * StrMaxSize;
	}

	constexpr const char* const operator[](size_t index) const {
		return arr + index * StrMaxSize;
	}

private:
	char arr[StrMaxSize * N];
};

constexpr size_t __getVariadicCount(const char* vaArgs) {
	u32 count = 0;
	u32 i = 0;
	while (vaArgs[i] != '\0') {
		if (vaArgs[i] == ',') {
			count++;
		}
		i++;
	}
	if (vaArgs[i - 1] != ',') {
		count++;
	}
	return count;
}

template<typename BaseType, int vaArgsCount>
constexpr auto __generateValuesArray(const char* vaArgs) {
	__ConstexprArray<BaseType, vaArgsCount> values = {};
	// -1: No number found, looking for the '=' sign
	// -2: No number found but '=' sign passed, looking for a number start
	// -3: A number has already been completed in this entry, skipping until next entry
	// >=0: Currently tracking a number, looking for the end of the number
	i32 numberStart = -1;
	size_t entry = 0;
	size_t i = 0;
	while (vaArgs[i] != '\0') {
		if (numberStart >= 0 && (vaArgs[i] == ' ' || vaArgs[i] == ',')) { // End of number found
			//values[entry] = numberStart;
			values[entry] = parseIntegerLiteral<BaseType>(vaArgs + numberStart, i - numberStart);
			if (vaArgs[i] == ',') {
				numberStart = -1;
				entry++;
			} else {
				numberStart = -3;
			}
		} else if (numberStart == -1) { // number may or may not be present
			if (vaArgs[i] == '=') { // Equal sign found, number will be after
				numberStart = -2;
			} else if (vaArgs[i] == ',') { // Next entry reached, no number specified
				if (entry == 0) {
					values[entry] = 0;
				} else {
					values[entry] = values[entry - 1] + 1;
				}
				entry++;
			}
		} else if (numberStart == -2 && (isBase10Digit(vaArgs[i]) || vaArgs[i] == '+' || vaArgs[i] == '-')) { // New number start found
			numberStart = static_cast<i32>(i);
		} else if (numberStart == -3 && vaArgs[i] == ',') { // New entry reached
			numberStart = -1;
			entry++;
		}
		i++;
	}

	if (numberStart >= 0) { // Finishes with a number digit
		values[entry] = parseIntegerLiteral<BaseType>(vaArgs + numberStart, i - numberStart);
	} else if (numberStart == -1 && entry < vaArgsCount) { // Finishes with a name, meaning an implicit value 
		if (entry == 0) {
			values[entry] = 0;
		} else {
			values[entry] = values[entry - 1] + 1;
		}
	}

	return values;
}

template<int vaArgsCount>
constexpr auto __generateNamesArray(const char* vaArgs) {
	__ConstexprStringArray<ION_ENUM_MAX_NAME_SIZE, vaArgsCount> names = {};
	// -1: No word found, looking for one in the current entry
	// -2: A word has already been completed in this entry, skipping until next entry
	// >=0: Currently tracking a word, looking for the end of the word
	i32 wordStart = -1;
	size_t entry = 0;
	size_t i = 0;
	while (vaArgs[i] != '\0') { // TODO handle tabs
		if (wordStart >= 0 && (vaArgs[i] == ' ' || vaArgs[i] == '=' || vaArgs[i] == ',')) { // End of word found
			names.setString(entry, vaArgs + wordStart, i - wordStart);
			if (vaArgs[i] == ',') {
				wordStart = -1;
				entry++;
			} else {
				wordStart = -2;
			}
		} else if (wordStart == -1 && vaArgs[i] != ' ') { // New word start found
			wordStart = static_cast<i32>(i);
		} else if (wordStart == -2 && vaArgs[i] == ',') { // New entry start found
			wordStart = -1;
			entry++;
		}
		i++;
	}

	if (wordStart >= 0) { // Finishes with name character
		names.setString(entry, vaArgs + wordStart, i - wordStart);
	}

	return names;
}

template<int vaArgsCount>
constexpr auto __generateNameHashesArray(const char* vaArgs) {
	__ConstexprArray<strId, vaArgsCount> hashes = {};
	// -1: No word found, looking for one in the current entry
	// -2: A word has already been completed in this entry, skipping until next entry
	// >=0: Currently tracking a word, looking for the end of the word
	i32 wordStart = -1;
	size_t entry = 0;
	size_t i = 0;
	while (vaArgs[i] != '\0') { // TODO handle tabs
		if (wordStart >= 0 && (vaArgs[i] == ' ' || vaArgs[i] == '=' || vaArgs[i] == ',')) { // End of word found
			hashes[entry] = crc64(vaArgs + wordStart, static_cast<u32>(i) - static_cast<u32>(wordStart));
			if (vaArgs[i] == ',') {
				wordStart = -1;
				entry++;
			} else {
				wordStart = -2;
			}
		} else if (wordStart == -1 && vaArgs[i] != ' ') { // New word start found
			wordStart = static_cast<i32>(i);
		} else if (wordStart == -2 && vaArgs[i] == ',') { // New entry start found
			wordStart = -1;
			entry++;
		}
		i++;
	}

	if (wordStart >= 0) { // Finishes with name character
		hashes[entry] = crc64(vaArgs + wordStart, static_cast<u32>(i) - static_cast<u32>(wordStart));
	}

	return hashes;
}

/* TO REMOVE */
/*namespace {
	enum class __TypeName : u32 {
		TEST = 0x50, A = 0b10000
	};

	namespace __TypeNameReflect {
		static constexpr char __vaArgs[] = "TEST = 0x50, A = 0b10000";
		static constexpr size_t __vaArgsCount = __getVariadicCount(__vaArgs);
		static constexpr __ConstexprArray<u32, __vaArgsCount> values = __generateValuesArray<u32, __vaArgsCount>(__vaArgs);
		static constexpr __ConstexprStringArray<ION_ENUM_MAX_NAME_SIZE, __vaArgsCount> names = __generateNamesArray<__vaArgsCount>(__vaArgs);
		static constexpr __ConstexprArray<strId, __vaArgsCount> nameHashes = __generateNameHashesArray<__vaArgsCount>(__vaArgs);

		static constexpr __TypeName toValue(strId hash) {
			for (size_t i = 0; i < __vaArgsCount; i++) {
				if (nameHashes[i] == hash) {
					return static_cast<__TypeName>(values[i]);
				}
			}
			return static_cast<__TypeName>(0);
		}

		static constexpr const char* toName(__TypeName e) {
			for (size_t i = 0; i < __vaArgsCount; i++) {
				if (values[i] == static_cast<u32>(e)) {
					return names[i];
				}
			}
			return nullptr;
		}

		static constexpr const char* toName(strId hash) {
			for (size_t i = 0; i < __vaArgsCount; i++) {
				if (nameHashes[i] == hash) {
					return names[i];
				}
			}
			return nullptr;
		}

		static constexpr strId toHash(__TypeName e) {
			for (size_t i = 0; i < __vaArgsCount; i++) {
				if (values[i] == static_cast<u32>(e)) {
					return nameHashes[i];
				}
			}
			return 0;
		}

	}
}*/


#define ION_ENUM(TypeName, BaseType, ...)\
enum class TypeName : BaseType { __VA_ARGS__ };\
\
namespace TypeName##Reflect {\
	static constexpr char __vaArgs[] = #__VA_ARGS__;\
	static constexpr size_t __vaArgsCount = __getVariadicCount(__vaArgs);\
	static constexpr __ConstexprArray<BaseType, __vaArgsCount> values = __generateValuesArray<BaseType, __vaArgsCount>(__vaArgs);\
	static constexpr __ConstexprStringArray<ION_ENUM_MAX_NAME_SIZE, __vaArgsCount> names = __generateNamesArray<__vaArgsCount>(__vaArgs);\
	static constexpr __ConstexprArray<strId, __vaArgsCount> nameHashes = __generateNameHashesArray<__vaArgsCount>(__vaArgs);\
\
	static constexpr TypeName toValue(strId hash) {\
		for (size_t i = 0; i < __vaArgsCount; i++) {\
			if (nameHashes[i] == hash) {\
				return static_cast<TypeName>(values[i]);\
			}\
		}\
		return static_cast<TypeName>(0);\
	}\
\
	static constexpr const char* toName(TypeName e) {\
		for (size_t i = 0; i < __vaArgsCount; i++) {\
			if (values[i] == static_cast<BaseType>(e)) {\
				return names[i];\
			}\
		}\
		return nullptr;\
	}\
\
	static constexpr const char* toName(strId hash) {\
		for (size_t i = 0; i < __vaArgsCount; i++) {\
			if (nameHashes[i] == hash) {\
				return names[i];\
			}\
		}\
		return nullptr;\
	}\
\
	static constexpr strId toHash(TypeName e) {\
		for (size_t i = 0; i < __vaArgsCount; i++) {\
			if (values[i] == static_cast<BaseType>(e)) {\
				return nameHashes[i];\
			}\
		}\
		return 0;\
	}\
}\

