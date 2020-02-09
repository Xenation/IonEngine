#include "RenderModule.h"

#if ION_VULKAN
#include "Vulkan/RenderContext.h"
#else
#include "DX11/RenderContext.h"
#include "DX11/Camera.h"
#include "DX11/Mesh.h"
#include "DX11/Shader.h"
#endif
#include "Display/Window.h"

using namespace IonEngine;



RenderModule::RenderModule(EngineCore* engine) : Module(engine) {
	context = new RenderContext(engine);
}

RenderModule::~RenderModule() {
	context->shutdown();
	delete context;
}


void RenderModule::initialize(Window* window) {
	Vec2i size = window->getSize();
	HWND win32Handle;
	void* win32ptr = reinterpret_cast<void*>(&win32Handle);
	window->getWin32Handle(win32ptr);
	if (!context->initialize(size.x, size.y, true, win32Handle, false, 0.1f, 1000.0f)) {
		Debug::fail(Debug::RENDER, "Could not initialize DX11!");
	}

	camera = new Camera();
	camera->setPosition(Vec3f::backward * 10.0f);
	mesh = new Mesh();
	mesh->initialize(context->getDevice());
	shader = new Shader();
	shader->initialize(context->getDevice(), win32Handle);
}

void RenderModule::render() {
	context->beginFrame(Color::clear);
	
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	camera->render();
	context->getWorldMatrix(worldMatrix);
	camera->getViewMatrix(viewMatrix);
	context->getProjectionMatrix(projectionMatrix);

	mesh->render(context->getDeviceContext());

	bool success = shader->render(context->getDeviceContext(), mesh->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!success) {
		Debug::fail(Debug::RENDER, "Failed to render!");
	}

	context->endFrame();
}

void RenderModule::shutdown() {
	if (shader != nullptr) {
		shader->shutdown();
		delete shader;
		shader = nullptr;
	}

	if (mesh != nullptr) {
		mesh->shutdown();
		delete mesh;
		mesh = nullptr;
	}

	if (camera != nullptr) {
		delete camera;
		camera = nullptr;
	}

	if (context != nullptr) {
		context->shutdown();
		delete context;
		context = nullptr;
	}
}
