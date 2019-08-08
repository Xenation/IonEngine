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

	class Pipeline {
	public:
		SimpleList<RenderPass*> renderPasses;

		Pipeline(int width, int height, LightManager* lightManager);
		Pipeline(const Pipeline&) = delete;
		~Pipeline();

		void render();
		void resizeFrameBuffer(int width, int height);
		inline float getAspectRatio() { return aspectRatio; }

		unsigned int registerRenderer(Renderer* renderer);
		void unregisterRenderer(unsigned int id);
		unsigned int registerCamera(Camera* camera);
		void unregisterCamera(unsigned int id);

		RenderPass* getRenderPass(std::string name);
		HollowSet<Renderer*>* getRenderersSet() { return &renderers; } // Unsure

	private:
		HollowSet<Renderer*> renderers;
		HollowSet<Camera*> cameras;
		UniformBuffer* globalUniformBuffer;
		Framebuffer* renderBuffer;
		float aspectRatio = 1.0f;
		unsigned int samples = 4;
		LightManager* lightManager;

		void render(Camera* camera);
	};
}
