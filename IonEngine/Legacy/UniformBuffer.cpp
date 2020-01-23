#include "UniformBuffer.h"

#include <string>
#include "Debug.h"
using namespace IonEngine;



UniformLayout::UniformLayout(u32 binding, ShaderUniformBufferLayoutFieldInfo* bufferField)
	: UniformLayout(binding, bufferField->layoutType, bufferField->subFieldCount, bufferField->getMembersTypes()) {
	memberNames = new std::string[bufferField->subFieldCount];
	for (u32 i = 0; i < bufferField->subFieldCount; i++) {
		memberNames[i] = bufferField->subFields[i]->name;
	}
	memberArraySizes = new u32[bufferField->subFieldCount];
	for (u32 i = 0; i < bufferField->subFieldCount; i++) {
		if (bufferField->subFields[i]->fieldType == ShaderFieldType::NativeTypeArray) {
			memberArraySizes[i] = ((ShaderNativeTypeArrayFieldInfo*) bufferField->subFields[i])->arraySize;
		} else {
			memberArraySizes[i] = 0;
		}
	}
}

UniformLayout::UniformLayout(u32 binding, u32 memberCount, GLSLType* members)
	: UniformLayout(binding, UniformLayoutType::STD140, memberCount, members) {

}

UniformLayout::UniformLayout(u32 binding, UniformLayoutType type, u32 memberCount, GLSLType* members)
	: binding(binding), type(type), memberCount(memberCount), members(members) {
	membersOffsets = new u32[memberCount];
}

UniformLayout::~UniformLayout() {
	if (members != nullptr) {
		delete[] members;
		members = nullptr;
	}
	if (membersOffsets != nullptr) {
		delete[] membersOffsets;
		membersOffsets = nullptr;
	}
}

void UniformLayout::setMember(u32 index, bool value) {
	unsigned char bytes[4]{value, 0, 0, 0}; // Required because booleans are 4bytes in glsl
	setMember(index, bytes, 4);
}
void UniformLayout::setMember(u32 index, i32 value) {
	setMember(index, (u8*) &value, 4);
}
void UniformLayout::setMember(u32 index, u32 value) {
	setMember(index, (u8*) &value, 4);
}
void UniformLayout::setMember(u32 index, float value) {
	setMember(index, (u8*) &value, 4);
}
void UniformLayout::setMember(u32 index, double value) {
	setMember(index, (u8*) &value, 8);
}
void UniformLayout::setMember(u32 index, Vec2f value) {
	setMember(index, (u8*) &value, 8);
}
void UniformLayout::setMember(u32 index, Vec3f value) {
	setMember(index, (u8*) &value, 12);
}
void UniformLayout::setMember(u32 index, Vec4f value) {
	setMember(index, (u8*) &value, 16);
}
void UniformLayout::setMember(u32 index, Vec2i value) {
	setMember(index, (u8*) &value, 8);
}
void UniformLayout::setMember(u32 index, Vec3i value) {
	setMember(index, (u8*) &value, 12);
}
void UniformLayout::setMember(u32 index, Vec4i value) {
	setMember(index, (u8*) &value, 16);
}
void UniformLayout::setMember(u32 index, Matrix4x4f value) {
	setMember(index, (u8*) &value, 64);
}
void UniformLayout::setMember(u32 index, bool value, u32 arrayIndex) {
	u8 bytes[4]{value, 0, 0, 0}; // Required because booleans are 4bytes in glsl
	setMember(index, arrayIndex, bytes, 4);
}
void UniformLayout::setMember(u32 index, i32 value, u32 arrayIndex) {
	setMember(index, arrayIndex, (u8*) &value, 4);
}
void UniformLayout::setMember(u32 index, u32 value, u32 arrayIndex) {
	setMember(index, arrayIndex, (u8*) &value, 4);
}
void UniformLayout::setMember(u32 index, float value, u32 arrayIndex) {
	setMember(index, arrayIndex, (u8*) &value, 4);
}
void UniformLayout::setMember(u32 index, double value, u32 arrayIndex) {
	setMember(index, arrayIndex, (u8*) &value, 8);
}
void UniformLayout::setMember(u32 index, Vec2f value, u32 arrayIndex) {
	setMember(index, arrayIndex, (u8*) &value, 8);
}
void UniformLayout::setMember(u32 index, Vec3f value, u32 arrayIndex) {
	setMember(index, arrayIndex, (u8*) &value, 12);
}
void UniformLayout::setMember(u32 index, Vec4f value, u32 arrayIndex) {
	setMember(index, arrayIndex, (u8*) &value, 16);
}
void UniformLayout::setMember(u32 index, Vec2i value, u32 arrayIndex) {
	setMember(index, arrayIndex, (u8*) &value, 8);
}
void UniformLayout::setMember(u32 index, Vec3i value, u32 arrayIndex) {
	setMember(index, arrayIndex, (u8*) &value, 12);
}
void UniformLayout::setMember(u32 index, Vec4i value, u32 arrayIndex) {
	setMember(index, arrayIndex, (u8*) &value, 16);
}
void UniformLayout::setMember(u32 index, Matrix4x4f value, u32 arrayIndex) {
	setMember(index, arrayIndex, (u8*) &value, 64);
}

void UniformLayout::setMember(u32 index, u8* bytes, u32 byteSize) {
	for (u32 i = 0; i < byteSize; i++) {
		buffer[membersOffsets[index] + i] = bytes[i];
	}
}

void UniformLayout::setMember(u32 index, u32 arrayIndex, u8* bytes, u32 byteSize) {
	// TODO not flexible, expects a std140 layout
	u32 stride = byteSize;
	if (stride % 16 != 0) {
		stride += 16 - stride % 16;
	}
	for (u32 i = 0; i < byteSize; i++) {
		buffer[membersOffsets[index] + stride * arrayIndex + i] = bytes[i];
	}
}

void UniformLayout::copyFrom(UniformLayout& layout) {
	for (u32 i = 0; i < memberCount; i++) {
		int toCopy = layout.indexOf(memberNames[i]);
		if (toCopy != -1) {
			// TODO perform type conversions when necessary (int to float, ...)
			setMember(toCopy, layout.getBytes(toCopy), glslTypeSize(layout.members[toCopy]));
		} else {
			// Init at zero
			u32 size = glslTypeSize(members[i]);
			for (u32 bi = 0; bi < membersOffsets[i] + size; bi++) {
				buffer[bi] = 0;
			}
		}
	}
}

int UniformLayout::indexOf(std::string memberName) const {
	for (u32 i = 0; i < memberCount; i++) {
		if (memberName == memberNames[i]) {
			return i;
		}
	}
	return -1;
}

u8* UniformLayout::getBytes(u32 index) const {
	return &buffer[membersOffsets[index]];
}

u8* UniformLayout::getBytes(u32 index, u32 arrayIndex) const {
	return &buffer[membersOffsets[index] + memberArraySizes[index] * 16]; // TODO unsafe, expects array elem stride to be 16 (vec4) but might be bigger
}

void UniformLayout::computeLayoutOffsets() {
	u32 currentOffset = 0;
	switch (type) {
	case UniformLayoutType::STD140:
		for (u32 i = 0; i < memberCount; i++) {
			u32 baseAlignment;
			if (memberArraySizes != nullptr && memberArraySizes[i] != 0) {
				baseAlignment = glslTypeBaseAlignment(members[i], memberArraySizes[i]);
			} else {
				baseAlignment = glslTypeBaseAlignment(members[i]);
			}
			u32 unaligment = currentOffset % baseAlignment;
			if (unaligment > 0) {
				currentOffset += baseAlignment - unaligment;
			}
			membersOffsets[i] = currentOffset;
			u32 memberSize = glslstd140TypeSize(members[i], (memberArraySizes != nullptr) ? memberArraySizes[i] : 0);
			currentOffset += memberSize;
		}
		size = currentOffset;
		break;
	default:
		Debug::logError("UniformBuffer", "Unsupported layout type!");
		break;
	}
}



UniformBuffer::UniformBuffer(std::string name) : name(name) {}

UniformBuffer::~UniformBuffer() {
	if (layouts != nullptr) {
		delete[] layouts;
		layouts = nullptr;
	}
	if (buffer != nullptr) {
		delete[] buffer;
		buffer = nullptr;
	}
	deleteFromGL();
}


void UniformBuffer::setLayouts(u32 layoutCount, UniformLayout* layouts) {
	u32 currentOffset = 0;
	for (u32 i = 0; i < layoutCount; i++) {
		layouts[i].offset = currentOffset;
		layouts[i].computeLayoutOffsets();
		currentOffset += layouts[i].size;
		currentOffset = glGetUniformBufferAlignment(currentOffset);
	}

	u32 nBufferSize = layouts[layoutCount - 1].offset + layouts[layoutCount - 1].size;
	u8* nBuffer = nullptr;
	bool resized = false;
	if (nBufferSize != bufferSize) {
		nBuffer = new u8[nBufferSize];
		resized = true;
	} else {
		nBuffer = buffer;
	}

	for (u32 i = 0; i < layoutCount; i++) {
		layouts[i].buffer = nBuffer + layouts[i].offset;
		UniformLayout* existingLayout = findLayout(layouts[i].binding);
		if (existingLayout != nullptr) {
			layouts[i].copyFrom(*existingLayout);
		}
	}

	if (this->layouts != nullptr) {
		delete[] this->layouts;
	}
	this->layoutCount = layoutCount;
	this->layouts = layouts;

	if (resized) {
		if (buffer != nullptr) {
			delete[] buffer;
		}
		buffer = nBuffer;
		bufferSize = nBufferSize;
	}
}

UniformLayout& UniformBuffer::getLayout(u32 index) {
	return layouts[index];
}

UniformLayout* UniformBuffer::findLayout(GLuint binding) {
	for (u32 i = 0; i < layoutCount; i++) {
		if (layouts[i].binding == binding) return &layouts[i];
	}
	return nullptr;
}

void UniformBuffer::updateLayout(u32 index) { // TODO make a version of this function to update a single member of a layout
	if (!loadedToGL) {
		Debug::logError("UniformBuffer", "Trying to update a layout from a buffer not yet uploaded!");
		return;
	}
	u32 offset = layouts[index].offset;
	u32 size = layouts[index].size;
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, layouts[index].buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::bindLayout(u32 index) {
	if (!loadedToGL) {
		Debug::logError("UniformBuffer", "Trying to bind a layout from a buffer not yet uploaded!");
		return;
	}
	glBindBufferRange(GL_UNIFORM_BUFFER, layouts[index].binding, ubo, layouts[index].offset, layouts[index].size);
}

void UniformBuffer::uploadToGL() {
	if (ubo == 0) {
		glGenBuffers(1, &ubo);
		std::string fullName = "UBO " + name;
		glObjectLabel(GL_BUFFER, ubo, (GLsizei) fullName.size(), fullName.c_str());
	}
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, bufferSize, buffer, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	for (u32 i = 0; i < layoutCount; i++) {
		glBindBufferRange(GL_UNIFORM_BUFFER, layouts[i].binding, ubo, layouts[i].offset, layouts[i].size);
	}

	loadedToGL = true;
}

void UniformBuffer::deleteFromGL() {
	if (ubo != 0) {
		glDeleteBuffers(1, &ubo);
		ubo = 0;
	}
	loadedToGL = false;
}
