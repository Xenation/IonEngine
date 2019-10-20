#pragma once
#include <string>

namespace IonEngine {
	class AtomicCounterBuffer {
	public:
		const std::string name;

		AtomicCounterBuffer(std::string name, unsigned int binding, unsigned int size);
		~AtomicCounterBuffer();

		void bind();
		void clear();
		void allocateLocal();
		void deleteLocal();
		void uploadToGL();
		void deleteFromGL();


	private:
		unsigned int acbo = 0;
		unsigned int binding = 0;
		unsigned char* buffer = nullptr;
		unsigned int bufferSize = 0;
		bool loadedToGL = false;
	};
}
