#pragma once
#include "XMath.h"

namespace IonEngine {
	class Framebuffer;
	class Light;
	class Camera;

	class ShadowCaster {
	public:
		const Light*const light;

		ShadowCaster(const Light* light);
		~ShadowCaster();

		unsigned int getAtlasIndex() const { return atlasIndex; }
		void setAtlasIndex(unsigned int index) { atlasIndex = index; }
		Vec4i getAtlasViewport() const { return viewport; }
		Vec4f getAtlasCoords() const;
		void setAtlasViewport(Vec4i viewport) { this->viewport = viewport; }
		Matrix4x4f updateWorldToLightMatrix(Camera* camera);
		Matrix4x4f getWorldToLightMatrix() const { return worldToLight; }

	private:
		unsigned int atlasIndex = 0;
		Vec4i viewport;
		Matrix4x4f worldToLight;
	};
}
