#include "Mesh.h"

#include <string>
#include "GLUtils.h"
#include "Debug.h"



unsigned int Mesh::triangleCount = 0;


Mesh::Mesh(int vCount, int iCount) : Mesh("", vCount, iCount) {}

Mesh::Mesh(std::string name, int vCount, int iCount)
	: name(name), vertexCount(vCount), indexCount(iCount) {
	indices = nullptr;
}

Mesh::~Mesh() {
	if (loadedToGL) {
		deleteFromGL();
	}
	if (cachedInLocal) {
		deleteLocal();
	}
}

void Mesh::setAttributesDefinition(int count, int* sizes) {
	GLenum* defTypes = new GLenum[count];
	for (int i = 0; i < count; i++) {
		defTypes[i] = GL_FLOAT;
	}
	setAttributesDefinition(count, sizes, defTypes);
}

void Mesh::setAttributesDefinition(int count, int* sizes, GLenum* types) {
	attributeCount = count;
	attributeSizes = sizes;
	attributeTypes = types;
	attributeByteOffsets = new unsigned int[count];
	vertexByteSize = 0;
	for (int i = 0; i < attributeCount; i++) {
		attributeByteOffsets[i] = vertexByteSize;
		vertexByteSize += attributeSizes[i] * glTypeSize(types[i]);
	}
	if (vertices != nullptr) {
		delete[] vertices;
	}
	vertices = new char[vertexCount * vertexByteSize];
	cachedInLocal = true;
}

void Mesh::setAttribute(int index, signed char* values) {
	if (attributeTypes[index] != GL_BYTE) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_BYTE!").c_str());
		return;
	}
	setAttributeData(index, (char*) values);
}

void Mesh::setAttribute(int index, char* values) {
	if (attributeTypes[index] != GL_UNSIGNED_BYTE) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_UNSIGNED_BYTE!").c_str());
		return;
	}
	setAttributeData(index, (char*) values);
}

void Mesh::setAttribute(int index, short* values) {
	if (attributeTypes[index] != GL_SHORT) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_SHORT!").c_str());
		return;
	}
	setAttributeData(index, (char*) values);
}

void Mesh::setAttribute(int index, unsigned short* values) {
	if (attributeTypes[index] != GL_UNSIGNED_SHORT || attributeTypes[index] != GL_HALF_FLOAT) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_UNSIGNED_SHORT or GL_HALF_FLOAT!").c_str());
		return;
	}
	setAttributeData(index, (char*) values);
}

void Mesh::setAttribute(int index, int* values) {
	if (attributeTypes[index] != GL_INT) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_INT!").c_str());
		return;
	}
	setAttributeData(index, (char*) values);
}

void Mesh::setAttribute(int index, unsigned int* values) {
	if (attributeTypes[index] != GL_UNSIGNED_INT) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_UNSIGNED_INT!").c_str());
		return;
	}
	setAttributeData(index, (char*) values);
}

void Mesh::setAttribute(int index, float* values) {
	if (attributeTypes[index] != GL_FLOAT) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_FLOAT!").c_str());
		return;
	}
	setAttributeData(index, (char*) values);
}

void Mesh::setAttribute(int index, double* values) {
	if (attributeTypes[index] != GL_DOUBLE) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_DOUBLE!").c_str());
		return;
	}
	setAttributeData(index, (char*) values);
}

void Mesh::setAttributeData(int index, char* bytes) {
	int attrByteOffset = attributeByteOffsets[index];
	int attrSize = attributeSizes[index];
	int typeSize = glTypeSize(attributeTypes[index]);
	for (int vi = 0; vi < vertexCount; vi++) {
		for (int i = 0; i < attrSize; i++) {
			for (int bi = 0; bi < typeSize; bi++) {
				((char*) vertices)[vi * vertexByteSize + attrByteOffset + i * typeSize + bi] = bytes[vi * attrSize * typeSize + i * typeSize + bi];
			}
		}
	}
}

void Mesh::setAttributeElement(int attrIndex, int elemIndex, signed char value) {
	setAttributeElementData(attrIndex, elemIndex, (unsigned char*) &value);
}

void Mesh::setAttributeElement(int attrIndex, int elemIndex, char value) {
	setAttributeElementData(attrIndex, elemIndex, (unsigned char*) &value);
}

void Mesh::setAttributeElement(int attrIndex, int elemIndex, short value) {
	setAttributeElementData(attrIndex, elemIndex, (unsigned char*) &value);
}

void Mesh::setAttributeElement(int attrIndex, int elemIndex, unsigned short value) {
	setAttributeElementData(attrIndex, elemIndex, (unsigned char*) &value);
}

void Mesh::setAttributeElement(int attrIndex, int elemIndex, int value) {
	setAttributeElementData(attrIndex, elemIndex, (unsigned char*) &value);
}

void Mesh::setAttributeElement(int attrIndex, int elemIndex, unsigned int value) {
	setAttributeElementData(attrIndex, elemIndex, (unsigned char*) &value);
}

void Mesh::setAttributeElement(int attrIndex, int elemIndex, float value) {
	setAttributeElementData(attrIndex, elemIndex, (unsigned char*) &value);
}

void Mesh::setAttributeElement(int attrIndex, int elemIndex, double value) {
	setAttributeElementData(attrIndex, elemIndex, (unsigned char*) &value);
}

void Mesh::setAttributeElement(int attrIndex, int elemIndex, Vec2i value) {
	setAttributeElementData(attrIndex, elemIndex, (unsigned char*) &value);
}

void Mesh::setAttributeElement(int attrIndex, int elemIndex, Vec3i value) {
	setAttributeElementData(attrIndex, elemIndex, (unsigned char*) &value);
}

void Mesh::setAttributeElement(int attrIndex, int elemIndex, Vec4i value) {
	setAttributeElementData(attrIndex, elemIndex, (unsigned char*) &value);
}

void Mesh::setAttributeElement(int attrIndex, int elemIndex, Vec2f value) {
	setAttributeElementData(attrIndex, elemIndex, (unsigned char*) &value);
}

void Mesh::setAttributeElement(int attrIndex, int elemIndex, Vec3f value) {
	setAttributeElementData(attrIndex, elemIndex, (unsigned char*) &value);
}

void Mesh::setAttributeElement(int attrIndex, int elemIndex, Vec4f value) {
	setAttributeElementData(attrIndex, elemIndex, (unsigned char*) &value);
}

void Mesh::setAttributeElementData(int attrIndex, int elemIndex, unsigned char* bytes) {
	int attrByteOffset = attributeByteOffsets[attrIndex];
	int attrSize = attributeSizes[attrIndex];
	int typeSize = glTypeSize(attributeTypes[attrIndex]);
	for (int ci = 0; ci < attrSize; ci++) {
		for (int bi = 0; bi < typeSize; bi++) {
			((char*) vertices)[elemIndex * vertexByteSize + attrByteOffset + ci * typeSize + bi] = bytes[ci * typeSize + bi];
		}
	}
}

void Mesh::setIndices(unsigned int* indices) {
	if (this->indices != nullptr) {
		delete[] this->indices;
	}
	this->indices = indices;
}

void Mesh::setIndex(unsigned int indexIndex, unsigned int index) {
	if (indices == nullptr) {
		indices = new unsigned int[indexCount];
	}
	indices[indexIndex] = index;
}

void Mesh::setTopology(GLenum topology) {
	this->topology = topology;
}

void Mesh::setUsageHint(GLenum usage) {
	this->usage = usage;
}

void Mesh::setName(std::string n) {
	name = n;
	if (loadedToGL) {
		updateLabel();
	}
}

void Mesh::resize(int vCount, int iCount, bool copy, ResizeMode mode) {
	void* nVertices = nullptr;
	unsigned int* nIndices = nullptr;
	switch (mode) {
	case ResizeMode::GrowOnly:
		if (vCount > vertexCount) {
			nVertices = new char[vCount * vertexByteSize];
		}
		if (iCount > indexCount) {
			nIndices = new unsigned int[iCount];
		}
		break;
	case ResizeMode::GrowOrShrinkHalf:
		if (vCount > vertexCount || vCount < vertexCount / 2) {
			nVertices = new char[vCount * vertexByteSize];
		}
		if (iCount > indexCount || iCount < indexCount / 2) {
			nIndices = new unsigned int[iCount];
		}
		break;
	case ResizeMode::GrowOrShrinkQuarter:
		if (vCount > vertexCount || vCount < vertexCount - vertexCount / 4) {
			nVertices = new char[vCount * vertexByteSize];
		}
		if (iCount > indexCount || iCount < indexCount -  indexCount / 4) {
			nIndices = new unsigned int[iCount];
		}
		break;
	case ResizeMode::Force:
		if (vCount != vertexCount) {
			nVertices = new char[vCount * vertexByteSize];
		}
		if (iCount != indexCount) {
			nIndices = new unsigned int[iCount];
		}
		break;
	}

	if (nVertices != nullptr) {
		if (copy) {
			for (unsigned int i = 0; i < vCount && i < vertexCount; i++) {
				((char*) nVertices)[i] = ((char*) vertices)[i];
			}
		}
		delete[] vertices;
		vertices = nVertices;
		vertexCount = vCount;
	}
	if (nIndices != nullptr) {
		if (copy) {
			for (unsigned int i = 0; i < iCount && i < indexCount; i++) {
				nIndices[i] = indices[i];
			}
		}
		delete[] indices;
		indices = nIndices;
		indexCount = iCount;
	}
}

void Mesh::deleteLocal() {
	if (attributeSizes != nullptr) {
		delete[] attributeSizes;
		attributeSizes = nullptr;
	}
	if (attributeTypes != nullptr) {
		delete[] attributeTypes;
		attributeTypes = nullptr;
	}
	if (attributeByteOffsets != nullptr) {
		delete[] attributeByteOffsets;
		attributeByteOffsets = nullptr;
	}
	if (vertices != nullptr) {
		delete[] vertices;
		vertices = nullptr;
	}
	if (indices != nullptr) {
		delete[] indices;
		indices = nullptr;
	}
	cachedInLocal = false;
}

void Mesh::uploadToGL() {
	if (vao == 0) {
		glGenVertexArrays(1, &vao);
	}
	glBindVertexArray(vao);
	for (int i = 0; i < attributeCount; i++) {
		glEnableVertexAttribArray(i);
	}

	if (vboVertices == 0) {
		glGenBuffers(1, &vboVertices);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	loadedVertexBufferSize = vertexCount * vertexByteSize;
	glBufferData(GL_ARRAY_BUFFER, loadedVertexBufferSize, vertices, usage);
	for (int i = 0; i < attributeCount; i++) {
		if (glTypeIsInteger(attributeTypes[i])) {
			glVertexAttribIPointer(i, attributeSizes[i], attributeTypes[i], vertexByteSize, (void*) attributeByteOffsets[i]);
		} else {
			glVertexAttribPointer(i, attributeSizes[i], attributeTypes[i], GL_FALSE, vertexByteSize, (void*) attributeByteOffsets[i]);
		}
	}

	if (vboIndices == 0) {
		glGenBuffers(1, &vboIndices);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
	loadedIndexBufferSize = indexCount * sizeof(unsigned int);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, loadedIndexBufferSize, indices, usage);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Not unbinding the element array since it is bound to the vao
	loadedToGL = true;

	updateLabel();
}

void Mesh::updateInGL() {
	if (!loadedToGL) return;

	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	unsigned int vboVerticesSize = vertexCount * vertexByteSize;
	if (loadedVertexBufferSize != vboVerticesSize) {
		loadedVertexBufferSize = vboVerticesSize;
		glBufferData(GL_ARRAY_BUFFER, loadedVertexBufferSize, vertices, usage);
	} else {
		glBufferSubData(GL_ARRAY_BUFFER, 0, loadedVertexBufferSize, vertices);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
	unsigned int vboIndicesSize = indexCount * sizeof(unsigned int);
	if (loadedIndexBufferSize != vboIndicesSize) {
		loadedIndexBufferSize = vboIndicesSize;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, loadedIndexBufferSize, indices, usage);
	} else {
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, loadedIndexBufferSize, indices);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::deleteFromGL() {
	if (vboIndices != 0) {
		glDeleteBuffers(1, &vboIndices);
		vboIndices = 0;
	}
	if (vboVertices != 0) {
		glDeleteBuffers(1, &vboVertices);
		vboVertices = 0;
	}
	if (vao != 0) {
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}
	loadedToGL = false;
}

void Mesh::render() const {
	if (!loadedToGL) {
		Debug::log("Mesh", "Trying to render a mesh that is not loaded to GL!");
		return;
	}

	triangleCount += indexCount / 3;

	glBindVertexArray(vao);

	glDrawElements(topology, indexCount, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
}

void Mesh::updateLabel() {
	if (!name.empty()) {
		std::string fullName = "Mesh_VAO " + name;
		std::string fullVerticesName = "Mesh_VBO " + name + "/Vertices";
		std::string fullIndicesName = "Mesh_VBO " + name + "/Indices";
		glObjectLabel(GL_VERTEX_ARRAY, vao, fullName.size(), fullName.c_str());
		glObjectLabel(GL_BUFFER, vboVertices, fullVerticesName.size(), fullVerticesName.c_str());
		glObjectLabel(GL_BUFFER, vboIndices, fullIndicesName.size(), fullIndicesName.c_str());
	}
}

void* Mesh::getAttributePointer(int attrIndex, int& stride) const {
	stride = vertexByteSize;
	return ((char*) vertices) + attributeByteOffsets[0];
}
