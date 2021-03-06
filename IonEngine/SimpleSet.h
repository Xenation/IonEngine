#pragma once
#include "Debug.h"

namespace IonEngine {
	// A Collection that does not guaranty order but guaranties contiguity
	template<typename T>
	class SimpleSet {
	public:
		unsigned int capacity;
		unsigned int count;
		unsigned int growAmount;

		SimpleSet() = delete;
		SimpleSet(const SimpleSet&) = delete;
		SimpleSet(unsigned int capacity, unsigned int growAmount, bool lazyInit = false)
			: capacity(capacity), arr((lazyInit) ? nullptr : new T[capacity]), count(0), growAmount(growAmount) {

		}
		~SimpleSet() {
			delete[] arr;
		}

		unsigned int add(T elem) {
			if (arr == nullptr) { // Lazy initialize
				arr = new T[capacity];
			}
			if (count == capacity) {
				growArray();
			}
			int index = count++;
			arr[index] = elem;
			return index;
		}

		void removeAt(unsigned int index) {
			arr[index] = arr[count - 1];
			count--;
		}

		void remove(T item) {
			unsigned int i;
			for (i = 0; i < count; i++) {
				if (arr[i] == item) break;
			}
			if (i < count) {
				removeAt(i);
			}
		}

		void clear() {
			count = 0;
		}

		bool contains(T item) const {
			for (unsigned int i = 0; i < count; i++) {
				if (arr[i] == item) {
					return true;
				}
			}
			return false;
		}

		inline T& operator[](unsigned int index) {
			return arr[index];
		}

	private:
		T* arr;

		void growArray() {
			capacity += growAmount;
			T* nArr = new T[capacity];
			if (nArr == nullptr) {
				Debug::logError("SimpleList", "Could not allocate memory for array growth!");
				return;
			}
			unsigned int index = 0;
			for (; index < count; index++) {
				nArr[index] = arr[index];
			}
			delete[] arr;
			arr = nArr;
		}
	};
}
