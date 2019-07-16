#pragma once
#include <string>
#include "gl3w.h"
#include "XMath.h"

class Mesh {
public:
	enum class ResizeMode {
		GrowOnly,
		GrowOrShrinkHalf,
		GrowOrShrinkQuarter,
		Force,
	};

	static unsigned int triangleCount;

	Mesh(int vCount, int iCount);
	Mesh(std::string name, int vCount, int iCount);
	~Mesh();

	void setAttributesDefinition(int count, int* sizes);
	void setAttributesDefinition(int count, int* sizes, GLenum* types);
	void setAttribute(int index, signed char* values);
	void setAttribute(int index, char* values);
	void setAttribute(int index, short* values);
	void setAttribute(int index, unsigned short* values);
	void setAttribute(int index, int* values);
	void setAttribute(int index, unsigned int* values);
	void setAttribute(int index, float* values);
	void setAttribute(int index, double* values);
	void setAttributeData(int index, char* bytes);
	void setAttributeElement(int attrIndex, int elemIndex, signed char value);
	void setAttributeElement(int attrIndex, int elemIndex, char value);
	void setAttributeElement(int attrIndex, int elemIndex, short value);
	void setAttributeElement(int attrIndex, int elemIndex, unsigned short value);
	void setAttributeElement(int attrIndex, int elemIndex, int value);
	void setAttributeElement(int attrIndex, int elemIndex, unsigned int value);
	void setAttributeElement(int attrIndex, int elemIndex, float value);
	void setAttributeElement(int attrIndex, int elemIndex, double value);
	void setAttributeElement(int attrIndex, int elemIndex, Vec2i value);
	void setAttributeElement(int attrIndex, int elemIndex, Vec3i value);
	void setAttributeElement(int attrIndex, int elemIndex, Vec4i value);
	void setAttributeElement(int attrIndex, int elemIndex, Vec2f value);
	void setAttributeElement(int attrIndex, int elemIndex, Vec3f value);
	void setAttributeElement(int attrIndex, int elemIndex, Vec4f value);
	void setAttributeElementData(int attrIndex, int elemIndex, unsigned char* bytes);
	void setIndices(unsigned int* indices);
	void setIndex(unsigned int indexIndex, unsigned int index);
	void setTopology(GLenum topology);
	void setUsageHint(GLenum usage);
	void setName(std::string n);
	void resize(int vCount, int iCount, bool copy = true, ResizeMode mode = ResizeMode::Force);

	void deleteLocal();
	void uploadToGL();
	void updateInGL();
	void deleteFromGL();
	void render() const;

	inline int getVertexCount() const { return vertexCount; }
	inline int getIndexCount() const { return indexCount; }
	inline bool isLoadedToGL() const { return loadedToGL; }
	inline bool isCachedInLocal() const { return cachedInLocal; }
	inline GLenum getTopology() const { return topology; }

	void* getAttributePointer(int attrIndex, int& stride) const;
	inline unsigned int* getIndicesPointer() const { return indices; }

private:
	std::string name;
	GLuint vao = 0;
	GLuint vboVertices = 0;
	GLuint vboIndices = 0;
	bool loadedToGL = false;
	bool cachedInLocal = false;
	int attributeCount;
	int* attributeSizes = nullptr;
	GLenum* attributeTypes = nullptr;
	unsigned int* attributeByteOffsets = nullptr;
	int vertexByteSize;
	int vertexCount;
	void* vertices = nullptr;
	int indexCount;
	unsigned int* indices = nullptr;
	GLenum topology = GL_TRIANGLES;
	GLenum usage = GL_STATIC_DRAW;
	unsigned int loadedVertexBufferSize = 0;
	unsigned int loadedIndexBufferSize = 0;

	void updateLabel();
};

