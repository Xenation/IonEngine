#pragma once
#include "Debug.h"

namespace IonEngine {
	// A Collection that does not guaranty order and contiguity but garanties that an element never moves
	template<typename T>
	class HollowSet {
	public:
		u32 capacity;
		u32 count;
		u32 growAmount;

		HollowSet() = delete;
		HollowSet(const HollowSet<T>&) = delete;
		HollowSet(u32 capacity, u32 growAmount)
			: capacity(capacity), arr(new T[capacity]), count(0), growAmount(growAmount) {
			for (u32 i = 0; i < this->capacity; i++) {
				arr[i] = nullptr;
			}
		}
		~HollowSet() {
			delete[] arr;
		}

		u32 add(T elem) {
			if (count == capacity) {
				growArray();
			}
			count++;
			u32 index = getFreeIndex();
			arr[index] = elem;
			return index;
		}

		void remove(u32 index) {
			arr[index] = nullptr;
		}

		void clear() {
			for (u32 i = 0; i < capacity; i++) {
				arr[i] = nullptr;
			}
		}

		inline T& operator[](const u32 index) {
			return arr[index];
		}

		inline const T& operator[](u32 index) const {
			return arr[index];
		}

	private:
		T* arr;

		u32 getFreeIndex() {
			u32 fIndex = 0;
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
			u32 index = 0;
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
}
