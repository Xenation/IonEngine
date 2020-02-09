#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

namespace IonEngine {
	using namespace DirectX;

	class Shader {
	private:
		struct MatrixBuffer {
			XMMATRIX world;
			XMMATRIX view;
			XMMATRIX projection;
		};

	public:
		Shader();
		~Shader();
		Shader(const Shader&) = delete;
		void operator=(const Shader&) = delete;

		bool initialize(ID3D11Device* device, HWND hwnd);
		void shutdown();
		bool render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

	private:
		bool initializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename);
		void shutdownShader();
		void outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);

		bool setShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
		void renderShader(ID3D11DeviceContext* deviceContext, int indexCount);

	private:
		ID3D11VertexShader* vertexShader;
		ID3D11PixelShader* pixelShader;
		ID3D11InputLayout* layout;
		ID3D11Buffer* matrixBuffer;
	};
}
