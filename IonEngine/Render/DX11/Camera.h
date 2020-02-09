#pragma once
//// ==== ==== THIS IS A TEMPORARY FILE FOR TESTING, IT SHOULD EVENTUALLY BE REMOVED ==== ==== ////
#include <DirectXMath.h>
#include "Core/MathVec.h"

namespace IonEngine {
	using namespace DirectX;

	class Camera {
	public:
		Camera();
		~Camera();
		Camera(const Camera&) = delete;
		void operator=(const Camera&) = delete;

		void setPosition(Vec3f pos);
		void setRotation(Vec3f rot);

		XMFLOAT3 getPosition();
		XMFLOAT3 getRotation();

		void render();
		void getViewMatrix(XMMATRIX& viewMatrix);

	private:
		Vec3f pos;
		Vec3f rot;
		XMMATRIX viewMatrix;
	};
}
