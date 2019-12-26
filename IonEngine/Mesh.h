#pragma once
#include <string>
#include "gl3w.h"
#include "XMath.h"

namespace IonEngine {
	class Mesh {
	public:
		enum class ResizeMode {
			GrowOnly,
			GrowOrShrinkHalf,
			GrowOrShrinkQuarter,
			Force,
		};

		static u32 triangleCount;

		Mesh(u32 vCount, u32 iCount);
		Mesh(std::string name, u32 vCount, u32 iCount);
		~Mesh();

		void setAttributesDefinition(u32 count, u32* sizes);
		void setAttributesDefinition(u32 count, u32* sizes, GLenum* types);
		void setAttribute(u32 index, i8* values);
		void setAttribute(u32 index, u8* values);
		void setAttribute(u32 index, i16* values);
		void setAttribute(u32 index, u16* values);
		void setAttribute(u32 index, i32* values);
		void setAttribute(u32 index, u32* values);
		void setAttribute(u32 index, float* values);
		void setAttribute(u32 index, double* values);
		void setAttributeData(u32 index, u8* bytes);
		void setAttributeElement(u32 attrIndex, u32 elemIndex, i8 value);
		void setAttributeElement(u32 attrIndex, u32 elemIndex, u8 value);
		void setAttributeElement(u32 attrIndex, u32 elemIndex, i16 value);
		void setAttributeElement(u32 attrIndex, u32 elemIndex, u16 value);
		void setAttributeElement(u32 attrIndex, u32 elemIndex, i32 value);
		void setAttributeElement(u32 attrIndex, u32 elemIndex, u32 value);
		void setAttributeElement(u32 attrIndex, u32 elemIndex, float value);
		void setAttributeElement(u32 attrIndex, u32 elemIndex, double value);
		void setAttributeElement(u32 attrIndex, u32 elemIndex, Vec2i value);
		void setAttributeElement(u32 attrIndex, u32 elemIndex, Vec3i value);
		void setAttributeElement(u32 attrIndex, u32 elemIndex, Vec4i value);
		void setAttributeElement(u32 attrIndex, u32 elemIndex, Vec2f value);
		void setAttributeElement(u32 attrIndex, u32 elemIndex, Vec3f value);
		void setAttributeElement(u32 attrIndex, u32 elemIndex, Vec4f value);
		void setAttributeElementData(u32 attrIndex, u32 elemIndex, u8* bytes);
		void setIndices(u32* indices);
		void setIndex(u32 indexIndex, u32 index);
		void setTopology(GLenum topology);
		void setUsageHint(GLenum usage);
		void setName(std::string n);
		void resize(u32 vCount, u32 iCount, bool copy = true, ResizeMode mode = ResizeMode::Force);
		void reverseWindingOrder();
		void computeTangents(u32 posAttrIndex, u32 uvAttrIndex, u32 tanAttrIndex);
		void recalculateBounds();
		void recalculateBoundsFromIndices();

		void deleteLocal();
		void uploadToGL();
		void updateInGL();
		void deleteFromGL();
		void render() const;

		inline u32 getVertexCount() const { return vertexCount; }
		inline u32 getIndexCount() const { return indexCount; }
		inline void setDrawnIndexCount(i32 drawnCount) { drawnIndexCount = drawnCount; }
		inline i32 getDrawnIndexCount() const { return drawnIndexCount; }
		inline bool isLoadedToGL() const { return loadedToGL; }
		inline bool isCachedInLocal() const { return cachedInLocal; }
		inline GLenum getTopology() const { return topology; }
		inline std::string getName() const { return name; }
		inline Boxf getBounds() const { return bounds; }

		u8* getAttributePointer(u32 attrIndex, u32& stride) const;
		inline u32* getIndicesPointer() const { return indices; }

	private:
		std::string name;
		GLuint vao = 0;
		GLuint vboVertices = 0;
		GLuint vboIndices = 0;
		bool loadedToGL = false;
		bool cachedInLocal = false;
		u32 attributeCount;
		u32* attributeSizes = nullptr;
		GLenum* attributeTypes = nullptr;
		u32* attributeByteOffsets = nullptr;
		u32 vertexByteSize;
		u32 vertexCount;
		u8* vertices = nullptr;
		u32 indexCount;
		u32* indices = nullptr;
		i32 drawnIndexCount = -1; // -1 means draw all
		GLenum topology = GL_TRIANGLES;
		GLenum usage = GL_STATIC_DRAW;
		u32 loadedVertexBufferSize = 0;
		u32 loadedIndexBufferSize = 0;
		Boxf bounds = Boxf(Vec3f::zero, Vec3f::zero);

		void updateLabel();
	};
}
