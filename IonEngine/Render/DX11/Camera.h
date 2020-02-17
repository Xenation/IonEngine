#pragma once
//// ==== ==== THIS IS A TEMPORARY FILE FOR TESTING, IT SHOULD EVENTUALLY BE REMOVED ==== ==== ////
#include "Core/MathAdv.h"

namespace IonEngine {
	class Camera {
	public:
		Camera();
		~Camera();
		Camera(const Camera&) = delete;
		void operator=(const Camera&) = delete;

		void setPosition(Vec3f pos);
		void setRotation(Vec3f rot);

		Vec3f getPosition();
		Vec3f getRotation();

		void render();
		void getViewMatrix(Matrix4x4f& viewMatrix);

	private:
		Vec3f pos;
		Vec3f rot;
		Matrix4x4f viewMatrix;
	};
}
