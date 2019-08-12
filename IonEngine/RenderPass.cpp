#include "RenderPass.h"

#include <gl3w.h>
#include "SpecializedShaderProgram.h"
#include "Material.h"
#include "Renderer.h"
#include "Framebuffer.h"
#include "ShaderProgram.h"
#include "Pipeline.h"
#include "LightManager.h"
#include "Light.h"
using namespace IonEngine;



RenderPass::RenderPass(const char* name, Pipeline* pipeline) : name(name), pipeline(pipeline), programs(4, 16) {}

RenderPass::~RenderPass() {}


void RenderPass::render(const SimpleSet<unsigned int>& visibleRenderers) {
	prepare();
	unsigned int shadersRendered = 0;
	for (unsigned int shaderIndex = 0; shaderIndex < programs.capacity && shadersRendered < programs.count; shaderIndex++) {
		SpecializedShaderProgram* shaderProgram = programs[shaderIndex];
		if (shaderProgram == nullptr) continue;
		shaderProgram->use();
		unsigned int materialsRendered = 0;
		for (unsigned int materialIndex = 0; materialIndex < shaderProgram->materials.capacity && materialsRendered < shaderProgram->materials.count; materialIndex++) {
			Material* material = shaderProgram->materials[materialIndex];
			if (material == nullptr) continue;
			material->use();
			unsigned int renderersRendered = 0;
			for (unsigned int rendererIndex = 0; rendererIndex < material->renderers.capacity && rendererIndex < material->renderers.count; rendererIndex++) {
				if (!visibleRenderers.contains(material->renderers[rendererIndex]->getID())) {
					continue; // TODO better culling
				}
				Renderer* renderer = material->renderers[rendererIndex];
				if (renderer == nullptr) continue;
				renderer->render();
			}
		}
		shaderProgram->unuse();
	}
	finish();
}



RenderPassOpaque::RenderPassOpaque(const char* name, Pipeline* pipeline, unsigned int width, unsigned int height, unsigned int samples) : RenderPass(name, pipeline), deferredMaterial(deferredMaterial) {
	renderBuffer = new Framebuffer("RenderBuffer", width, height, samples);
	renderBuffer->createAttachments(3, new Framebuffer::Attachment[3]{Framebuffer::Attachment(GL_COLOR_ATTACHMENT0, GL_RGBA), Framebuffer::Attachment(GL_COLOR_ATTACHMENT1, GL_RGBA), Framebuffer::Attachment(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT)});
	renderBuffer->clearColor = Color(0.52f, 0.80f, 0.97f, 0.0); //135-206-250
}

RenderPassOpaque::~RenderPassOpaque() {}


void RenderPassOpaque::onShadersInitialized() {
	ShaderProgram* deferredShader = ShaderProgram::find("lightpass");
	deferredShader->load();
	unsigned int count;
	deferredSpecShader = deferredShader->getAllSpecializedPrograms(count)[0];
	deferredMaterial = new Material("DeferredLight", deferredSpecShader);
}

void RenderPassOpaque::prepareFrame() {
	updateLightsData();
}

void RenderPassOpaque::prepare() {
	glDisable(GL_BLEND);
	renderBuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void RenderPassOpaque::finish() {
	renderBuffer->unbind();
	deferredSpecShader->use();
	renderBuffer->blitTo(nullptr, deferredMaterial, true);
}

void RenderPassOpaque::onResize(unsigned int width, unsigned int height) {
	renderBuffer->resize(width, height);
}

void RenderPassOpaque::updateLightsData() {
	SimpleSet<Light*>& directionals = pipeline->lightManager->getDirectionalLights();
	SimpleSet<Light*>& points = pipeline->lightManager->getPointLights();
	SimpleSet<Light*>& spots = pipeline->lightManager->getSpotLights();

	deferredMaterial->setField(0, directionals.count);
	for (unsigned int i = 0; i < directionals.count; i++) {
		deferredMaterial->setField(1, directionals[i]->color, i);
	}
	for (unsigned int i = 0; i < directionals.count; i++) {
		deferredMaterial->setField(2, toVec4f(directionals[i]->getDirection()), i);
	}
	deferredMaterial->setField(3, points.count);
	for (unsigned int i = 0; i < points.count; i++) {
		deferredMaterial->setField(4, points[i]->color, i);
	}
	for (unsigned int i = 0; i < points.count; i++) {
		Vec3f pos = points[i]->getPosition();
		deferredMaterial->setField(5, Vec4f(pos.x, pos.y, pos.z, points[i]->range), i);
	}
	deferredMaterial->setField(6, spots.count);
	for (unsigned int i = 0; i < spots.count; i++) {
		deferredMaterial->setField(7, spots[i]->color, i);
	}
	for (unsigned int i = 0; i < spots.count; i++) {
		Vec3f pos = spots[i]->getPosition();
		deferredMaterial->setField(8, Vec4f(pos.x, pos.y, pos.z, spots[i]->range), i);
	}
	for (unsigned int i = 0; i < spots.count; i++) {
		Vec2f dir = encodeNormal(spots[i]->getDirection());
		deferredMaterial->setField(9, Vec4f(dir.x, dir.y, spots[i]->innerAngle, spots[i]->angle), i);
	}

	// Test data
	//deferredMaterial->setField(6, 32);
	//for (unsigned int i = 0; i < 32; i++) {
	//	deferredMaterial->setField(7, Color::white, i);
	//}
	//for (unsigned int i = 0; i < 32; i++) {
	//	deferredMaterial->setField(8, Vec4f((i % 6) * 7, -3, (i / 6) * 7, 10), i);
	//}
	//for (unsigned int i = 0; i < 32; i++) {
	//	deferredMaterial->setField(9, Vec4f(0.707106f, -0.707106f, 0.785398f, 1.570796f), i);
	//}
}



RenderPassTransparent::RenderPassTransparent(const char* name, Pipeline* pipeline) : RenderPass(name, pipeline) {}

RenderPassTransparent::~RenderPassTransparent() {}

void RenderPassTransparent::prepare() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



RenderPassPostprocess::RenderPassPostprocess(const char* name, Pipeline* pipeline, Framebuffer* renderBuffer) : RenderPass(name, pipeline), renderBuffer(renderBuffer) {
	temporary1 = renderBuffer->copy("PostProcess1");
	temporary2 = renderBuffer->copy("PostProcess2");
}

RenderPassPostprocess::~RenderPassPostprocess() {
	delete temporary1;
	delete temporary2;
}

void RenderPassPostprocess::render(const SimpleSet<unsigned int>& visibleRenderers) {
	glDisable(GL_DEPTH_TEST);

	temporary1->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	temporary2->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	temporary2->unbind();

	Framebuffer* currentFramebuffer = temporary1;
	Framebuffer* nextFramebuffer = temporary2;
	renderBuffer->blitTo(currentFramebuffer);
	unsigned int shadersRendered = 0;
	for (unsigned int shaderIndex = 0; shaderIndex < programs.capacity && shadersRendered < programs.count; shaderIndex++) {
		SpecializedShaderProgram* shaderProgram = programs[shaderIndex];
		if (shaderProgram == nullptr) continue;
		shaderProgram->use();
		unsigned int materialsRendered = 0;
		for (unsigned int materialIndex = 0; materialIndex < shaderProgram->materials.capacity && materialsRendered < shaderProgram->materials.count; materialIndex++) {
			Material* material = shaderProgram->materials[materialIndex];
			if (material == nullptr) continue;
			currentFramebuffer->blitTo(nextFramebuffer, material);
			Framebuffer* oldCurrent = currentFramebuffer;
			currentFramebuffer = nextFramebuffer;
			nextFramebuffer = oldCurrent;
		}
		shaderProgram->unuse();
	}
	currentFramebuffer->blitTo(renderBuffer);

	glEnable(GL_DEPTH_TEST);
}

void RenderPassPostprocess::onResize(uint width, uint height) {
	temporary1->resize(width, height);
	temporary2->resize(width, height);
}
