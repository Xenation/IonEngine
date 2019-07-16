#pragma once
#include "Debug.h"
template<typename T>
class HollowSet {
public:
	T* arr;
	unsigned int capacity;
	unsigned int count;
	unsigned int growAmount;
	
	HollowSet() = delete;
	HollowSet(const HollowSet<T>&) = delete;
	HollowSet(unsigned int capacity, unsigned int growAmount)
		: capacity(capacity), arr(new T[capacity]), count(0), growAmount(growAmount) {
		for (unsigned int i = 0; i < this->capacity; i++) {
			arr[i] = nullptr;
		}
	}
	~HollowSet() {
		delete[] arr;
	}

	unsigned int add(T elem) {
		if (count == capacity) {
			growArray();
		}
		count++;
		unsigned int index = getFreeIndex();
		arr[index] = elem;
		return index;
	}

	void remove(unsigned int index) {
		arr[index] = nullptr;
	}

	void clear() {
		for (unsigned int i = 0; i < capacity; i++) {
			arr[i] = nullptr;
		}
	}

	inline T& operator [](const unsigned int index) {
		return arr[index];
	}

private:
	unsigned int getFreeIndex() {
		unsigned int fIndex = 0;
		for (; fIndex < capacity; fIndex++) {
			if (arr[fIndex] == nullptr) break;
		}
		return fIndex;
	}

	void growArray() { // Implies growth is done only when the array is full
		capacity += growAmount;
		T* nArr = new T[capacity];
		if (nArr == nullptr) {
			Debug::logError("HollowSet", "Could not allocate memory for array growth!");
			return;
		}
		unsigned int index = 0;
		for (; index < count; index++) {
			nArr[index] = arr[index];
		}
		for (; index < capacity; index++) {
			nArr[index] = nullptr;
		}
		delete[] arr;
		arr = nArr;
	}
};

