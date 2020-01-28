#pragma once

namespace IonEngine {
	// A Set with the following specs:
	// - fixed/constant allocation size
	// - guaranties contiguity
	// - does NOT guaranty locality
	template<typename T, u32 N>
	class CappedDenseSet {
	public:
		struct Iterator {
			Iterator(const CappedDenseSet<T, N>* set, u32 pos) : set(set), pos(pos) {}

			bool operator!=(const Iterator& o) const {
				return pos != o.pos;
			}

			T& operator*() const {
				return const_cast<CappedDenseSet<T, N>*>(set)->slots[pos];
			}

			const Iterator& operator++() {
				pos++;
				return *this;
			}

		private:
			const CappedDenseSet<T, N>* set;
			u32 pos;
		};

		/* ==== CONSTRUCTORS ==== */
		// Creates a new CappedDenseSet
		CappedDenseSet() : count(0) {}
		CappedDenseSet(const CappedDenseSet&) = delete;
		void operator=(const CappedDenseSet&) = delete;

		/* ==== METHODS ==== */
		/* Properties */
		// Returns the total count of items
		inline u32 getCount() const { return count; }
		// Returns the max capacity of the set
		constexpr u32 getCapacity() const { return N; }
		// Returns true if the set is empty (i.e. no items are stored)
		inline bool isEmpty() const { return count == 0; }

		/* Modification */
		// Adds the given item by copying its data to the set
		// !!!Beware that it means no constructor is called but problems
		// mights arise when the given item is destroyed!!! (prefer using `allocate()` with placement new)
		T* add(const T& item) {
			T* nItem = allocate();
			memcpy_s(reinterpret_cast<void*const>(nItem), itemSize, reinterpret_cast<const void*const>(&item), itemSize);
			//*nItem = static_cast<typename T&&>(item);
			return nItem;
		}

		// Allocates a slot in the set to host an item
		T* allocate() {
			count++;
			return slots + count - 1;
		}

		// Removes the item at the given index (no destructor will be called)
		void removeAt(u32 index) {
			memcpy_s(reinterpret_cast<void* const>(slots + index), itemSize, reinterpret_cast<const void* const>(slots + count - 1), itemSize);
			//*(slots + index) = static_cast<typename T&&>(*(slots + count - 1));
			count--;
		}

		// Remove the given item (the pointer must point to an item in the set, no destructor will be called)
		void remove(T* item) {
			memcpy_s(reinterpret_cast<void* const>(item), itemSize, reinterpret_cast<const void* const>(slots + count - 1), itemSize);
			//*item = static_cast<typename T&&>(*(slots + count - 1));
			count--;
		}

		// Clears the array (no destructor will be called)
		void clear() {
			count = 0;
		}

		/* Access */
		//
		inline T& operator[](u32 index) {
			return slots[index];
		}
		inline T& operator[](i32 index) {
			return slots[index];
		}

		/* Iteration */
		// The begining of the set
		Iterator begin() const {
			return Iterator(this, 0);
		}
		// The end of the set
		Iterator end() const {
			return Iterator(this, count);
		}

	private:
		T slots[N];
		u32 count;
		static constexpr size_t itemSize = sizeof(T);
	};
}
