#pragma once

namespace IonEngine {
	class VirtualBuffer final {
	public:

		/* ==== CONSTRUCTORS ==== */
		// Creates a buffer using virtual memory
		VirtualBuffer(size_t byteSize) : reservedSize(byteSize) {
			addr = reserveVirtualAddress(reservedSize);
		}
		// Frees the buffer memory and releases the address space
		~VirtualBuffer() {
			clear();
			releaseVirtualAddress(addr);
		}
		VirtualBuffer(const VirtualBuffer&) = delete;
		void operator=(const VirtualBuffer&) = delete;

		/* ==== METHODS ==== */
		// Set the range to be allocated
		// Will allocate or free to use the minimum required amount of pages
		inline void setUsedRange(size_t bytes) {
			byteRange = bytes;
			size_t nPageRange = getRequiredPageRange(byteRange);
			while (pageRange < nPageRange) {
				allocPage(pageRange);
				pageRange++;
			}
			while (pageRange > nPageRange) {
				pageRange--;
				freePage(pageRange);
			}
		}
		// Returns the range of allocated bytes
		inline size_t getUsedRange() {
			return byteRange;
		}
		// Frees all used memory, does not release address space
		inline void clear() {
			virtFree(addr, pageRange * virtPageSize);
			byteRange = 0;
			pageRange = 0;
		}

	private:
		union {
			void* addr;
			u8* byteAddr;
		};
		const size_t reservedSize;
		size_t byteRange;
		size_t pageRange;

		inline static size_t getRequiredPageRange(size_t byteRange) {
			return (byteRange / virtPageSize) + ((byteRange % virtPageSize != 0) ? 1 : 0);
		}

		inline void allocPage(size_t pageOffset) {
			virtAlloc(reinterpret_cast<void*>(byteAddr + virtPageSize * pageOffset), virtPageSize);
		}

		inline void freePage(size_t pageOffset) {
			virtFree(reinterpret_cast<void*>(byteAddr + virtPageSize * pageOffset));
		}

	};
}
