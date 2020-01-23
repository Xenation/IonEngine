#pragma once

namespace IonEngine {
	// A List with the following specs:
	// - fixed/constant allocation size
	// - does NOT guaranty locality
	// - guaranties contiguity
	template<typename T, u32 N>
	class CappedList {
	public:
		struct Iterator {
			Iterator(const CappedList<T, N>* set, u32 pos) : set(set), pos(pos) {}

			bool operator!=(const Iterator& o) const {
				return pos != o.pos;
			}

			T& operator*() const {
				return const_cast<CappedList<T, N>*>(set)->slots[pos];
			}

			const Iterator& operator++() {
				pos++;
				return *this;
			}

		private:
			const CappedList<T, N>* set;
			u32 pos;
		};

		/* ==== CONSTRUCTORS ==== */
		//
		CappedList() : count(0) {}
		CappedList(const CappedList&) = delete;
		void operator=(const CappedList&) = delete;

		/* ==== METHODS ==== */
		/* Properties */
		// Returns the total count of items
		inline u32 getCount() const { return count; }
		// Returns the max capacity
		constexpr u32 getCapacity() const { return N; }
		// Returns true if the list is empty
		inline bool isEmpty() const { return count == 0; }

		/* Modification */
		// Adds the given item by copying its data to the list
		// !!!Beware that it means no constructor is called but problems
		// mights arise when the given item is destroyed!!! (prefer using `allocate()` with placement new)
		T* add(const T& item) {
			T* nItem = allocate();
			memcpy_s(reinterpret_cast<void* const>(nItem), itemSize, reinterpret_cast<const void* const>(&item), itemSize);
			return nItem;
		}

		// Inserts the given item at the given index (offsets items above) by copying
		// !!!Has the same potential problems of an add!!! (prefer using `allocateInsert()` with placement new)
		T* insert(u32 index, const T& item) {
			T* nItem = allocateInsert();
			memcpy_s(reinterpret_cast<void* const>(nItem), itemSize, reinterpret_cast<const void* const>(&item), itemSize);
			return nItem;
		}

		// Allocates a slot at the end of the list to host an item
		T* allocate() {
			count++;
			return slots + count - 1;
		}

		// Allocates a slot at the specified index offsetting all the items above
		T* allocateInsert(u32 index) {
			count++;
			for (u32 iIndex = count - 1; iIndex > index; iIndex--) {
				memcpy_s(reinterpret_cast<void* const>(slots + rIndex), itemSize, reinterpret_cast<const void* const>(slots + rIndex - 1), itemSize);
			}
			return slots + index;
		}

		// Removes the item at the given index (no destructor will be called)
		void removeAt(u32 index) {
			count--;
			for (u32 rIndex = index; rIndex < count; rIndex++) {
				memcpy_s(reinterpret_cast<void* const>(slots + rIndex), itemSize, reinterpret_cast<const void* const>(slots + rIndex + 1), itemSize);
			}
		}

		// Remove the given item (the pointer must point to an item in the list, no destructor will be called)
		inline void remove(T* item) {
			removeAt(static_cast<u32>(item - slots));
		}

		// Clears the list (no destructor will be called)
		inline void clear() {
			count = 0;
		}

		/* Access */
		//
		inline T* operator[](u32 index) {
			return slots + index;
		}
		inline T& operator[](u32 index) {
			return slots[index];
		}

		/* Iteration */
		//
		Iterator begin() const {
			return Iterator(this, 0);
		}
		Iterator end() const {
			return Iterator(this, count);
		}

	private:
		static constexpr size_t itemSize = sizeof(T);

		T slots[N];
		u32 count;
	};
}
