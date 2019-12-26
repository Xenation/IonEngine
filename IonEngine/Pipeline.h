#pragma once
#include <string>
#include "HollowSet.h"
#include "SimpleList.h"


namespace IonEngine {
	class Renderer;
	class Camera;
	class UniformBuffer;
	class RenderPass;
	class Framebuffer;
	class LightManager;
	class Material;
	class SpecializedShaderProgram;
	class ShadowAtlas;

	class Pipeline {
	public:
		SimpleList<RenderPass*> renderPasses;
		LightManager* lightManager; // Maybe hide
		ShadowAtlas* shadowAtlas; // Maybe hide

		Pipeline(u32 width, u32 height, LightManager* lightManager);
		Pipeline(const Pipeline&) = delete;
		~Pipeline();

		void render();
		void resizeFrameBuffer(u32 width, u32 height);
		inline float getAspectRatio() { return aspectRatio; }

		u32 registerRenderer(Renderer* renderer);
		void unregisterRenderer(u32 id);
		u32 registerCamera(Camera* camera);
		void unregisterCamera(u32 id);

		RenderPass* getRenderPass(std::string name);
		HollowSet<Renderer*>* getRenderersSet() { return &renderers; } // Unsure

	private:
		HollowSet<Renderer*> renderers;
		HollowSet<Camera*> cameras;
		UniformBuffer* globalUniformBuffer;
		float aspectRatio = 1.0f;
		u32 width;
		u32 height;
		u32 samples = 4;

		void render(Camera* camera);
	};
}
