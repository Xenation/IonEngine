#pragma once
#include "Core/Module.h"

namespace IonEngine {
	class RenderContext;
	class Window;
	class Camera;
	class Mesh;
	class Shader;

	class RenderModule : public Module {
	public:
		RenderModule(EngineCore* engine);
		virtual ~RenderModule();

		void initialize(Window* window);
		void render();
		void shutdown();

	private:
		RenderContext* context;
		Camera* camera;
		Mesh* mesh;
		Shader* shader;
	};
}
