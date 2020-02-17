#pragma once
#include "CollectionParams.h"
#include "VirtualBuffer.h"

namespace IonEngine {
	// A List with the following specs:
	// - dynamic allocation size
	// - guaranties contiguity
	// - does NOT guaranty locality
	// (designed for large sizes)
	template<typename T>
	class BulkList {
	public:
		struct Iterator {
			Iterator(const BulkList<T>* set, u32 pos) : set(set), pos(pos) {}

			bool operator!=(const Iterator& o) const {
				return pos != o.pos;
			}

			T& operator*() const {
				return const_cast<BulkList<T>*>(set)->slots[pos];
			}

			const Iterator& operator++() {
				pos++;
				return *this;
			}

		private:
			const BulkList<T>* set;
			u32 pos;
		};

		/* ==== CONSTRUCTORS ==== */
		//
		BulkList() : count(0), bufferCount(0), slotsBuffer(ION_BULK_VIRT_ALLOC_SIZE) {}
		BulkList(const BulkList&) = delete;
		void operator=(const BulkList&) = delete;

		/* ==== METHODS ==== */
		/* Properties */
		// Returns the total count of items
		inline u32 getCount() const { return count; }
		// Returns true if there are no items in the list, false otherwise
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
			size_t bufferRange = slotsBuffer.getUsedRange();
			count++;
			if (count * itemSize > bufferRange) {
				bufferRange = count * itemSize;
				slotsBuffer.setUsedRange(bufferRange);
			}
			return slots + count - 1;
		}

		// Allocates a slot at the specified index offsetting all the items above
		T* allocateInsert(u32 index) {
			size_t bufferRange = slotsBuffer.getUsedRange();
			count++;
			if (count * itemSize > bufferRange) {
				bufferRange = count * itemSize;
				slotsBuffer.setUsedRange(bufferRange);
			}
			for (u32 iIndex = count - 1; iIndex > index; iIndex--) {
				memcpy_s(reinterpret_cast<void* const>(slots + iIndex), itemSize, reinterpret_cast<const void* const>(slots + iIndex - 1), itemSize);
			}
		}

		// Removes the item at the given index (no destructor will be called)
		// use `freeMemory` to specify if the now unused memory should be freed
		void removeAt(u32 index, bool freeMemory = false) {
			count--;
			if (freeMemory) {
				u32 bufferRange = count * itemSize;
				slotsBuffer.setUsedRange(bufferRange);
			}
			for (u32 rIndex = index; rIndex < count; rIndex++) {
				memcpy_s(reinterpret_cast<void* const>(slots + rIndex), itemSize, reinterpret_cast<const void* const>(slots + rIndex + 1), itemSize);
			}
		}

		// Remove the given item (the pointer must point to an item in the list, no destructor will be called)
		// use `freeMemory` to specify if the now unused memory should be freed
		void remove(T* item, bool freeMemory = false) {
			removeAt(static_cast<u32>(item - slots), freeMemory);
		}

		// Clears the list (no destructor will be called)
		// use `freeMemory` to specify if the now unused memory should be freed
		void clear(bool freeMemory = false) {
			count = 0;
			if (freeMemory) {
				size_t bufferRange = count * itemSize;
				slotsBuffer.setUsedRange(bufferRange);
			}
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
		//
		Iterator begin() {
			return Iterator(this, 0);
		}
		Iterator end() {
			return Iterator(this, count);
		}

	private:
		static constexpr size_t itemSize = sizeof(T);

		union {
			VirtualBuffer slotsBuffer;
			T* slots;
		};
		u32 count;
	};
}
