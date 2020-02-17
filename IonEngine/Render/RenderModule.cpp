#include "RenderModule.h"

#if ION_VULKAN
#include "Vulkan/RenderContext.h"
#else
#include <d3d11.h>
#include "DX11/RenderContext.h"
#include "DX11/Camera.h"
#include "Resource/Mesh.h"
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
	mesh->allocate(3, 3);
	mesh->setVertex(0, {Vec3f(0.0f, 1.0f, 0.0f), Color::red});
	mesh->setVertex(1, {Vec3f(1.0f, 0.0f, 0.0f), Color::red});
	mesh->setVertex(2, {Vec3f(-1.0f, 0.0f, 0.0f), Color::red});
	mesh->setIndex(0, 0);
	mesh->setIndex(1, 1);
	mesh->setIndex(2, 2);
	mesh->setRenderContext(context);
	mesh->upload();

	shader = new Shader();
	shader->initialize(context->getDevice(), win32Handle);
}

void RenderModule::render() {
	context->beginFrame(Color::clear);
	
	Matrix4x4f worldMatrix, viewMatrix, projectionMatrix;
	camera->render();
	context->getWorldMatrix(worldMatrix);
	camera->getViewMatrix(viewMatrix);
	context->getProjectionMatrix(projectionMatrix);

	bool success = shader->use(context->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	if (!success) {
		Debug::fail(Debug::RENDER, "Failed to use shader!");
	}

	mesh->render();

	context->endFrame();
}

void RenderModule::shutdown() {
	if (shader != nullptr) {
		shader->shutdown();
		delete shader;
		shader = nullptr;
	}

	if (mesh != nullptr) {
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
