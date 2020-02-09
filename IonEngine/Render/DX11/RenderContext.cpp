#if ION_DX11
#include "RenderContext.h"

using namespace IonEngine;



RenderContext::RenderContext(EngineCore* core) : engine(core) {
	swapchain = nullptr;
	device = nullptr;
	deviceContext = nullptr;
	renderTargetView = nullptr;
	depthStencilBuffer = nullptr;
	depthStencilState = nullptr;
	depthStencilView = nullptr;
	rasterState = nullptr;
}

RenderContext::~RenderContext() {

}


bool RenderContext::initialize(int width, int height, bool vsync, HWND hwnd, bool fullscreen, float near, float far) {
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	u32 numModes, i, numerator, denominator;
	u64 strLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapchainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backbuffer;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fov, aspect;

	vsyncEnabled = vsync;

	// Create a DX graphics interface factory
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**) &factory);
	if (FAILED(result)) {
		return false;
	}

	// Use factory to create an adapter for the main graphics interface
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)) {
		return false;
	}

	// Enumerate the primary adapter output
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result)) {
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if (FAILED(result)) {
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (displayModeList == nullptr) {
		return false;
	}

	// Fill the display mode list structures
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result)) {
		return false;
	}

	// Go through all the display modes and find the one that matches the screen width and height
	// When a match is found store the numerator and denominator of the refresh rate for that monitor
	for (i = 0; i < numModes; i++) {
		if (displayModeList[i].Width == (u32) width && displayModeList[i].Height == (u32) height) {
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}

	// Get adapter description
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result)) {
		return false;
	}


	// Store the dedicated video card memory in MB
	videoCardMemory = (int) (adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&strLength, videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0) {
		return false;
	}


	// Release the display mode list
	delete[] displayModeList;
	displayModeList = nullptr;
	// Release the adapter output
	adapterOutput->Release();
	adapterOutput = nullptr;
	// Release the adapter
	adapter->Release();
	adapter = nullptr;
	// Release the factory
	factory->Release();
	factory = nullptr;

	// Initialize the swap chain description
	ZeroMemory(&swapchainDesc, sizeof(swapchainDesc));
	// Set to single back buffer
	swapchainDesc.BufferCount = 1;
	// Set to width and height of the back buffer
	swapchainDesc.BufferDesc.Width = width;
	swapchainDesc.BufferDesc.Height = height;
	// Set regular 32bit surface for the back buffer
	swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// Set the refresh rate of the back buffer
	if (vsyncEnabled) {
		swapchainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapchainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	} else {
		swapchainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	// Set the usage of the back buffer
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// Set the handle for the window to render to
	swapchainDesc.OutputWindow = hwnd;
	// Turn multisampling off
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.SampleDesc.Quality = 0;
	// Set to full screen or windowed mode
	if (fullscreen) {
		swapchainDesc.Windowed = false;
	} else {
		swapchainDesc.Windowed = true;
	}
	// Set the scan line ordering and scaling to unspecified
	swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// Discard the back buffer contents after presenting
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	// Don't set the advanced flags
	swapchainDesc.Flags = 0;

	// Set the feature level to DX11
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context
	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapchainDesc, &swapchain, &device, nullptr, &deviceContext);
	if (FAILED(result)) {
		return false;
	}

	// Get the pointer to the backbuffer
	result = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) &backbuffer);
	if (FAILED(result)) {
		return false;
	}
	// Create the render target view with the back buffer pointer
	result = device->CreateRenderTargetView(backbuffer, nullptr, &renderTargetView);
	if (FAILED(result)) {
		return false;
	}
	// Release pointer to the back buffer as we no longer need it
	backbuffer->Release();
	backbuffer = nullptr;

	// Initialize the description of the depth buffer
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	// Set up the description of the depth buffer
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out desc
	result = device->CreateTexture2D(&depthBufferDesc, nullptr, &depthStencilBuffer);
	if (FAILED(result)) {
		return false;
	}

	// Initialize the description of the stencil state
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	// Set up the description of the stencil state
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xff;
	depthStencilDesc.StencilWriteMask = 0xff;
	// Stencil operations if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state
	result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	if (FAILED(result)) {
		return false;
	}
	// Set the depth stencil state
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);

	// Initialize the depth stencil view
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	// Set up the depth stencil view desc
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	// Create the depth stencil view
	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	if (FAILED(result)) {
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);


	// Setup the raster description which will determine how and what polygons will be drawn
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	// Create the rasterizer state from the description wa just filled out.
	result = device->CreateRasterizerState(&rasterDesc, &rasterState);
	if (FAILED(result)) {
		return false;
	}
	// Now set the rasterizer state
	deviceContext->RSSetState(rasterState);

	// Setup the viewport for rendering
	viewport.Width = (float) width;
	viewport.Height = (float) width;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	// Create the viewport
	deviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix
	fov = 3.141592654f / 2.0f;
	aspect = (float) width / (float) height;
	// Create the projection matrix
	projectionMatrix = XMMatrixPerspectiveFovLH(fov, aspect, near, far);

	// Initialize the world matrix to the identity
	worldMatrix = XMMatrixIdentity();

	// create an ortho proj matrix for 2D rendering
	orthoMatrix = XMMatrixOrthographicLH(width, height, near, far);


	return true;
}

void RenderContext::shutdown() {
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception
	if (swapchain != nullptr) {
		swapchain->SetFullscreenState(false, nullptr);
	}

	if (rasterState != nullptr) {
		rasterState->Release();
		rasterState = nullptr;
	}
	if (depthStencilView != nullptr) {
		depthStencilView->Release();
		depthStencilView = nullptr;
	}
	if (depthStencilState != nullptr) {
		depthStencilState->Release();
		depthStencilState = nullptr;
	}
	if (depthStencilBuffer != nullptr) {
		depthStencilBuffer->Release();
		depthStencilBuffer = nullptr;
	}
	if (renderTargetView != nullptr) {
		renderTargetView->Release();
		renderTargetView = nullptr;
	}
	if (deviceContext != nullptr) {
		deviceContext->Release();
		deviceContext = nullptr;
	}
	if (device != nullptr) {
		device->Release();
		device = nullptr;
	}
	if (swapchain != nullptr) {
		swapchain->Release();
		swapchain = nullptr;
	}
}

void RenderContext::beginFrame(Color clearColor) {
	// Clear the back buffer
	deviceContext->ClearRenderTargetView(renderTargetView, clearColor.data);

	// Clear the depth buffer
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void RenderContext::endFrame() {
	// Present the back buffer to the screen
	if (vsyncEnabled) {
		// Lock to screen refresh rate
		swapchain->Present(1, 0);
	} else {
		// Present as fast as possible
		swapchain->Present(0, 0);
	}
}

ID3D11Device* RenderContext::getDevice() {
	return device;
}

ID3D11DeviceContext* RenderContext::getDeviceContext() {
	return deviceContext;
}

void RenderContext::getProjectionMatrix(XMMATRIX& projectionMatrix) {
	projectionMatrix = this->projectionMatrix;
}

void RenderContext::getWorldMatrix(XMMATRIX& worldMatrix) {
	worldMatrix = this->worldMatrix;
}

void RenderContext::getOrthoMatrix(XMMATRIX& orthoMatrix) {
	orthoMatrix = this->orthoMatrix;
}

void RenderContext::getVideoCardInfo(char* cardName, int& memory) {
	strcpy_s(cardName, 128, videoCardDescription);
	memory = videoCardMemory;
}

#endif