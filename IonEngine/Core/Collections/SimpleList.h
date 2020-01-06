#pragma once

namespace IonEngine {
	// A Collection that guaranties order and contiguity but does not guaranty location
	template<typename T>
	class SimpleList {
	public:
		u32 capacity;
		u32 count;
		u32 growAmount;

		SimpleList() = delete;
		SimpleList(const SimpleList<T>&) = delete;
		SimpleList(u32 capacity, u32 growAmount, bool lazyInit = false)
			: capacity(capacity), arr((lazyInit) ? nullptr : new T[capacity]), count(0), growAmount(growAmount) {

		}
		~SimpleList() {
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
			for (u32 i = index + 1; i < count; i++) {
				arr[i - 1] = arr[i];
			}
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

		inline T& operator[](u32 index) {
			return arr[index];
		}

	private:
		T* arr;

		void growArray() {
			capacity += growAmount;
			T* nArr = new T[capacity];
			if (nArr == nullptr) {
				//FAIL
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
