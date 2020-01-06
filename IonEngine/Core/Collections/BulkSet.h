#pragma once
#include "CollectionParams.h"

namespace IonEngine {
	// A collection that does not guaranty order or location but guaranties contiguity
	// i.e.: good for things that will not get pointed by a pointer
	template<typename T>
	class BulkSet {
	public:
		/* ==== CONSTRUCTORS ==== */
		BulkSet() {
			virtAddr = reserveVirtualAddress(BULK_VIRT_ALLOC_SIZE);
		}
		~BulkSet() {
			clear();
			releaseVirtualAddress(virtAddr);
		}
		BulkSet(const BulkSet&) = delete;
		void operator=(const BulkSet&) = delete;


		/* ==== METHODS ==== */
		inline u32 getCount() { return count; }

		void add(T item) {
			count++;

			// Grow allocation
			u32 nPageCount = getRequiredPageCount();
			if (nPageCount > committedPages) {
				allocPage(committedPages);
				committedPages = nPageCount;
			}

			arr[count - 1] = item;
		}

		void removeAt(u32 idx) {
			if (idx >= count) return;
			count--;
			if (idx != count) {
				memcpy_s(reinterpret_cast<void*>(virtByteAddr + itemSize * idx), itemSize, reinterpret_cast<void*>(virtByteAddr + itemSize * count), itemSize);
			}

			// Shrink allocation
			u32 nPageCount = getRequiredPageCount();
			if (nPageCount < committedPages) {
				freePage(nPageCount);
				committedPages = nPageCount;
			}
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
			if (committedPages > 0) {
				virtFree(virtAddr, committedPages * virtPageSize);
			}
			count = 0;
			committedPages = 0;
		}

		inline T& operator[](u32 index) {
			return arr[index];
		}


	private:
		u32 count = 0;
		u32 committedPages = 0;
		static constexpr size_t itemSize = sizeof(T);
		
		union {
			void* virtAddr = nullptr;
			u8* virtByteAddr;
			T* arr;
		};

		u32 getRequiredPageCount() {
			size_t effectiveSize = count * itemSize;
			return effectiveSize / virtPageSize + ((effectiveSize % virtPageSize != 0) ? 1 : 0);
		}

		inline void allocPage(u32 pageOffset) {
			virtAlloc(reinterpret_cast<void*>(virtByteAddr + virtPageSize * pageOffset), virtPageSize);
		}

		inline void freePage(u32 pageOffset) {
			virtFree(reinterpret_cast<void*>(virtByteAddr + virtPageSize * pageOffset));
		}
	};
}
