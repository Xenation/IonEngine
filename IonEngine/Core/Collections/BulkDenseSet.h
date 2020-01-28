#pragma once
#include "CollectionParams.h"

namespace IonEngine {
	// A Set with the following specs:
	// - dynamic allocation size
	// - guaranties contiguity
	// - does NOT guaranty locality
	// (designed for large sizes)
	template<typename T>
	class BulkDenseSet {
	public:
		struct Iterator {
			Iterator(const BulkDenseSet<T>* set, u32 pos) : set(set), pos(pos) {}

			bool operator!=(const Iterator& o) const {
				return pos != o.pos;
			}

			T& operator*() const {
				return const_cast<BulkDenseSet<T>*>(set)->slots[pos];
			}

			const Iterator& operator++() {
				pos++;
				return *this;
			}

		private:
			const BulkDenseSet<T>* set;
			u32 pos;
		};

		/* ==== CONSTRUCTORS ==== */
		// Creates a new BulkDenseSet, no allocation yet (virtual address space is reserved)
		BulkDenseSet() : count(0), pageCount(0) {
			virtAddr = reserveVirtualAddress(BULK_VIRT_ALLOC_SIZE);
		}
		// Destroys the set by clearing and freeing any allocated memory (virtual address space is released)
		~BulkDenseSet() {
			clear(true);
			releaseVirtualAddress(virtAddr);
		}
		BulkDenseSet(const BulkDenseSet&) = delete;
		void operator=(const BulkDenseSet&) = delete;

		/* ==== METHODS ==== */
		/* Properties */
		// Returns the total count of items
		inline u32 getCount() const { return count; }
		// Returns true if the set is empty (i.e. no items are stored)
		inline bool isEmpty() const { return count == 0; }

		/* Modification */
		// Adds the given item by copying its data to the set
		// !!!Beware that it means no constructor is called but problems
		// mights arise when the given item is destroyed!!! (prefer using `allocate()` with placement new)
		T* add(const T& item) {
			T* nItem = allocate();
			memcpy_s(reinterpret_cast<void* const>(nItem), itemSize, reinterpret_cast<const void* const>(&item), itemSize);
			return nItem;
		}

		// Allocates a slot in the set to host an item
		T* allocate() {
			count++;
			u32 nPageCount = getRequiredPageCount();
			while (pageCount < nPageCount) {
				allocPage(pageCount);
				pageCount++;
			}
			return slots + count - 1;
		}

		// Removes the item at the given index (no destructor will be called)
		void removeAt(u32 index) {
			memcpy_s(reinterpret_cast<void* const>(slots + index), itemSize, reinterpret_cast<const void* const>(slots + count - 1), itemSize);
			count--;
		}

		// Remove the given item (the pointer must point to an item in the set, no destructor will be called)
		void remove(T* item) {
			memcpy_s(reinterpret_cast<void* const>(item), itemSize, reinterpret_cast<const void* const>(slots + count - 1), itemSize);
			count--;
		}

		// Clears the array (no destructor will be called)
		// if freeMemory is true the allocated memory will be freed, otherwise nothing is freed
		void clear(bool freeMemory = false) {
			count = 0;
			if (freeMemory) {
				virtFree(virtAddr, pageCount * virtPageSize);
				pageCount = 0;
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
		// The begining of the set
		Iterator begin() const {
			return Iterator(this, 0);
		}
		// The end of the set
		Iterator end() const {
			return Iterator(this, count);
		}

	private:
		union {
			void* virtAddr;
			u8* virtByteAddr;
			T* slots;
		};
		u32 count;
		u32 pageCount;
		static constexpr size_t itemSize = sizeof(T);

		inline u32 getRequiredPageCount() {
			size_t effectiveSize = count * itemSize;
			return static_cast<u32>(effectiveSize / virtPageSize) + ((effectiveSize % virtPageSize != 0) ? 1 : 0);
		}

		inline void allocPage(u32 pageOffset) {
			virtAlloc(reinterpret_cast<void*>(virtByteAddr + virtPageSize * pageOffset), virtPageSize);
		}

		inline void freePage(u32 pageOffset) {
			virtFree(reinterpret_cast<void*>(virtByteAddr + virtPageSize * pageOffset));
		}
	};
}
