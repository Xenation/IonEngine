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

#define RENDERERS_START_SIZE 32
#define RENDERERS_INCREASE 32
#define CAMERAS_START_SIZE 1
#define CAMERAS_INCREASE 1
#define RENDERPASSES_START_SIZE 4
#define RENDERPASSES_INCREASE 1



Pipeline::Pipeline(int width, int height, LightManager* lightManager)
	: lightManager(lightManager), renderers(RENDERERS_START_SIZE, RENDERERS_INCREASE), cameras(CAMERAS_START_SIZE, CAMERAS_INCREASE), renderPasses(RENDERPASSES_START_SIZE, RENDERERS_INCREASE) {

	renderBuffer = new Framebuffer("RenderBuffer", width, height, samples);
	renderBuffer->createAttachments(2, new Framebuffer::Attachment[2]{Framebuffer::Attachment(GL_COLOR_ATTACHMENT0, GL_RGBA), Framebuffer::Attachment(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT)});
	renderBuffer->clearColor = Color(0.52f, 0.80f, 0.97f, 1); //135-206-250
	resizeFrameBuffer(width, height);

	renderPasses.add(new RenderPassOpaque("opaque"));
	renderPasses.add(new RenderPassTransparent("transparent"));
	//renderPasses.add(new RenderPassPostprocess("postprocess", renderBuffer));

	SpecializedShaderProgram::initialize(this);
	ShaderProgram::initializeAll(this);

	globalUniformBuffer = new UniformBuffer("Global");
	globalUniformBuffer->setLayouts(2, new UniformLayout[2]{UniformLayout(1, 4, new GLSLType[4]{GLSL_MAT4, GLSL_MAT4, GLSL_IVEC2, GLSL_UINT}), UniformLayout(2, 3, new GLSLType[3]{GLSL_FLOAT, GLSL_VEC4, GLSL_VEC4})});
	globalUniformBuffer->uploadToGL();
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

Pipeline::~Pipeline() {
	delete globalUniformBuffer;
	delete renderBuffer;
}


void Pipeline::render() {
	// Globals Update
	Light* mainDirectional = lightManager->getMainDirectional();
	globalUniformBuffer->getLayout(1).setMember(0, Time::time);
	globalUniformBuffer->getLayout(1).setMember(1, mainDirectional->color.vec);
	globalUniformBuffer->getLayout(1).setMember(2, toVec4f(mainDirectional->getDirection()));
	globalUniformBuffer->updateLayout(1);

	unsigned int rendered = 0;
	for (unsigned int i = 0; i < cameras.capacity && rendered < cameras.count; i++) {
		if (cameras[i] == nullptr) continue;
		render(cameras[i]);
		rendered++;
	}
}

void Pipeline::render(Camera* camera) {
	// Frustum Culling
	Frustum3f cameraFrustum = camera->getFrustum();
	SimpleSet<unsigned int> visibleRenderers(32, 32);
	unsigned int counted = 0;
	for (unsigned int i = 0; i < renderers.capacity && counted < renderers.count; i++) {
		if (renderers[i] == nullptr) continue;
		if (cameraFrustum.intersect(renderers[i]->getWorldBounds())) {
			visibleRenderers.add(i);
		}
		counted++;
	}

	// Camera Globals Update
	globalUniformBuffer->getLayout(0).setMember(0, camera->getProjectionMatrix());
	globalUniformBuffer->getLayout(0).setMember(1, camera->getViewMatrix());
	globalUniformBuffer->getLayout(0).setMember(2, Vec2i(renderBuffer->getWidth(), renderBuffer->getHeight()));
	globalUniformBuffer->getLayout(0).setMember(3, samples);
	globalUniformBuffer->updateLayout(0);

	renderBuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	for (unsigned int passIndex = 0; passIndex < renderPasses.count; passIndex++) {
		RenderPass* renderPass = renderPasses[passIndex];
#ifdef _DEBUG // May be overkill
		glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, renderPass->name.length(), renderPass->name.c_str());
#endif
		renderPass->render(visibleRenderers);
#ifdef _DEBUG
		glPopDebugGroup();
#endif
	}
	renderBuffer->unbind();
	renderBuffer->blitTo(nullptr);
}

void Pipeline::resizeFrameBuffer(int width, int height) {
	glViewport(0, 0, width, height);
	aspectRatio = ((float) width) / ((float) height);
	unsigned int adjusted = 0;
	for (unsigned int i = 0; i < cameras.capacity && adjusted < cameras.count; i++) {
		if (cameras[i] == nullptr) continue;
		cameras[i]->setAspect(aspectRatio);
		adjusted++;
	}
	renderBuffer->resize(width, height);
	for (unsigned int passIndex = 0; passIndex < renderPasses.count; passIndex++) {
		RenderPass* renderPass = renderPasses[passIndex];
		renderPass->onResize(width, height);
	}
}

unsigned int Pipeline::registerRenderer(Renderer* renderer) {
	return renderers.add(renderer);
}

void Pipeline::unregisterRenderer(unsigned int id) {
	renderers.remove(id);
}

unsigned int Pipeline::registerCamera(Camera* camera) {
	return cameras.add(camera);
}

void Pipeline::unregisterCamera(unsigned int id) {
	cameras.remove(id);
}

RenderPass* Pipeline::getRenderPass(std::string name) {
	for (int i = 0; i < renderPasses.count; i++) {
		if (renderPasses[i]->name == name) {
			return renderPasses[i];
		}
	}
	return nullptr;
}
