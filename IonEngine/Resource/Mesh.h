#pragma once
#include "Core/MathVec.h"

namespace IonEngine {
	class Buffer;
	class RenderContext;

	class Mesh {
	public:
		struct Vertex {
			Vec3f position;
			Color color;
		};

		Mesh();
		~Mesh();
		Mesh(const Mesh&) = delete;
		void operator=(const Mesh&) = delete;

		inline u32 getVertexCount() { return vertexCount; }
		inline u32 getIndexCount() { return indexCount; }

		void allocate(u32 vCount, u32 iCount);
		void free();
		void setVertex(u32 index, const Vertex& vertex);
		void setIndex(u32 index, u32 indexValue);

		void upload(RenderContext* context);
		void render(RenderContext* context);
		void destroy(RenderContext* context);

	private:
		static constexpr u8 CPU_ALLOCATED = 0b0001;
		static constexpr u8 GPU_ALLOCATED = 0b0010;

		u8 state;
		Vertex* vertices;
		u32* indices;
		Buffer* vertexBuffer;
		Buffer* indexBuffer;
		u32 vertexCount;
		u32 indexCount;
	};
}
