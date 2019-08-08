#pragma once
#include "Component.h"
#include "XMath.h"
#undef near
#undef far

namespace IonEngine {
	class Entity;

	class Camera : public Component {
	public:
		Camera(Entity* entity);
		~Camera();

		virtual void onEnable() override;
		virtual void onDisable() override;

		inline float getFov() { return frustum.horiFOV; }
		inline float getNearPlane() { return frustum.near; }
		inline float getFarPlane() { return frustum.far; }
		inline float getAspect() { return frustum.aspect; }
		Matrix4x4f getProjectionMatrix();
		Matrix4x4f getViewMatrix();
		inline Frustum3f getFrustum() { return frustum; }

		void setFov(float fov);
		void setNearPlane(float near);
		void setFarPlane(float far);
		void setAspect(float aspect);

	private:
		unsigned int id;

		Frustum3f frustum = Frustum3f(0.1f, 1000.0f, 100.0f, 1.0f);
		bool frustumExpired = true;

		void updateFrustum();
	};
}
