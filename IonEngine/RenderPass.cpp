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
#include "UniformBuffer.h"
#include "Transform.h"
#include "ShadowCaster.h"
#include "ShadowAtlas.h"
#include "Time.h" //
using namespace IonEngine;



RenderPass::RenderPass(const char* name, Pipeline* pipeline) : name(name), pipeline(pipeline), programs(4, 16) {}

RenderPass::~RenderPass() {}


void RenderPass::render(Camera* camera, const SimpleSet<unsigned int>& visibleRenderers) {
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



/* ==== SHADOWS ====*/
RenderPassShadows::RenderPassShadows(Pipeline* pipeline, HollowSet<Renderer*>& renderers) : RenderPass("shadows", pipeline), renderers(renderers) {
	
}

RenderPassShadows::~RenderPassShadows() {
	
}


void RenderPassShadows::onShadersInitialized() {
	pipeline->shadowAtlas->initializeShaders();
}

void RenderPassShadows::render(Camera* camera, const SimpleSet<unsigned int>& visibleRenderers) {
	pipeline->shadowAtlas->renderShadows(camera, renderers);
}



/* ==== OPAQUE ==== */
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
	if (pipeline->shadowAtlas != nullptr) {
		ShadowAtlas* shadowAtlas = pipeline->shadowAtlas;
		deferredMaterial->setTextureByUnit(3, shadowAtlas->getTexture());
		
	}

	SimpleSet<Light*>& directionals = pipeline->lightManager->getDirectionalLights();
	SimpleSet<Light*>& points = pipeline->lightManager->getPointLights();
	SimpleSet<Light*>& spots = pipeline->lightManager->getSpotLights();

	deferredMaterial->setField(0, directionals.count);
	for (unsigned int i = 0; i < directionals.count; i++) {
		deferredMaterial->setField(1, i, directionals[i]->color);
		deferredMaterial->setField(2, i, toVec4f(directionals[i]->getDirection()));
		Vec4i indicesSlice = deferredMaterial->getUniformLayout()->getVec4i(10, i);
		if (directionals[i]->isCastingShadow()) {
			indicesSlice.x = directionals[i]->getShadowCaster()->getAtlasIndex();
			deferredMaterial->setField(11, indicesSlice.x, directionals[i]->getShadowCaster()->getAtlasCoords());
			deferredMaterial->setField(12, indicesSlice.x, directionals[i]->getShadowCaster()->getWorldToLightMatrix());
		} else {
			indicesSlice.x = -1;
		}
		deferredMaterial->setField(10, i, indicesSlice);
	}
	deferredMaterial->setField(3, points.count);
	for (unsigned int i = 0; i < points.count; i++) {
		deferredMaterial->setField(4, i, points[i]->color);
		Vec3f pos = points[i]->getPosition();
		deferredMaterial->setField(5, i, Vec4f(pos.x, pos.y, pos.z, points[i]->range));
		Vec4i indicesSlice = deferredMaterial->getUniformLayout()->getVec4i(10, i);
		if (points[i]->isCastingShadow()) {
			indicesSlice.y = points[i]->getShadowCaster()->getAtlasIndex();
			deferredMaterial->setField(11, indicesSlice.y, points[i]->getShadowCaster()->getAtlasCoords());
			deferredMaterial->setField(12, indicesSlice.y, points[i]->getShadowCaster()->getWorldToLightMatrix());
		} else {
			indicesSlice.y = -1;
		}
		deferredMaterial->setField(10, i, indicesSlice);
	}
	deferredMaterial->setField(6, spots.count);
	for (unsigned int i = 0; i < spots.count; i++) {
		deferredMaterial->setField(7, i, spots[i]->color);
		Vec3f pos = spots[i]->getPosition();
		deferredMaterial->setField(8, i, Vec4f(pos.x, pos.y, pos.z, spots[i]->range));
		Vec2f dir = encodeNormal(spots[i]->getDirection());
		deferredMaterial->setField(9, i, Vec4f(dir.x, dir.y, spots[i]->innerAngle, spots[i]->angle));
		Vec4i indicesSlice = deferredMaterial->getUniformLayout()->getVec4i(10, i);
		if (spots[i]->isCastingShadow()) {
			indicesSlice.z = spots[i]->getShadowCaster()->getAtlasIndex();
			deferredMaterial->setField(11, indicesSlice.z, spots[i]->getShadowCaster()->getAtlasCoords());
			deferredMaterial->setField(12, indicesSlice.z, spots[i]->getShadowCaster()->getWorldToLightMatrix());
		} else {
			indicesSlice.z = -1;
		}
		deferredMaterial->setField(10, i, indicesSlice);
	}

	// Test data
	//deferredMaterial->setField(6, 25);
	//for (unsigned int i = 0; i < 25; i++) {
	//	Color color = Color::white;
	//	color.a = 2.0f;
	//	deferredMaterial->setField(7, color, i);
	//}
	//for (unsigned int i = 0; i < 25; i++) {
	//	deferredMaterial->setField(8, Vec4f(10 + (i % 5) * 7, -3, 10 + (i / 5) * 7, 20), i);
	//}
	//for (unsigned int i = 0; i < 25; i++) {
	//	//Vec2f dir = encodeNormal(Vec3f(0.577350f, -0.577350f, 0.577350f));
	//	Vec2f dir = encodeNormal(Vec3f(cosf(Time::time), -1, sinf(Time::time)).normalized());
	//	deferredMaterial->setField(9, Vec4f(dir.x, dir.y, 1.099557f, 1.256637f), i);
	//}
}



/* ==== TRANSARENT ==== */
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

void RenderPassPostprocess::render(Camera* camera, const SimpleSet<unsigned int>& visibleRenderers) {
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
