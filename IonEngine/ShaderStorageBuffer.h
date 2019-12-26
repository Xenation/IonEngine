#pragma once
#include <string>

namespace IonEngine {

	struct ShaderStorageBlock {
		u32 binding;
		u32 offset;
		u32 size;
		u8* buffer;

	public:
		ShaderStorageBlock(u32 binding, u32 offset, u32 size) : binding(binding), offset(offset), size(size), buffer(nullptr) {}
		~ShaderStorageBlock() {}
	};

	class ShaderStorageBuffer {
	public:
		const std::string name;

		ShaderStorageBuffer(std::string name);
		~ShaderStorageBuffer();

		void setBlocks(u32 blockCount, ShaderStorageBlock* blocks);
		ShaderStorageBlock& getStorageBlock(u32 index);
		void updateStorageBlock(u32 index);
		void bindStorageBlock(u32 index);
		void clearData();
		void uploadToGL();
		void deleteFromGL();

	private:
		u32 ssbo = 0;
		ShaderStorageBlock* blocks = nullptr;
		u32 blockCount = 0;
		u8* buffer = nullptr;
		u32 bufferSize = 0;
		bool loadedToGL = false;
	};
}
