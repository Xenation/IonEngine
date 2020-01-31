#include "ShadowCaster.h"

#include "Light.h"
#include "Transform.h"
#include "Engine.h"
#include "Pipeline.h"
#include "ShadowAtlas.h"
#include "Camera.h"
using namespace IonEngine;



ShadowCaster::ShadowCaster(const Light* light) : light(light) {
	Engine::pipeline->shadowAtlas->registerShadowCaster(this);
}

ShadowCaster::~ShadowCaster() {
	Engine::pipeline->shadowAtlas->unregisterShadowCaster(this);
}


Vec4f ShadowCaster::getAtlasCoords() const {
	Vec2f atlasSize = toVec2f(Engine::pipeline->shadowAtlas->getSize());
	return Vec4f(viewport.x / atlasSize.x, viewport.y / atlasSize.y, viewport.z / atlasSize.x, viewport.w / atlasSize.y);
}

Matrix4x4f ShadowCaster::updateWorldToLightMatrix(Camera* camera) {
	Matrix4x4f viewMatrix = Matrix4x4f::identity;
	Matrix4x4f projectionMatrix = Matrix4x4f::identity;

	switch (light->getType()->id) {
	case LightType::DirectionalId:
		viewMatrix = light->transform->getLocalToWorldMatrix();
		viewMatrix.translation() = -light->getDirection() * 50.0f; // TODO reimplement camera offset while avoiding flicker
		viewMatrix = viewMatrix.inverseAffine();
		projectionMatrix = Matrix4x4f::orthogonalProjection(25.0f, -25.0f, 25.0f, -25.0f, 0.01f, 100.0f);
		break;
	case LightType::SpotId:
		viewMatrix = light->transform->getWorldToLocalMatrix();
		projectionMatrix = Matrix4x4f::perspectiveProjection(light->angle * rad2deg, 1.0f, 0.01f, light->range);
		break;
	}

	worldToLight = projectionMatrix * viewMatrix;
	return worldToLight;
}
