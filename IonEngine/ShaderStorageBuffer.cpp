#include "ShaderStorageBuffer.h"

#include <gl3w.h>
#include "GLUtils.h"
#include "Debug.h"
using namespace IonEngine;



ShaderStorageBuffer::ShaderStorageBuffer(std::string name) : name(name) {

}

ShaderStorageBuffer::~ShaderStorageBuffer() {
	if (blocks != nullptr) {
		delete blocks;
	}
}


void ShaderStorageBuffer::setBlocks(unsigned int blockCount, ShaderStorageBlock* blocks) {
	unsigned int currentOffset = 0;
	for (unsigned int i = 0; i < blockCount; i++) {
		blocks[i].offset = currentOffset;
		currentOffset += blocks[i].size;
		currentOffset = glGetStorageBlockAlignment(currentOffset);
	}

	unsigned int nBufferSize = blocks[blockCount - 1].offset + blocks[blockCount - 1].size;
	unsigned char* nBuffer = nullptr;
	bool resized = false;
	if (nBufferSize != bufferSize) {
		nBuffer = new unsigned char[nBufferSize];
		resized = true;
	} else {
		nBuffer = buffer;
	}

	for (unsigned int i = 0; i < blockCount; i++) {
		blocks[i].buffer = nBuffer + blocks[i].offset;
		// TODO copy data from old buffer
	}

	if (this->blocks != nullptr) {
		delete this->blocks;
	}
	this->blockCount = blockCount;
	this->blocks = blocks;

	if (resized) {
		if (buffer != nullptr) {
			delete[] buffer;
		}
		buffer = nBuffer;
		bufferSize = nBufferSize;
	}
}

ShaderStorageBlock& ShaderStorageBuffer::getStorageBlock(unsigned int index) {
	return blocks[index];
}

void ShaderStorageBuffer::updateStorageBlock(unsigned int index) {
	if (!loadedToGL) {
		Debug::logError("SSBO", "Tried updating a block from a buffer not yet uploaded!");
		return;
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, blocks[index].offset, blocks[index].size, blocks[index].buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ShaderStorageBuffer::bindStorageBlock(unsigned int index) {
	glBindBufferRange(GL_SHADER_STORAGE_BLOCK, blocks[index].binding, ssbo, blocks[index].offset, blocks[index].size);
}

void ShaderStorageBuffer::clearData() {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R32UI, GL_RED, GL_UNSIGNED_INT, nullptr);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ShaderStorageBuffer::uploadToGL() {
	if (ssbo == 0) {
		glGenBuffers(1, &ssbo);
		std::string fullName = "SSBO " + name;
		glObjectLabel(GL_BUFFER, ssbo, (GLsizei) fullName.size(), fullName.c_str());
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	for (unsigned int i = 0; i < blockCount; i++) {
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, blocks[i].binding, ssbo, blocks[i].offset, blocks[i].size);
	}

	loadedToGL = true;
}

void ShaderStorageBuffer::deleteFromGL() {
	if (ssbo != 0) {
		glDeleteBuffers(1, &ssbo);
		ssbo = 0;
	}

	loadedToGL = false;
}
