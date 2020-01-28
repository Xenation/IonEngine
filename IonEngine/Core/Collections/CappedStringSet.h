#pragma once
#include <initializer_list>
#include "CappedDenseSet.h"

namespace IonEngine {
	template<u32 N>
	class CappedStringSet {
	public:
		CappedStringSet() {}
		CappedStringSet(std::initializer_list<const char*> list) {
			addAll(list.begin(), list.size());
		}
		CappedStringSet(const CappedStringSet&) = delete;
		void operator=(const CappedStringSet&) = delete;

		/* ==== METHODS ==== */
		/* Properties */
		//
		inline u32 getCount() const { return ids.count(); }
		constexpr u32 getCapacity() const { return N; }
		inline bool isEmpty() const { return getCount() == 0; }

		/* Modification */
		//
		void add(const char* str) {
			strId id = hashString(str, getLength(str));
			for (strId oid : ids) {
				if (oid == id) { // Duplicate found, don't add
					return;
				}
			}
			strings.add(str);
			ids.add(id);
		}
		void addAll(const char** strs, u32 count) {
			for (int i = 0; i < count; i++) {
				add(strs[i]);
			}
		}
		void remove(const char* str) {
			strId id = hashString(str, getLength(str));
			for (u32 i = 0; i < ids.getCount(); i++) {
				if (ids[i] == id) { // Entry found
					strings.removeAt(i);
					ids.removeAt(i);
					return;
				}
			}
		}
		void clear() {
			strings.clear();
			ids.clear();
		}

		/* Access */
		inline const char** data() const { return reinterpret_cast<const char**>(&strings); }

	private:
		CappedDenseSet<const char*, N> strings;
		CappedDenseSet<strId, N> ids;

		static u32 getLength(const char* str) {
			u32 i = 0;
			while (str[i] != '\0' && i < 2048) { // Hard limit to avoid inf loop
				i++;
			}
			return i;
		}
	};
}
