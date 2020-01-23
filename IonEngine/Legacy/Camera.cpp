#include "Camera.h"

#include "Engine.h"
#include "Pipeline.h"
#include "Entity.h"
#include "Transform.h"
using namespace IonEngine;



Camera::Camera(Entity* entity) : Component(entity) {
	
}

Camera::~Camera() {

}

void Camera::onEnable() {
	id = Engine::pipeline->registerCamera(this);
}

void Camera::onDisable() {
	Engine::pipeline->unregisterCamera(id);
}

Matrix4x4f Camera::getProjectionMatrix() {
	if (frustumExpired) {
		updateFrustum();
	}
	return frustum.projectionMatrix;
}

Matrix4x4f Camera::getViewMatrix() {
	Matrix4x4f viewMatrix = entity->transform->getWorldToLocalMatrix();
	frustum.updatePlanes(viewMatrix);
	return viewMatrix;
}

void Camera::setFov(float fov) {
	frustum.horiFOV = fov;
	frustumExpired = true;
}

void Camera::setNearPlane(float near) {
	frustum.near = near;
	frustumExpired = true;
}

void Camera::setFarPlane(float far) {
	frustum.far = far;
	frustumExpired = true;
}

void Camera::setAspect(float aspect) {
	frustum.aspect = aspect;
	frustumExpired = true;
}

void Camera::updateFrustum() {
	frustum.aspect = Engine::pipeline->getAspectRatio();
	frustum.updateProjectionMatrix();
	frustumExpired = false;
}
