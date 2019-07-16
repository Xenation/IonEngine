#pragma once
#include "HollowSet.h"
struct GLFWwindow;
class Renderer;
class Window {
public:
	Window(const char* title = "IonEngine");
	~Window();

	void display();
	int shouldClose();
	void closeWindow();

	inline int getWidth() { return width; }
	inline int getHeight() { return height; }
	inline GLFWwindow* glfwWindow() { return window; }

private:
	static bool isGLFWinitialized;
	static void initializeGLFW();
	static void resizeFrameCallback(GLFWwindow* window, int width, int height);
	static HollowSet<Window*> windows;

	GLFWwindow* window;
	unsigned int id;
	int width;
	int height;

	Window(const Window& w) {};
};

