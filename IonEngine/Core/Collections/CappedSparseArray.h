#pragma once

namespace IonEngine {
	template<typename T, int N>
	class CappedSparseArray {
	public:
		struct Iterator {
			Iterator(const CappedSparseArray<T, N>* set, u32 pos) : set(set), pos(pos) {}

			bool operator!=(const Iterator& o) const {
				return pos != o.pos;
			}

			T& operator*() const {
				return const_cast<CappedSparseArray<T, N>*>(set)->slots[pos].item;
			}

			const Iterator& operator++() {
				pos++;
				while (set->isHole(pos - 1) && pos < N) {
					pos++;
				}
				return *this;
			}

		private:
			const CappedSparseArray<T, N>* set;
			u32 pos;
		};

		/* ==== CONSTRUCTORS ==== */
		//
		CappedSparseArray() : count(0) {

		}
		~CappedSparseArray() {

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
			while (isHole(startIndex) && startIndex < usedRange - 1) {
				startIndex++;
			}
			return Iterator(this, startIndex);
		}
		Iterator end() const {
			return Iterator(this, usedRange);
		}

	private:
		static constexpr u32 bitFieldSize = (N / 32) + ((N % 32 != 0) ? 1 : 0);

		T slots[N];
		u32 holeBitField[bitFieldSize];
		u32 count;

		inline void fieldSetHole(u32 index) {
			holeBitField[index / 32] |= 1 << (index % 32);
		}

		inline void fieldSetItem(u32 index) {
			holeBitField[index / 32] &= ~(1 << (index % 32));
		}

		bool isHole(u32 index) const {
			return (holeBitField[index / 32] & (1 << (index % 32))) != 0;
		}
	};
}
