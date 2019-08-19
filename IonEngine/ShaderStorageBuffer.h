#pragma once
#include <string>

namespace IonEngine {

	struct ShaderStorageBlock {
		unsigned int binding;
		unsigned int offset;
		unsigned int size;
		unsigned char* buffer;

	public:
		ShaderStorageBlock(unsigned int binding, unsigned int offset, unsigned int size) : binding(binding), offset(offset), size(size) {}
		~ShaderStorageBlock() {}
	};

	class ShaderStorageBuffer {
	public:
		const std::string name;

		ShaderStorageBuffer(std::string name, unsigned int size);
		~ShaderStorageBuffer();

		void setBlocks(unsigned int blockCount, ShaderStorageBlock* blocks);
		ShaderStorageBlock& getStorageBlock(unsigned int index);
		void updateStorageBlock(unsigned int index);
		void bindStorageBlock(unsigned int index);
		void clearData();
		void allocateLocal();
		void deleteLocal();
		void uploadToGL();
		void deleteFromGL();

	private:
		unsigned int ssbo = 0;
		ShaderStorageBlock* blocks = nullptr;
		unsigned int blockCount = 0;
		unsigned char* buffer = nullptr;
		unsigned int bufferSize = 0;
		bool loadedToGL = false;
	};
}
