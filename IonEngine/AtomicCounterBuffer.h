#pragma once
#include <string>

namespace IonEngine {
	class AtomicCounterBuffer {
	public:
		const std::string name;

		AtomicCounterBuffer(std::string name, u32 binding, u32 size);
		~AtomicCounterBuffer();

		void bind();
		void clear();
		void allocateLocal();
		void deleteLocal();
		void uploadToGL();
		void deleteFromGL();


	private:
		u32 acbo = 0;
		u32 binding = 0;
		u8* buffer = nullptr;
		u32 bufferSize = 0;
		bool loadedToGL = false;
	};
}
