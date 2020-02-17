#pragma once
#include "CollectionParams.h"

namespace IonEngine {
	template<typename T>
	class BulkSparseArray {
	public:
		struct Iterator {
			Iterator(const BulkSparseArray<T>* set, u32 pos) : set(set), pos(pos) {}

			bool operator!=(const Iterator& o) const {
				return pos != o.pos;
			}

			T& operator*() const {
				return const_cast<BulkSparseArray<T>*>(set)->slots[pos].item;
			}

			const Iterator& operator++() {
				pos++;
				while (set->isHole(pos - 1) && pos < N) {
					pos++;
				}
				return *this;
			}

		private:
			const BulkSparseArray<T>* set;
			u32 pos;
		};

		/* ==== CONSTRUCTORS ==== */
		//
		BulkSparseArray() : count(0) {
			slotsAddr = reserveVirtualAddress(ION_BULK_VIRT_ALLOC_SIZE);
			holeBitFieldAddr = reserveVirtualAddress(getRequiredHoleFieldSize(ION_BULK_VIRT_ALLOC_SIZE));
		}
		~BulkSparseArray() {

		}

		/* ==== METHODS ==== */
		/* Properties */
		//
		inline u32 getCount() { return count; }
		constexpr u32 getCapacity() { return N; }
		inline bool isEmpty() { return count == 0; }

		/* Modification */
		//
		T* use(u32 index) {
			fieldSetItem(index);
			return at(index);
		}
		void unuse(u32 index) {
			fieldSetHole(index);
		}
		void clear() {
			for (u32 i = 0; i < bitFieldSize; i++) {
				holeBitField[i] = 0;
			}
		}

		/* Access */
		//
		T* at(u32 index) {
			return slots + index;
		}
		bool exists(u32 index) {
			return !isHole(index);
		}
		inline T& operator[](u32 index) {
			return slots[index];
		}
		inline T& operator[](i32 index) {
			return slots[index];
		}

		/* Iterator */
		//
		Iterator begin() const {
			u32 startIndex = 0;
			while (isHole(startIndex) && startIndex < N - 1) {
				startIndex++;
			}
			return Iterator(this, startIndex);
		}
		Iterator end() const {
			return Iterator(this, N);
		}

	private:
		static constexpr u32 maxPageCount = ION_BULK_VIRT_ALLOC_SIZE / virtPageSize + ((ION_BULK_VIRT_ALLOC_SIZE % virtPageSize != 0) ? 1 : 0);
		static constexpr u32 pageFieldSize = maxPageCount / 32 + ((maxPageCount % 32 != 0) ? 1 : 0);

		// TODO CONTINUE HERE!
		union {
			void* slotsAddr;
			T* slots;
		};
		union {
			void* holeBitFieldAddr;
			u32* holeBitField;
		};
		u32 pageBitField[pageFieldSize]; // TODO maybe use a more dynamic allocation to reduce memory cost

		u32 count;

		inline static u32 getRequiredHoleFieldSize(u32 range) {
			return ((range - 1) / 32) + (((range - 1) % 32 != 0) ? 1 : 0);
		}

		inline void fieldSetHole(u32 index) {
			holeBitField[index / 32] |= 1 << (index % 32);
		}

		inline void fieldSetItem(u32 index) {
			holeBitField[index / 32] &= ~(1 << (index % 32));
		}

		inline bool isHole(u32 index) const {
			return (holeBitField[index / 32] & (1 << (index % 32))) != 0;
		}

		inline void pageSetUsed(u32 pageIndex) {
			pageBitField[pageIndex / 32] |= 1 << (pageIndex % 32);
		}

		inline void pageSetUnused(u32 pageIndex) {
			pageBitField[pageIndex / 32] &= ~(1 << (pageIndex % 32));
		}

		inline bool pageIsUsed(u32 pageIndex) {
			return (pageBitField[pageIndex / 32] & (1 << (pageIndex % 32))) != 0;
		}
	};
}
