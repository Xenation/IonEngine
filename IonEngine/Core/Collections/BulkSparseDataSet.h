#pragma once
#include "CollectionParams.h"
#include "VirtualBuffer.h"

namespace IonEngine {
	// A Data (no known type) Set with the following specs:
	// - dynamic allocation size
	// - guaranties locality
	// - does NOT guaranty contiguity
	// (designed for large amounts of elements of type not known at compile time)
	class BulkSparseDataSet {
	public:
		struct Iterator {
			Iterator(const BulkSparseDataSet* set, u32 pos) : set(set), pos(pos) {}

			bool operator!=(const Iterator& o) const {
				return pos != o.pos;
			}

			u8* operator*() const {
				return const_cast<BulkSparseDataSet*>(set)->data + pos * set->itemSize;
			}

			const Iterator& operator++() {
				pos++;
				while (set->isHole(pos - 1) && pos < set->usedRange) {
					pos++;
				}
				return *this;
			}

		private:
			const BulkSparseDataSet* set;
			u32 pos;
		};

		/* ==== CONSTRUCTORS ==== */
		// Creates a new BulkSparseSet
		BulkSparseDataSet(size_t itemSize) : count(0), usedRange(1), itemSize(itemSize), slotSize((itemSize < 4) ? 4 : itemSize), slotsBuffer(ION_BULK_VIRT_ALLOC_SIZE), holeFieldBuffer(getRequiredHoleFieldSize(ION_BULK_VIRT_ALLOC_SIZE)) {
			slotsBuffer.setUsedRange(usedRange * slotSize);
			holeFieldBuffer.setUsedRange(getRequiredHoleFieldSize(usedRange) * 4);
		}
		~BulkSparseDataSet() {
			// The buffer destructors will handle freeing
		}
		BulkSparseDataSet(const BulkSparseDataSet&) = delete;
		void operator=(const BulkSparseDataSet&) = delete;

		/* ==== METHODS ==== */
		/* Properties */
		// Returns the total count of items
		inline u32 getCount() const { return count; }
		// Returns true if there are no items in the set
		inline bool isEmpty() const { return count == 0; }

		/* Access */
		//
		template<typename T>
		T* at(u32 index) {
			return reinterpret_cast<T*>(data + index * itemSize);
		}
		template<>
		u8* at(u32 index) {
			return data + index * itemSize;
		}

		/* Modification */
		// Allocates a slot in the set to host an item
		u8* allocate() {
			u32 index = *at<u32>(0);
			*at<u32>(0) = *at<u32>(index);

			count++;
			if (index != 0) { // Free slot found
				fieldSetItem(index - 1);
				return at<u8>(index);
			}

			// Get from the end
			size_t bufferRange = slotsBuffer.getUsedRange();
			usedRange++;
			if (usedRange * itemSize > bufferRange) {
				bufferRange = usedRange * slotSize;
				slotsBuffer.setUsedRange(bufferRange);
				holeFieldBuffer.setUsedRange(getRequiredHoleFieldSize(usedRange) * 4);
			}
			fieldSetItem(usedRange - 2);
			return at<u8>(usedRange - 1);
		}

		// Removes the item at the given index (no destructor will be called)
		void removeAt(u32 index) {
			*at<u32>(index + 1) = *at<u32>(0);
			*at<u32>(0) = index + 1;
			count--;
			fieldSetHole(index);
		}

		// Remove the given item (the pointer must point to an item in the set, no destructor will be called)
		void remove(u8* item) {
			u32 absIndex = static_cast<u32>(item - data) / itemSize;
			*reinterpret_cast<u32*>(item) = *at<u32>(0);
			*at<u32>(0) = absIndex;
			count--;
			fieldSetHole(absIndex - 1);
		}

		// Clears the array (no destructor will be called)
		void clear(bool freeMemory = false) {
			usedRange = 1;
			count = 0;
			*at<u32>(0) = 0;
			if (freeMemory) {
				size_t bufferRange = usedRange * slotSize;
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
		// item at index 0 is the freelist head
		union {
			VirtualBuffer slotsBuffer;
			u8* data;
		};
		// bit field, 0 means hole, 1 means item
		union {
			VirtualBuffer holeFieldBuffer;
			u32* holeField;
		};
		u32 count;
		u32 usedRange;

		const size_t itemSize;
		const size_t slotSize;
		

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
