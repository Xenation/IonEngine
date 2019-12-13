#include "ShadowAtlas.h"

#include "Framebuffer.h"
#include "ShadowCaster.h"
#include "Light.h"
#include "SpecializedShaderProgram.h"
#include "ShaderProgram.h"
#include "Material.h"
#include "Renderer.h"
#include "Entity.h"
using namespace IonEngine;



ShadowAtlas::ShadowAtlas() : directionalCasters(1, 1), spotCasters(16, 16) {
	size = Vec2i(4096, 4096);
	shadowAtlas = new Framebuffer("ShadowAtlas", size.x, size.y);
	Framebuffer::Attachment::Descriptor depthDesc;
	depthDesc.format = GL_DEPTH_COMPONENT;
	depthDesc.internalFormat = GL_DEPTH_COMPONENT24;
	depthDesc.minFilter = GL_LINEAR;
	depthDesc.magFilter = GL_LINEAR;
	depthDesc.compareMode = GL_COMPARE_REF_TO_TEXTURE;
	depthDesc.borderColor = Color::white;
	shadowAtlas->createAttachments(1, new Framebuffer::Attachment[1]{Framebuffer::Attachment(GL_DEPTH_ATTACHMENT, depthDesc)});
}

ShadowAtlas::~ShadowAtlas() {
	delete shadowAtlas;
}


void ShadowAtlas::initializeShaders() {
	ShaderProgram* depthShader = ShaderProgram::find("depth");
	depthShader->load();
	unsigned int count;
	depthSpecShader = depthShader->getAllSpecializedPrograms(count)[0];
	depthMaterial = new Material("Depth", depthSpecShader);
	worldToLightMatrixLocation = depthSpecShader->getUniformLocation("worldToLightMatrix");
	localToWorldMatrixLocation = depthSpecShader->getUniformLocation("localToWorldMatrix");
}

void ShadowAtlas::renderShadows(Camera* camera, HollowSet<Renderer*>& renderers) {
	// Prepare Scene Render
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glCullFace(GL_FRONT);

	shadowAtlas->bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	depthSpecShader->use();
	depthMaterial->use();
	// Directional Casters
	for (unsigned int ci = 0; ci < directionalCasters.count; ci++) {
		#ifdef _DEBUG
		glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, (GLsizei) directionalCasters[ci]->light->entity->name.length(), directionalCasters[ci]->light->entity->name.c_str());
		#endif
		// Setup
		directionalCasters[ci]->setAtlasIndex(ci);
		Vec4i viewport = getDirectionalViewport(ci);
		directionalCasters[ci]->setAtlasViewport(viewport);
		glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
		depthSpecShader->loadMatrix4x4f(worldToLightMatrixLocation, directionalCasters[ci]->updateWorldToLightMatrix(camera));
		// Render scene depth
		unsigned int counted = 0;
		for (unsigned int i = 0; i < renderers.capacity && counted < renderers.count; i++) {
			if (renderers[i] == nullptr) continue;
			depthSpecShader->loadMatrix4x4f(localToWorldMatrixLocation, renderers[i]->entity->transform->getLocalToWorldMatrix());
			renderers[i]->renderNoUniform();
			counted++;
		}
		#ifdef _DEBUG
		glPopDebugGroup();
		#endif
	}
	// Spot Casters
	for (unsigned int ci = 0; ci < spotCasters.count; ci++) {
		#ifdef _DEBUG
		glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, (GLsizei) spotCasters[ci]->light->entity->name.length(), spotCasters[ci]->light->entity->name.c_str());
		#endif
		// Setup
		spotCasters[ci]->setAtlasIndex(directionalCasters.count + ci);
		Vec4i viewport = getSpotViewport(ci);
		spotCasters[ci]->setAtlasViewport(viewport);
		glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
		depthSpecShader->loadMatrix4x4f(worldToLightMatrixLocation, spotCasters[ci]->updateWorldToLightMatrix(camera));
		// Render scene depth
		unsigned int counted = 0;
		for (unsigned int i = 0; i < renderers.capacity && counted < renderers.count; i++) {
			if (renderers[i] == nullptr) continue;
			depthSpecShader->loadMatrix4x4f(localToWorldMatrixLocation, renderers[i]->entity->transform->getLocalToWorldMatrix());
			renderers[i]->renderNoUniform();
			counted++;
		}
		#ifdef _DEBUG
		glPopDebugGroup();
		#endif
	}
	depthSpecShader->unuse();
	shadowAtlas->unbind();

	glCullFace(GL_BACK);
}

void ShadowAtlas::registerShadowCaster(ShadowCaster* caster) {
	switch (caster->light->getType()->id) {
	case LightType::DirectionalId:
		directionalCasters.add(caster);
		break;
	case LightType::PointId:
		// Not supported yet
		break;
	case LightType::SpotId:
		spotCasters.add(caster);
		break;
	}
}

void ShadowAtlas::unregisterShadowCaster(ShadowCaster* caster) {
	switch (caster->light->getType()->id) {
	case LightType::DirectionalId:
		directionalCasters.remove(caster);
		break;
	case LightType::PointId:
		// Not supported yet
		break;
	case LightType::SpotId:
		spotCasters.remove(caster);
		break;
	}
}

Texture* ShadowAtlas::getTexture() {
	return shadowAtlas->getTexture(0);
}

Vec4i ShadowAtlas::getDirectionalViewport(unsigned int index) {
	return Vec4i(index * 2048, 2048, 2048, 2048);
}

Vec4i ShadowAtlas::getSpotViewport(unsigned int index) {
	return Vec4i(index * 512, 1536 - (index / 8) * 512, 512, 512);
}
