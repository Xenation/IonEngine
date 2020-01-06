#pragma once
#include "CollectionParams.h"

namespace IonEngine {
	// A collection that does not guaranty order and contiguity but guaranties location
	// i.e.: good for storing things that will get pointed by a pointer
	template<typename T>
	class BulkHollowSet {
		static constexpr u32 runLengthFlagMask = 0b10000000'00000000'00000000'00000000u;
		static constexpr u32 runLengthValueMask = ~runLengthFlagMask;
		union ItemSlot {
			T item;
			struct {
				u32 nextHole;
				//u32 runLength; // MSB: 0->hole, 1->items
			};

			/*bool isHoleLength() {
				return (runLength & runLengthFlagMask) == 0;
			}

			void setHoleLength(u32 length) {
				runLength = length & runLengthValueMask;
			}

			void setItemLength(u32 length) {
				runLength = (length & runLengthValueMask) | runLengthFlagMask;
			}*/
		};

	public:
		struct Iterator {
			Iterator(const BulkHollowSet<T>* set, u32 pos) : set(set), pos(pos), nextHole(set->slots[0].nextHole) {}

			bool operator!=(const Iterator& o) const {
				return pos != o.pos;
			}

			T& operator*() const {
				return set->slots[pos].item;
			}

			const Iterator& operator++() {
				pos++;
				while (pos == nextHole) {
					nextHole = set->slots[nextHole].nextHole;
					pos++;
				}
				return *this;
			}

		private:
			const BulkHollowSet<T>* set;
			u32 pos;
			u32 nextHole;
		};

		/* ==== CONSTRUCTORS ===== */
		BulkHollowSet() {
			virtAddr = reserveVirtualAddress(BULK_VIRT_ALLOC_SIZE);
			allocPage(0);
			slots[0].nextHole = 0;
		}
		~BulkHollowSet() {
			clear();
			virtFree(virtAddr, virtPageSize);
			releaseVirtualAddress(virtAddr);
		}
		BulkHollowSet(const BulkHollowSet&) = delete;
		void operator=(const BulkHollowSet&) = delete;


		/* ==== METHODS ==== */
		inline u32 getCount() { return count; }

		u32 indexOf(T* item) {
			return static_cast<u32>(reinterpret_cast<ItemSlot*>(item) - slots) - 1;
		}

		T* add(const T& item) {
			T* nItem = allocateSlot();
			memcpy_s(reinterpret_cast<void*>(nItem), sizeof(T), reinterpret_cast<const void*const>(&item), sizeof(T));
			return nItem;
		}

		T* allocateSlot() {
			u32 index = slots[0].nextHole;
			slots[0].nextHole = slots[index].nextHole;

			if (index != 0) { // Free slot found
				return &(slots[index].item);
			}

			// Get from the end
			capacity++;
			u32 pageCount = getRequiredPageCount();
			if (pageCount > committedPages) {
				allocPage(committedPages);
				committedPages = pageCount;
			}
			return reinterpret_cast<T*>(slots + capacity);
		}

		void removeAt(u32 index) {
			// Unordered freelist remove (breaks iteration support)
			//slots[index + 1].nextHole = slots[0].nextHole;
			//slots[0].nextHole = index + 1;
			// Ordered freelist remove
			u32 prevHole = getPreviousHole(index + 1);
			slots[index + 1].nextHole = slots[prevHole].nextHole;
			slots[prevHole].nextHole = index + 1;
		}

		void remove(T* item) {
			u32 index = indexOf(item);
			u32 prevHole = getPreviousHole(index + 1);
			ItemSlot* itemSlot = reinterpret_cast<ItemSlot*>(item);
			itemSlot->nextHole = slots[prevHole].nextHole;
			slots[prevHole].nextHole = index + 1;
		}

		void clear() {
			if (committedPages > 1) {
				virtFree(reinterpret_cast<void*>(virtByteAddr + virtPageSize), (committedPages - 1) * virtPageSize);
			}
			count = 0;
			capacity = 0;
			committedPages = 1;
		}

		Iterator begin() const {
			u32 startIndex = 0;
			while (slots[startIndex].nextHole == startIndex + 1) {
				startIndex = slots[startIndex].nextHole;
			}
			return Iterator(this, startIndex + 1);
		}

		Iterator end() const {
			return Iterator(this, capacity + 1);
		}


	private:
		u32 count = 0;
		u32 capacity = 0;
		u32 committedPages = 1;
		static constexpr size_t itemSize = sizeof(ItemSlot);

		union {
			void* virtAddr = nullptr;
			u8* virtByteAddr;
			ItemSlot* slots;
		};

		inline u32 getRequiredPageCount() {
			size_t effectiveSize = (capacity + 1) * itemSize;
			return effectiveSize / virtPageSize + ((effectiveSize % virtPageSize != 0) ? 1 : 0);
		}

		inline void allocPage(u32 pageOffset) {
			virtAlloc(reinterpret_cast<void*>(virtByteAddr + virtPageSize * pageOffset), virtPageSize);
		}

		inline void freePage(u32 pageOffset) {
			virtFree(reinterpret_cast<void*>(virtByteAddr + virtPageSize * pageOffset));
		}

		inline u32 getPreviousHole(u32 index) {
			u32 prevIndex = slots[0].nextHole;
			while (prevIndex > 0) {
				if (slots[prevIndex].nextHole > index) {
					break;
				}
				prevIndex = slots[prevIndex].nextHole;
			}
			return prevIndex;
		}
	};
}
