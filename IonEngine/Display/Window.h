#pragma once
#include "Core/IonMath.h"

struct GLFWwindow;

namespace IonEngine {
	ION_ENUM(WindowState, u32, 
		WINDOWED,
		WINDOWED_MAXIMIZED,
		FULLSCREEN_WINDOW,
		FULLSCREEN_EXCLUSIVE,
	)

	class Window {
	public:
		/*enum class State : u32 {
			WINDOWED,
			WINDOWED_MAXIMIZED,
			FULLSCREEN_WINDOW,
			FULLSCREEN_EXCLUSIVE,
		};*/

		/* ==== CONSTRUCTORS ==== */
		Window(const Window&) = delete;
		void operator=(const Window&) = delete;

		/* ==== MANIPULATION ==== */
		void setTitle(const char* title);
		Vec2i getPosition() const;
		void setPosition(const Vec2i& pos);
		Vec2i getSize() const;
		void setSize(const Vec2i& size);
		Vec2i getFramebufferSize() const;
		bool isFullscreen() const;
		WindowState getState() const;
		inline void setState(WindowState nState) { setState(nState, Vec2i::zero); }
		inline void setState(WindowState nState, Vec2i size) { setState(nState, size, Vec2i::min); }
		void setState(WindowState nState, Vec2i size, Vec2i pos);
		bool shouldClose();

		bool nextTMP(u32 idx); // TODO FOR TEST ONLY, TO REMOVE

	private:
		GLFWwindow* windowHandle;
		WindowState state;

		/* ==== CONSTRUCTORS ==== */
		friend class WindowModule;
		Window(const char* title) : Window(title, WindowState::WINDOWED_MAXIMIZED) {}
		Window(const char* title, WindowState state) : Window(title, Vec2i::min, Vec2i(0, 0), state) {}
		Window(const char* title, Vec2i pos, Vec2i size, WindowState state);
		~Window();

		static void framebufferResizeCallback(GLFWwindow* windowHandle, int width, int height);
		static void resizeCallback(GLFWwindow* windowHandle, int width, int height);
		static void positionCallback(GLFWwindow* windowHandle, int posx, int posy);
	};
}
