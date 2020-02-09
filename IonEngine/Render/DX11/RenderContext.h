#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d11.h>
#include <DirectXMath.h> // TODO replace with custom math
#undef near
#undef far
#include "Core/MathVec.h"

namespace IonEngine {
	using namespace DirectX;

	class EngineCore;

	class RenderContext {
	public:
		RenderContext(EngineCore* core);
		~RenderContext();
		RenderContext(const RenderContext&) = delete;
		void operator=(const RenderContext&) = delete;

		bool initialize(int width, int height, bool vsync, HWND hwnd, bool fullscreen, float near, float far);
		void shutdown();

		void beginFrame(Color clearColor);
		void endFrame();

		ID3D11Device* getDevice();
		ID3D11DeviceContext* getDeviceContext();

		void getProjectionMatrix(XMMATRIX&);
		void getWorldMatrix(XMMATRIX&);
		void getOrthoMatrix(XMMATRIX&);

		void getVideoCardInfo(char*, int&);

	private:
		EngineCore* engine;

		bool vsyncEnabled;
		int videoCardMemory;
		char videoCardDescription[128];
		IDXGISwapChain* swapchain;
		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;
		ID3D11RenderTargetView* renderTargetView;
		ID3D11Texture2D* depthStencilBuffer;
		ID3D11DepthStencilState* depthStencilState;
		ID3D11DepthStencilView* depthStencilView;
		ID3D11RasterizerState* rasterState;
		XMMATRIX projectionMatrix;
		XMMATRIX worldMatrix;
		XMMATRIX orthoMatrix;
	};
}
