#pragma once
#include "XMath.h"
#include "SimpleSet.h"
#include "HollowSet.h"

namespace IonEngine {
	class ShadowCaster;
	class Framebuffer;
	class Texture;
	class Material;
	class SpecializedShaderProgram;
	class Renderer;
	class Camera;

	class ShadowAtlas {
	public:
		ShadowAtlas();
		~ShadowAtlas();

		void initializeShaders();

		void renderShadows(Camera* camera, HollowSet<Renderer*>& renderers);

		void registerShadowCaster(ShadowCaster* caster);
		void unregisterShadowCaster(ShadowCaster* caster);

		Texture* getTexture();
		Vec2i getSize() const { return size; }

	private:
		Framebuffer* shadowAtlas;
		Vec2i size;
		Material* depthMaterial;
		unsigned int worldToLightMatrixLocation = 0;
		SpecializedShaderProgram* depthSpecShader;
		SimpleSet<ShadowCaster*> directionalCasters;
		SimpleSet<ShadowCaster*> spotCasters;

		Vec4i getDirectionalViewport(unsigned int index);
		Vec4i getSpotViewport(unsigned int index);
	};
}
