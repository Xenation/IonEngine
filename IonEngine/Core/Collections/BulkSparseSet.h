#pragma once

namespace IonEngine {
	// A Set with the following specs:
	// - dynamic allocation size
	// - guaranties locality
	// - does NOT guaranty contiguity
	// (designed for large sizes)
	template<typename T>
	class BulkSparseSet {
	public:
		BulkSparseSet(const BulkSparseSet&) = delete;
		void operator=(const BulkSparseSet&) = delete;

		// TODO implement
	};
}
