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

		void setRenderContext(RenderContext* context);
		void upload();
		void render();
		void destroy();

	private:
		static constexpr u8 CPU_ALLOCATED = 0b0001;
		static constexpr u8 GPU_ALLOCATED = 0b0010;

		u8 state = 0;
		Vertex* vertices = nullptr;
		u32* indices = nullptr;
		Buffer* vertexBuffer = nullptr;
		Buffer* indexBuffer = nullptr;
		u32 vertexCount = 0;
		u32 indexCount = 0;
		RenderContext* context = nullptr;
	};
}
