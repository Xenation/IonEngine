#pragma once
//// ==== ==== THIS IS A TEMPORARY FILE FOR TESTING, IT SHOULD EVENTUALLY BE REMOVED ==== ==== ////
#include <d3d11.h>
#include <DirectXMath.h>

namespace IonEngine {
	class Buffer;

	using namespace DirectX;

	class Mesh {
	private:
		struct Vertex {
			XMFLOAT3 position;
			XMFLOAT4 color;
		};

	public:
		Mesh();
		~Mesh();
		Mesh(const Mesh&) = delete;
		void operator=(const Mesh&) = delete;

		bool initialize(ID3D11Device* device);
		void shutdown();
		void render(ID3D11DeviceContext* deviceContext);

		i32 getIndexCount();

	private:
		bool initializeBuffers(ID3D11Device* device);
		void shutdownBuffers();
		void renderBuffers(ID3D11DeviceContext* deviceContext);

	private:
		Buffer* vertexBuffer;
		Buffer* indexBuffer;
		//ID3D11Buffer* vertexBuffer;
		//ID3D11Buffer* indexBuffer;
		i32 vertexCount;
		i32 indexCount;
	};
}
