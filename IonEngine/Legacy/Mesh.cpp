#include "Mesh.h"

#include <string>
#include "GLUtils.h"
#include "Debug.h"
using namespace IonEngine;



unsigned int Mesh::triangleCount = 0;


Mesh::Mesh(u32 vCount, u32 iCount) : Mesh("", vCount, iCount) {}

Mesh::Mesh(std::string name, u32 vCount, u32 iCount)
	: name(name), vertexCount(vCount), indexCount(iCount), attributeCount(0), vertexByteSize(0) {
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

void Mesh::setAttributesDefinition(u32 count, u32* sizes) {
	GLenum* defTypes = new GLenum[count];
	for (u32 i = 0; i < count; i++) {
		defTypes[i] = GL_FLOAT;
	}
	setAttributesDefinition(count, sizes, defTypes);
}

void Mesh::setAttributesDefinition(u32 count, u32* sizes, GLenum* types) {
	attributeCount = count;
	attributeSizes = sizes;
	attributeTypes = types;
	attributeByteOffsets = new u32[count];
	vertexByteSize = 0;
	for (u32 i = 0; i < attributeCount; i++) {
		attributeByteOffsets[i] = vertexByteSize;
		vertexByteSize += attributeSizes[i] * glTypeSize(types[i]);
	}
	if (vertices != nullptr) {
		delete[] vertices;
	}
	vertices = new u8[vertexCount * vertexByteSize];
	cachedInLocal = true;
}

void Mesh::setAttribute(u32 index, i8* values) {
	if (attributeTypes[index] != GL_BYTE) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_BYTE!").c_str());
		return;
	}
	setAttributeData(index, (u8*) values);
}

void Mesh::setAttribute(u32 index, u8* values) {
	if (attributeTypes[index] != GL_UNSIGNED_BYTE) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_UNSIGNED_BYTE!").c_str());
		return;
	}
	setAttributeData(index, (u8*) values);
}

void Mesh::setAttribute(u32 index, i16* values) {
	if (attributeTypes[index] != GL_SHORT) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_SHORT!").c_str());
		return;
	}
	setAttributeData(index, (u8*) values);
}

void Mesh::setAttribute(u32 index, u16* values) {
	if (attributeTypes[index] != GL_UNSIGNED_SHORT && attributeTypes[index] != GL_HALF_FLOAT) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_UNSIGNED_SHORT or GL_HALF_FLOAT!").c_str());
		return;
	}
	setAttributeData(index, (u8*) values);
}

void Mesh::setAttribute(u32 index, i32* values) {
	if (attributeTypes[index] != GL_INT) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_INT!").c_str());
		return;
	}
	setAttributeData(index, (u8*) values);
}

void Mesh::setAttribute(u32 index, u32* values) {
	if (attributeTypes[index] != GL_UNSIGNED_INT) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_UNSIGNED_INT!").c_str());
		return;
	}
	setAttributeData(index, (u8*) values);
}

void Mesh::setAttribute(u32 index, float* values) {
	if (attributeTypes[index] != GL_FLOAT) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_FLOAT!").c_str());
		return;
	}
	setAttributeData(index, (u8*) values);
}

void Mesh::setAttribute(u32 index, double* values) {
	if (attributeTypes[index] != GL_DOUBLE) {
		Debug::log("Mesh", ("Attribute set at index " + std::to_string(index) + ", wrong type: expected GL_DOUBLE!").c_str());
		return;
	}
	setAttributeData(index, (u8*) values);
}

void Mesh::setAttributeData(u32 index, u8* bytes) {
	u32 attrByteOffset = attributeByteOffsets[index];
	u32 attrSize = attributeSizes[index];
	u32 typeSize = glTypeSize(attributeTypes[index]);
	for (u32 vi = 0; vi < vertexCount; vi++) {
		for (u32 i = 0; i < attrSize; i++) {
			for (u32 bi = 0; bi < typeSize; bi++) {
				vertices[vi * vertexByteSize + attrByteOffset + i * typeSize + bi] = bytes[vi * attrSize * typeSize + i * typeSize + bi];
			}
		}
	}
}

void Mesh::setAttributeElement(u32 attrIndex, u32 elemIndex, i8 value) {
	setAttributeElementData(attrIndex, elemIndex, (u8*) &value);
}

void Mesh::setAttributeElement(u32 attrIndex, u32 elemIndex, u8 value) {
	setAttributeElementData(attrIndex, elemIndex, (u8*) &value);
}

void Mesh::setAttributeElement(u32 attrIndex, u32 elemIndex, i16 value) {
	setAttributeElementData(attrIndex, elemIndex, (u8*) &value);
}

void Mesh::setAttributeElement(u32 attrIndex, u32 elemIndex, u16 value) {
	setAttributeElementData(attrIndex, elemIndex, (u8*) &value);
}

void Mesh::setAttributeElement(u32 attrIndex, u32 elemIndex, i32 value) {
	setAttributeElementData(attrIndex, elemIndex, (u8*) &value);
}

void Mesh::setAttributeElement(u32 attrIndex, u32 elemIndex, u32 value) {
	setAttributeElementData(attrIndex, elemIndex, (u8*) &value);
}

void Mesh::setAttributeElement(u32 attrIndex, u32 elemIndex, float value) {
	setAttributeElementData(attrIndex, elemIndex, (u8*) &value);
}

void Mesh::setAttributeElement(u32 attrIndex, u32 elemIndex, double value) {
	setAttributeElementData(attrIndex, elemIndex, (u8*) &value);
}

void Mesh::setAttributeElement(u32 attrIndex, u32 elemIndex, Vec2i value) {
	setAttributeElementData(attrIndex, elemIndex, (u8*) &value);
}

void Mesh::setAttributeElement(u32 attrIndex, u32 elemIndex, Vec3i value) {
	setAttributeElementData(attrIndex, elemIndex, (u8*) &value);
}

void Mesh::setAttributeElement(u32 attrIndex, u32 elemIndex, Vec4i value) {
	setAttributeElementData(attrIndex, elemIndex, (u8*) &value);
}

void Mesh::setAttributeElement(u32 attrIndex, u32 elemIndex, Vec2f value) {
	setAttributeElementData(attrIndex, elemIndex, (u8*) &value);
}

void Mesh::setAttributeElement(u32 attrIndex, u32 elemIndex, Vec3f value) {
	setAttributeElementData(attrIndex, elemIndex, (u8*) &value);
}

void Mesh::setAttributeElement(u32 attrIndex, u32 elemIndex, Vec4f value) {
	setAttributeElementData(attrIndex, elemIndex, (u8*) &value);
}

void Mesh::setAttributeElementData(u32 attrIndex, u32 elemIndex, u8* bytes) {
	u32 attrByteOffset = attributeByteOffsets[attrIndex];
	u32 attrSize = attributeSizes[attrIndex];
	u32 typeSize = glTypeSize(attributeTypes[attrIndex]);
	for (u32 ci = 0; ci < attrSize; ci++) {
		for (u32 bi = 0; bi < typeSize; bi++) {
			vertices[elemIndex * vertexByteSize + attrByteOffset + ci * typeSize + bi] = bytes[ci * typeSize + bi];
		}
	}
}

void Mesh::setIndices(u32* indices) {
	if (this->indices != nullptr) {
		delete[] this->indices;
	}
	this->indices = indices;
}

void Mesh::setIndex(u32 indexIndex, u32 index) {
	if (indices == nullptr) {
		indices = new u32[indexCount];
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

void Mesh::resize(u32 vCount, u32 iCount, bool copy, ResizeMode mode) {
	u8* nVertices = nullptr;
	u32* nIndices = nullptr;
	switch (mode) {
	case ResizeMode::GrowOnly:
		if (vCount > vertexCount) {
			nVertices = new u8[vCount * vertexByteSize];
		}
		if (iCount > indexCount) {
			nIndices = new u32[iCount];
		}
		break;
	case ResizeMode::GrowOrShrinkHalf:
		if (vCount > vertexCount || vCount < vertexCount / 2) {
			nVertices = new u8[vCount * vertexByteSize];
		}
		if (iCount > indexCount || iCount < indexCount / 2) {
			nIndices = new u32[iCount];
		}
		break;
	case ResizeMode::GrowOrShrinkQuarter:
		if (vCount > vertexCount || vCount < vertexCount - vertexCount / 4) {
			nVertices = new u8[vCount * vertexByteSize];
		}
		if (iCount > indexCount || iCount < indexCount -  indexCount / 4) {
			nIndices = new u32[iCount];
		}
		break;
	case ResizeMode::Force:
		if (vCount != vertexCount) {
			nVertices = new u8[vCount * vertexByteSize];
		}
		if (iCount != indexCount) {
			nIndices = new u32[iCount];
		}
		break;
	}

	if (nVertices != nullptr) {
		if (copy) {
			for (u32 i = 0; i < vCount && i < vertexCount; i++) {
				nVertices[i] = vertices[i];
			}
		}
		delete[] vertices;
		vertices = nVertices;
		vertexCount = vCount;
	}
	if (nIndices != nullptr) {
		if (copy) {
			for (u32 i = 0; i < iCount && i < indexCount; i++) {
				nIndices[i] = indices[i];
			}
		}
		delete[] indices;
		indices = nIndices;
		indexCount = iCount;
	}
}

void Mesh::reverseWindingOrder() {
	for (u32 t = 0; t < indexCount / 3; t++) {
		u32 tmp = indices[3 * t + 1];
		indices[3 * t + 1] = indices[3 * t + 2];
		indices[3 * t + 2] = tmp;
	}
}

void Mesh::computeTangents(u32 posAttrIndex, u32 uvAttrIndex, u32 tanAttrIndex) {
	u32 posAttrByteOffset = attributeByteOffsets[posAttrIndex];
	u32 uvAttrByteOffset = attributeByteOffsets[uvAttrIndex];
	u32 tanAttrByteOffset = attributeByteOffsets[tanAttrIndex];
	for (u32 t = 0; t < indexCount; t += 3) { // Can probably be improved
		Vec3f p0 = *(Vec3f*) (vertices + indices[t] * vertexByteSize + posAttrByteOffset);
		Vec3f p1 = *(Vec3f*) (vertices + indices[t + 1] * vertexByteSize + posAttrByteOffset);
		Vec3f p2 = *(Vec3f*) (vertices + indices[t + 2] * vertexByteSize + posAttrByteOffset);
		Vec2f uv0 = *(Vec2f*) (vertices + indices[t] * vertexByteSize + uvAttrByteOffset);
		Vec2f uv1 = *(Vec2f*) (vertices + indices[t + 1] * vertexByteSize + uvAttrByteOffset);
		Vec2f uv2 = *(Vec2f*) (vertices + indices[t + 2] * vertexByteSize + uvAttrByteOffset);
		Vec3f edge1 = p1 - p0;
		Vec3f edge2 = p2 - p0;
		Vec2f deltaUV1 = uv1 - uv0;
		Vec2f deltaUV2 = uv2 - uv0;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		Vec3f tangent;
		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent.normalize();

		*((Vec3f*) (vertices + indices[t] * vertexByteSize + tanAttrByteOffset)) = tangent;
		*((Vec3f*) (vertices + indices[t + 1] * vertexByteSize + tanAttrByteOffset)) = tangent;
		*((Vec3f*) (vertices + indices[t + 2] * vertexByteSize + tanAttrByteOffset)) = tangent;
	}
}

void Mesh::recalculateBounds() {
	bounds = Boxf(Vec3f::positiveInfinity, Vec3f::negativeInfinity);
	u32 stride;
	u8* posAttr = getAttributePointer(0, stride);
	for (u32 vi = 0; vi < vertexCount * stride; vi += stride) {
		Vec3f pos = *((Vec3f*) (posAttr + vi));
		if (pos.x < bounds.min.x) {
			bounds.min.x = pos.x;
		}
		if (pos.y < bounds.min.y) {
			bounds.min.y = pos.y;
		}
		if (pos.z < bounds.min.z) {
			bounds.min.z = pos.z;
		}

		if (pos.x > bounds.max.x) {
			bounds.max.x = pos.x;
		}
		if (pos.y > bounds.max.y) {
			bounds.max.y = pos.y;
		}
		if (pos.z > bounds.max.z) {
			bounds.max.z = pos.z;
		}
	}
}

void Mesh::recalculateBoundsFromIndices() { // Slower but prevents using vertices out of drawn indices range
	bounds = Boxf(Vec3f::positiveInfinity, Vec3f::negativeInfinity);
	u32 stride;
	u8* posAttr = getAttributePointer(0, stride);
	u32 iCount = (drawnIndexCount == -1) ? indexCount : static_cast<u32>(drawnIndexCount);
	for (u32 ii = 0; ii < iCount; ii++) {
		Vec3f pos = *((Vec3f*) (posAttr + indices[ii] * stride));
		if (pos.x < bounds.min.x) {
			bounds.min.x = pos.x;
		}
		if (pos.y < bounds.min.y) {
			bounds.min.y = pos.y;
		}
		if (pos.z < bounds.min.z) {
			bounds.min.z = pos.z;
		}

		if (pos.x > bounds.max.x) {
			bounds.max.x = pos.x;
		}
		if (pos.y > bounds.max.y) {
			bounds.max.y = pos.y;
		}
		if (pos.z > bounds.max.z) {
			bounds.max.z = pos.z;
		}
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
	// TODO have more control over bounds
	recalculateBounds();

	if (vao == 0) {
		glGenVertexArrays(1, &vao);
	}
	glBindVertexArray(vao);
	for (u32 i = 0; i < attributeCount; i++) {
		glEnableVertexAttribArray(i);
	}

	if (vboVertices == 0) {
		glGenBuffers(1, &vboVertices);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	loadedVertexBufferSize = vertexCount * vertexByteSize;
	glBufferData(GL_ARRAY_BUFFER, loadedVertexBufferSize, vertices, usage);
	for (u32 i = 0; i < attributeCount; i++) {
		if (glTypeIsInteger(attributeTypes[i])) {
			glVertexAttribIPointer(i, attributeSizes[i], attributeTypes[i], (GLsizei) vertexByteSize, (void*) attributeByteOffsets[i]);
		} else {
			glVertexAttribPointer(i, attributeSizes[i], attributeTypes[i], GL_FALSE, (GLsizei) vertexByteSize, (void*) attributeByteOffsets[i]);
		}
	}

	if (vboIndices == 0) {
		glGenBuffers(1, &vboIndices);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
	loadedIndexBufferSize = indexCount * sizeof(u32);
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
	u32 vboVerticesSize = vertexCount * vertexByteSize;
	if (loadedVertexBufferSize != vboVerticesSize) {
		loadedVertexBufferSize = vboVerticesSize;
		glBufferData(GL_ARRAY_BUFFER, loadedVertexBufferSize, vertices, usage);
	} else {
		glBufferSubData(GL_ARRAY_BUFFER, 0, loadedVertexBufferSize, vertices);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
	u32 vboIndicesSize = indexCount * sizeof(u32);
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

	i32 drawnCount = (drawnIndexCount == -1) ? static_cast<i32>(indexCount) : drawnIndexCount;

	triangleCount += drawnCount / 3;

	glBindVertexArray(vao);

	glDrawElements(topology, drawnCount, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
}

void Mesh::updateLabel() {
	if (!name.empty()) {
		std::string fullName = "Mesh_VAO " + name;
		std::string fullVerticesName = "Mesh_VBO " + name + "/Vertices";
		std::string fullIndicesName = "Mesh_VBO " + name + "/Indices";
		glObjectLabel(GL_VERTEX_ARRAY, vao, (GLsizei) fullName.size(), fullName.c_str());
		glObjectLabel(GL_BUFFER, vboVertices, (GLsizei) fullVerticesName.size(), fullVerticesName.c_str());
		glObjectLabel(GL_BUFFER, vboIndices, (GLsizei) fullIndicesName.size(), fullIndicesName.c_str());
	}
}

u8* Mesh::getAttributePointer(u32 attrIndex, u32& stride) const {
	stride = vertexByteSize;
	return vertices + attributeByteOffsets[0];
}
