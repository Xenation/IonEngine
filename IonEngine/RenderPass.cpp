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
#include "Mesh.h"
#include "ShaderStorageBuffer.h"
#include "Texture.h"
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



/* ==== LIGHT ASSIGN ==== */
RenderPassLightAssign::RenderPassLightAssign(Pipeline* pipeline) : RenderPass("light-assign", pipeline) {
	lightAssignBuffer = new Framebuffer("LightAssign", 48, 24);
	lightAssignBuffer->createAttachments(1, new Framebuffer::Attachment[1]{Framebuffer::Attachment(GL_COLOR_ATTACHMENT0, GL_RG, GL_RG16F)});
	//lightAssignBuffer->clearColor = Color::clear;

	clustersBuffer = new ShaderStorageBuffer("LightClusters", 73728 * 4 + 147456 * 4 + 4);
	clustersBuffer->setBlocks(1, new ShaderStorageBlock[1]{ShaderStorageBlock(4, 0, 73728 * 4 + 147456 * 4 + 4)});
	clustersBuffer->uploadToGL();
	clustersBuffer->bindStorageBlock(0);
	clustersBuffer->clearData();
}

RenderPassLightAssign::~RenderPassLightAssign() {
	delete lightAssignBuffer;
}


void RenderPassLightAssign::onShadersInitialized() {
	ShaderProgram* lightAssignShader = ShaderProgram::find("lightassign");
	lightAssignShader->load();
	unsigned int count;
	lightAssignSpecShader = lightAssignShader->getAllSpecializedPrograms(count)[0];
	lightAssignMaterial = new Material("LightAssign", lightAssignSpecShader);
	ltwMatrixLocation = lightAssignSpecShader->getUniformLocation("modelMatrix");
	lightTypeLocation = lightAssignSpecShader->getUniformLocation("lightType");
	lightIdLocation = lightAssignSpecShader->getUniformLocation("lightId");

	LightType::Point->cullingMesh->uploadToGL();
	LightType::Spot->cullingMesh->uploadToGL();
}

void RenderPassLightAssign::render(Camera* camera, const SimpleSet<unsigned int>& visibleRenderers) {
	glEnable(GL_CONSERVATIVE_RASTERIZATION_NV);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
	clustersBuffer->clearData();
	clustersBuffer->bindStorageBlock(0);
	lightAssignBuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	glCullFace(GL_BACK);
	renderLightMeshes();

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glCullFace(GL_FRONT);
	renderLightMeshes();
	glActiveTexture(GL_TEXTURE4);
	lightAssignBuffer->getTexture(0)->bind();
	lightAssignBuffer->unbind();

	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CONSERVATIVE_RASTERIZATION_NV);

}

void RenderPassLightAssign::renderLightMeshes() {
	SimpleSet<Light*>& pointLights = pipeline->lightManager->getPointLights();
	SimpleSet<Light*>& spotLights = pipeline->lightManager->getSpotLights();

	lightAssignSpecShader->use();
	lightAssignMaterial->use();
	lightAssignSpecShader->loadUInt(lightTypeLocation, 0);
	for (unsigned int i = 0; i < pointLights.count; i++) {
		lightAssignSpecShader->loadUInt(lightIdLocation, i);
		lightAssignSpecShader->loadMatrix4x4f(ltwMatrixLocation, Matrix4x4f::transformation(pointLights[i]->getPosition(), Vec3f::one * pointLights[i]->range * 2.0f, Rotor3f::identity));
		pointLights[i]->getType()->cullingMesh->render();
	}

	lightAssignSpecShader->loadUInt(lightTypeLocation, 1);
	for (unsigned int i = 0; i < spotLights.count; i++) {
		lightAssignSpecShader->loadUInt(lightIdLocation, i);
		float endRadius = tanf(spotLights[i]->angle * 0.5) * spotLights[i]->range;
		lightAssignSpecShader->loadMatrix4x4f(ltwMatrixLocation, Matrix4x4f::transformation(spotLights[i]->getPosition(), Vec3f(endRadius, endRadius, spotLights[i]->range), Rotor3f(Vec3f::forward, spotLights[i]->getDirection())));
		spotLights[i]->getType()->cullingMesh->render();
	}
	lightAssignSpecShader->unuse();

}



/* ==== SHADOWS ==== */
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

RenderPassOpaque::~RenderPassOpaque() {
	delete renderBuffer;
}


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
