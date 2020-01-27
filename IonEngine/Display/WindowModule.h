﻿#pragma once
#include "Core/Module.h"
#include "Core/MathVec.h"

namespace IonEngine {
	class Window;
	enum class WindowState : u32;
	template<typename T, u32 N>
	class CappedSparseSet;

	class WindowModule : public Module {
	public:
		WindowModule(EngineCore* engine);
		virtual ~WindowModule();

		Window* createWindow(const char* title);
		Window* createWindow(const char* title, WindowState state);
		Window* createWindow(const char* title, Vec2i pos, Vec2i size, WindowState state);
		void destroyWindow(Window* window);
		void poolEvents();

	private:
		CappedSparseSet<Window, 16>* windows;
	};
}
