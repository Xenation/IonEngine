#include "Camera.h"

#include "Engine.h"
#include "Pipeline.h"
#include "Entity.h"
#include "Transform.h"



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
	if (projectionMatrixExpired) {
		updateProjectionMatrix();
	}
	return projectionMatrix;
}

Matrix4x4f Camera::getViewMatrix() {
	return entity->transform->getWorldToLocalMatrix();
}

void Camera::setFov(float fov) {
	this->fov = fov;
}

void Camera::setNearPlane(float near) {
	nearPlane = near;
}

void Camera::setFarPlane(float far) {
	farPlane = far;
}

void Camera::updateProjectionMatrix() {
	aspectRatio = Engine::pipeline->getAspectRatio();
	projectionMatrix = Matrix4x4f::perspectiveProjection(fov, aspectRatio, nearPlane, farPlane);
	projectionMatrixExpired = false;
}
