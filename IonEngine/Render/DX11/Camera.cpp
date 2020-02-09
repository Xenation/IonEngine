#include "Camera.h"

using namespace IonEngine;



Camera::Camera() {
	pos = Vec3f::zero;
	rot = Vec3f::zero;
}

Camera::~Camera() {

}


void Camera::setPosition(Vec3f pos) {
	this->pos = pos;
}

void Camera::setRotation(Vec3f rot) {
	this->rot = rot;
}

XMFLOAT3 Camera::getPosition() {
	return XMFLOAT3(pos.x, pos.y, pos.z);
}

XMFLOAT3 Camera::getRotation() {
	return XMFLOAT3(rot.x, rot.y, rot.z);
}

void Camera::render() {
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVec, positionVec, lookAtVec;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	upVec = XMLoadFloat3(&up);

	position.x = pos.x;
	position.y = pos.y;
	position.z = pos.z;
	positionVec = XMLoadFloat3(&position);

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;
	lookAtVec = XMLoadFloat3(&lookAt);

	pitch = rot.x * 0.0174532925f;
	yaw = rot.y * 0.0174532925f;
	roll = rot.z * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin
	lookAtVec = XMVector3TransformCoord(lookAtVec, rotationMatrix);
	upVec = XMVector3TransformCoord(upVec, rotationMatrix);

	// Translate the rotated camera positionto the location of the viewer
	lookAtVec = XMVectorAdd(positionVec, lookAtVec);

	// Create the view matrix from the three updated vectors
	viewMatrix = XMMatrixLookAtLH(positionVec, lookAtVec, upVec);
}

void Camera::getViewMatrix(XMMATRIX& viewMatrix) {
	viewMatrix = this->viewMatrix;
}
