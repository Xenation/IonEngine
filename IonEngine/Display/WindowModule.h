#pragma once
#include "../Core/Module.h"
#include "../Core/IonMath.h"

namespace IonEngine {
	class Window;
	enum class WindowState : u32;
	template<typename T>
	class BulkHollowSet;

	class WindowModule : Module {
	public:
		WindowModule(EngineCore* engine);
		~WindowModule();

		Window* createWindow(const char* title);
		Window* createWindow(const char* title, WindowState state);
		Window* createWindow(const char* title, Vec2i pos, Vec2i size, WindowState state);
		void poolEvents();

	private:
		BulkHollowSet<Window>* windows;
	};
}
