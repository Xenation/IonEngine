#pragma once
#include "CollectionParams.h"
#include "VirtualBuffer.h"

namespace IonEngine {
	// A Set with the following specs:
	// - dynamic allocation size
	// - guaranties locality
	// - does NOT guaranty contiguity
	// (designed for large sizes)
	template<typename T>
	class BulkSparseSet {
	public:
		struct Iterator {
			Iterator(const BulkSparseSet<T>* set, u32 pos) : set(set), pos(pos) {}

			bool operator!=(const Iterator& o) const {
				return pos != o.pos;
			}

			T& operator*() const {
				return const_cast<BulkSparseSet<T>*>(set)->slots[pos].item;
			}

			const Iterator& operator++() {
				pos++;
				while (set->isHole(pos - 1) && pos < set->usedRange) {
					pos++;
				}
				return *this;
			}

		private:
			const BulkSparseSet<T>* set;
			u32 pos;
		};

		/* ==== CONSTRUCTORS ==== */
		// Creates a new BulkSparseSet
		BulkSparseSet() : count(0), usedRange(1), slotsBuffer(BULK_VIRT_ALLOC_SIZE), holeFieldBuffer(getRequiredHoleFieldSize(BULK_VIRT_ALLOC_SIZE)) {
			slotsBuffer.setUsedRange(usedRange * itemSize);
			holeFieldBuffer.setUsedRange(getRequiredHoleFieldSize(usedRange) * 4);
		}
		~BulkSparseSet() {
			// The buffer destructors will handle freeing
		}
		BulkSparseSet(const BulkSparseSet&) = delete;
		void operator=(const BulkSparseSet&) = delete;

		/* ==== METHODS ==== */
		/* Properties */
		// Returns the total count of items
		inline u32 getCount() const { return count; }
		// Returns true if there are no items in the set
		inline bool isEmpty() const { return count == 0; }

		/* Modification */
		// Adds the given item by copying its data to the set
		// !!!Beware that it means no constructor is called but problems
		// mights arise when the given item is destroyed!!! (prefer using `allocate()` with placement new)
		T* add(const T& item) {
			T* nItem = allocate();
			memcpy_s(reinterpret_cast<void* const>(nItem), sizeof(T), reinterpret_cast<const void* const>(&item), sizeof(T));
			return nItem;
		}

		// Allocates a slot in the set to host an item
		T* allocate() {
			u32 index = slots[0].nextHole;
			slots[0].nextHole = slots[index].nextHole;

			count++;
			if (index != 0) { // Free slot found
				fieldSetItem(index - 1);
				return &(slots[index].item);
			}

			// Get from the end
			size_t bufferRange = slotsBuffer.getUsedRange();
			usedRange++;
			if (usedRange * itemSize > bufferRange) {
				bufferRange = usedRange * itemSize;
				slotsBuffer.setUsedRange(bufferRange);
				holeFieldBuffer.setUsedRange(getRequiredHoleFieldSize(usedRange) * 4);
			}
			fieldSetItem(usedRange - 2);
			return &(slots[usedRange - 1].item);
		}

		// Removes the item at the given index (no destructor will be called)
		void removeAt(u32 index) {
			slots[index + 1].nextHole = slots[0].nextHole;
			slots[0].nextHole = index + 1;
			count--;
			fieldSetHole(index);
		}

		// Remove the given item (the pointer must point to an item in the set, no destructor will be called)
		void remove(T* item) {
			ItemSlot* slot = reinterpret_cast<ItemSlot*>(item);
			u32 absIndex = static_cast<u32>(slot - slots);
			slot->nextHole = slots[0].nextHole;
			slots[0].nextHole = absIndex;
			count--;
			fieldSetHole(absIndex - 1);
		}

		// Clears the array (no destructor will be called)
		void clear(bool freeMemory = false) {
			usedRange = 1;
			count = 0;
			slots[0].nextHole = 0;
			if (freeMemory) {
				size_t bufferRange = usedRange * itemSize;
				slotsBuffer.setUsedRange(bufferRange);
				holeFieldBuffer.setUsedRange(getRequiredHoleFieldSize(usedRange) * 4);
			}
		}

		/* Iterator */
		// The begining of the set
		Iterator begin() const {
			u32 startIndex = 0;
			while (isHole(startIndex) && startIndex < usedRange - 1) {
				startIndex++;
			}
			return Iterator(this, startIndex + 1);
		}
		// The end of the set
		Iterator end() const {
			return Iterator(this, usedRange);
		}

	private:
		union ItemSlot {
			T item;
			u32 nextHole;
		};
		static constexpr size_t itemSize = sizeof(ItemSlot);

		// item at index 0 is the freelist head
		union {
			VirtualBuffer slotsBuffer;
			ItemSlot* slots;
		};
		// bit field, 0 means hole, 1 means item
		union {
			VirtualBuffer holeFieldBuffer;
			u32* holeField;
		};
		u32 count;
		u32 usedRange;

		inline static u32 getRequiredHoleFieldSize(u32 range) {
			return ((range - 1) / 32) + (((range - 1) % 32 != 0) ? 1 : 0);
		}

		inline void fieldSetHole(u32 index) {
			holeField[index / 32] &= ~(1 << (index % 32));
		}

		inline void fieldSetItem(u32 index) {
			holeField[index / 32] |= 1 << (index % 32);
		}

		bool isHole(u32 index) const {
			return (holeField[index / 32] & (1 << (index % 32))) == 0;
		}

	};
}
