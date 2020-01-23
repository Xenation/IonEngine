#pragma once
#include "Debug.h"

namespace IonEngine {
	// A Collection that does not guaranty order but guaranties contiguity
	template<typename T>
	class SimpleSet {
	public:
		u32 capacity;
		u32 count;
		u32 growAmount;

		SimpleSet() = delete;
		SimpleSet(const SimpleSet&) = delete;
		SimpleSet(u32 capacity, u32 growAmount, bool lazyInit = false)
			: capacity(capacity), arr((lazyInit) ? nullptr : new T[capacity]), count(0), growAmount(growAmount) {

		}
		~SimpleSet() {
			delete[] arr;
		}

		u32 add(T elem) {
			if (arr == nullptr) { // Lazy initialize
				arr = new T[capacity];
			}
			if (count == capacity) {
				growArray();
			}
			u32 index = count++;
			arr[index] = elem;
			return index;
		}

		void removeAt(u32 index) {
			arr[index] = arr[count - 1];
			count--;
		}

		void remove(T item) {
			u32 i;
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
			for (u32 i = 0; i < count; i++) {
				if (arr[i] == item) {
					return true;
				}
			}
			return false;
		}

		inline T& operator[](u32 index) {
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
			u32 index = 0;
			for (; index < count; index++) {
				nArr[index] = arr[index];
			}
			delete[] arr;
			arr = nArr;
		}
	};
}
