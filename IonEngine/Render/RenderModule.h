#pragma once
#include "Core/Module.h"

namespace IonEngine {
	class RenderContext;
	class Window;

	class RenderModule : public Module {
	public:
		RenderModule(EngineCore* engine);
		virtual ~RenderModule();

		void initialize(Window* window);
		void render();

	private:
		RenderContext* context;
	};
}
