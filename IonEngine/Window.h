#pragma once
#include "HollowSet.h"

struct GLFWwindow;

namespace IonEngine {
	class Renderer;

	class Window {
	public:
		Window(const char* title = "IonEngine");
		~Window();

		void display();
		i32 shouldClose();
		void closeWindow();

		void setTitle(const char* title);
		void enableVSync();
		void disableVSync();

		inline i32 getWidth() { return width; }
		inline i32 getHeight() { return height; }
		inline GLFWwindow* glfwWindow() { return window; }

	private:
		static bool isGLFWinitialized;
		static void initializeGLFW();
		static void resizeFrameCallback(GLFWwindow* window, i32 width, i32 height);
		static HollowSet<Window*> windows;

		GLFWwindow* window;
		u32 id;
		i32 width;
		i32 height;

		Window(const Window& w) {};
	};
}
