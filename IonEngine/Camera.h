#pragma once
#include "Component.h"
#include "XMath.h"
class Entity;
class Camera : public Component {
public:
	Camera(Entity* entity);
	~Camera();

	virtual void onEnable() override;
	virtual void onDisable() override;

	inline float getFov() { return fov; }
	inline float getNearPlane() { return nearPlane; }
	inline float getFarPlane() { return farPlane; }
	Matrix4x4f getProjectionMatrix();
	Matrix4x4f getViewMatrix();

	void setFov(float fov);
	void setNearPlane(float near);
	void setFarPlane(float far);

	void updateProjectionMatrix();

private:
	unsigned int id;

	float fov = 90.0f;
	float nearPlane = 0.01f;
	float farPlane = 1000.0f;
	float aspectRatio = 1.0f;
	Matrix4x4f projectionMatrix;
	bool projectionMatrixExpired = true;
};

