#pragma once
#include <d3d11.h>
#include "Core/MathAdv.h"

namespace IonEngine {
	class Shader {
	private:
		struct MatrixBuffer {
			Matrix4x4f world;
			Matrix4x4f view;
			Matrix4x4f projection;
		};

	public:
		Shader();
		~Shader();
		Shader(const Shader&) = delete;
		void operator=(const Shader&) = delete;

		bool initialize(ID3D11Device* device, HWND hwnd);
		void shutdown();
		bool use(ID3D11DeviceContext* deviceContext, Matrix4x4f worldMatrix, Matrix4x4f viewMatrix, Matrix4x4f projectionMatrix);

	private:
		bool initializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename);
		void shutdownShader();
		void outputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);

		bool setShaderParameters(ID3D11DeviceContext* deviceContext, Matrix4x4f worldMatrix, Matrix4x4f viewMatrix, Matrix4x4f projectionMatrix);
		void useShader(ID3D11DeviceContext* deviceContext);

	private:
		ID3D11VertexShader* vertexShader;
		ID3D11PixelShader* pixelShader;
		ID3D11InputLayout* layout;
		ID3D11Buffer* matrixBuffer;
	};
}
