#include "Pipeline.h"

#include <locale>
#include <gl3w.h>
#include "ShaderProgram.h"
#include "Renderer.h"
#include "Camera.h"
#include "Time.h"
#include "UniformBuffer.h"
#include "Material.h"
#include "RenderPass.h"
#include "Framebuffer.h"
#include "SpecializedShaderProgram.h"
#include "LightManager.h"
#include "Light.h"
#include "ShadowAtlas.h"
#include "Texture.h"
using namespace IonEngine;

#define RENDERERS_START_SIZE 32
#define RENDERERS_INCREASE 32
#define CAMERAS_START_SIZE 1
#define CAMERAS_INCREASE 1
#define RENDERPASSES_START_SIZE 4
#define RENDERPASSES_INCREASE 1



Pipeline::Pipeline(u32 width, u32 height, LightManager* lightManager)
	: lightManager(lightManager), renderers(RENDERERS_START_SIZE, RENDERERS_INCREASE), cameras(CAMERAS_START_SIZE, CAMERAS_INCREASE), renderPasses(RENDERPASSES_START_SIZE, RENDERERS_INCREASE) {
	Texture::initializeDefaults();

	resizeFrameBuffer(width, height);

	shadowAtlas = new ShadowAtlas();
	renderPasses.add(new RenderPassLightAssign(this));
	renderPasses.add(new RenderPassShadows(this, renderers));
	renderPasses.add(new RenderPassOpaque("opaque", this, width, height, samples));
	renderPasses.add(new RenderPassSkybox(this));
	renderPasses.add(new RenderPassTransparent("transparent", this));
	//renderPasses.add(new RenderPassPostprocess("postprocess", renderBuffer));

	SpecializedShaderProgram::initialize(this);
	ShaderProgram::initializeAll(this);

	for (u32 passIndex = 0; passIndex < renderPasses.count; passIndex++) {
		renderPasses[passIndex]->onShadersInitialized();
	}

	// Global Buffers Setup
	globalUniformBuffer = new UniformBuffer("Global");
	globalUniformBuffer->setLayouts(2, new UniformLayout[2]{UniformLayout(1, 3, new GLSLType[3]{GLSL_FLOAT, GLSL_VEC4, GLSL_VEC4}), UniformLayout(2, 8, new GLSLType[8]{GLSL_MAT4, GLSL_MAT4, GLSL_MAT4, GLSL_MAT4, GLSL_FLOAT, GLSL_FLOAT, GLSL_IVEC2, GLSL_UINT})});
	globalUniformBuffer->uploadToGL();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

Pipeline::~Pipeline() {
	delete globalUniformBuffer;
}


void Pipeline::render() {
	// Globals Update
	Light* mainDirectional = lightManager->getMainDirectional();
	globalUniformBuffer->getLayout(0).setMember(0, Time::time);
	if (mainDirectional != nullptr) {
		globalUniformBuffer->getLayout(0).setMember(1, mainDirectional->color.vec);
		globalUniformBuffer->getLayout(0).setMember(2, toVec4f(mainDirectional->getDirection()));
	}
	globalUniformBuffer->updateLayout(0);

	for (u32 passIndex = 0; passIndex < renderPasses.count; passIndex++) {
		renderPasses[passIndex]->prepareFrame();
	}

	u32 rendered = 0;
	for (u32 i = 0; i < cameras.capacity && rendered < cameras.count; i++) {
		if (cameras[i] == nullptr) continue;
		render(cameras[i]);
		rendered++;
	}
}

void Pipeline::render(Camera* camera) {
	// Frustum Culling
	Frustum3f cameraFrustum = camera->getFrustum();
	SimpleSet<u32> visibleRenderers(32, 32);
	u32 counted = 0;
	for (u32 i = 0; i < renderers.capacity && counted < renderers.count; i++) {
		if (renderers[i] == nullptr) continue;
		if (cameraFrustum.intersect(renderers[i]->getWorldBounds()) || renderers[i]->getWorldBounds().min == Vec3f::nan || renderers[i]->getWorldBounds().max == Vec3f::nan) {
			visibleRenderers.add(i);
		}
		counted++;
	}

	// Camera Globals Update
	globalUniformBuffer->getLayout(1).setMember(0, camera->getProjectionMatrix());
	globalUniformBuffer->getLayout(1).setMember(1, camera->getProjectionMatrix().inversePerspective());
	globalUniformBuffer->getLayout(1).setMember(2, camera->getViewMatrix());
	globalUniformBuffer->getLayout(1).setMember(3, camera->getViewMatrix().inverseAffine());
	globalUniformBuffer->getLayout(1).setMember(4, camera->getNearPlane());
	globalUniformBuffer->getLayout(1).setMember(5, camera->getFarPlane());
	globalUniformBuffer->getLayout(1).setMember(6, Vec2i(width, height));
	globalUniformBuffer->getLayout(1).setMember(7, samples);
	globalUniformBuffer->updateLayout(1);

	for (u32 passIndex = 0; passIndex < renderPasses.count; passIndex++) {
		RenderPass* renderPass = renderPasses[passIndex];
		#ifdef _DEBUG // May be overkill
		glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, (GLsizei) renderPass->name.length(), renderPass->name.c_str());
		#endif
		renderPass->render(camera, visibleRenderers);
		#ifdef _DEBUG
		glPopDebugGroup();
		#endif
	}
	
}

void Pipeline::resizeFrameBuffer(u32 width, u32 height) {
	this->width = width;
	this->height = height;
	glViewport(0, 0, width, height);
	aspectRatio = ((float) width) / ((float) height);
	u32 adjusted = 0;
	for (u32 i = 0; i < cameras.capacity && adjusted < cameras.count; i++) {
		if (cameras[i] == nullptr) continue;
		cameras[i]->setAspect(aspectRatio);
		adjusted++;
	}
	for (u32 passIndex = 0; passIndex < renderPasses.count; passIndex++) {
		RenderPass* renderPass = renderPasses[passIndex];
		renderPass->onResize(width, height);
	}
}

u32 Pipeline::registerRenderer(Renderer* renderer) {
	return renderers.add(renderer);
}

void Pipeline::unregisterRenderer(u32 id) {
	renderers.remove(id);
}

u32 Pipeline::registerCamera(Camera* camera) {
	return cameras.add(camera);
}

void Pipeline::unregisterCamera(u32 id) {
	cameras.remove(id);
}

RenderPass* Pipeline::getRenderPass(std::string name) {
	for (u32 i = 0; i < renderPasses.count; i++) {
		if (renderPasses[i]->name == name) {
			return renderPasses[i];
		}
	}
	return nullptr;
}
