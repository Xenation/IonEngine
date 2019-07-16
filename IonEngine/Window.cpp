#include "Window.h"

#include "gl3w.h"
#include <GLFW\glfw3.h>
#include "Engine.h"
#include "Debug.h"
#include "Input.h"
#include "Pipeline.h"



void error_callback(int error, const char* description) {
	Debug::logError("GLFW", description);
}


bool Window::isGLFWinitialized = false;
HollowSet<Window*> Window::windows = HollowSet<Window*>(1, 1);

void Window::initializeGLFW() {
	if (isGLFWinitialized) return;
	isGLFWinitialized = true;
	if (!glfwInit()) {
		Debug::fail("Failed to initialize GLFW!");
	} else {
		Debug::log("GLFW initialized");
	}
	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);
}

void Window::resizeFrameCallback(GLFWwindow* window, int width, int height) {
	Engine::pipeline->resizeFrameBuffer(width, height);
	for (unsigned int i = 0; i < windows.capacity; i++) {
		if (windows[i] == nullptr || windows[i]->window != window) continue;
		windows[i]->width = width;
		windows[i]->height = height;
	}
}


Window::Window(const char* title) {
	initializeGLFW();
	window = glfwCreateWindow(640, 480, title, NULL, NULL);
	if (!window) {
		Debug::fail("Failed to create window!");
	} else {
		Debug::log("Window created");
	}
	glfwSetKeyCallback(window, Input::getKeyCallback());
	glfwSetMouseButtonCallback(window, Input::getMouseButtonCallback());
	glfwSetCursorPosCallback(window, Input::getMousePosCallback());
	glfwSetFramebufferSizeCallback(window, resizeFrameCallback);
	glfwSetWindowPos(window, 500, 500);
	glfwGetWindowSize(window, &width, &height);

	glfwMakeContextCurrent(window);
	if (gl3wInit2(glfwGetProcAddress)) {
		Debug::fail("Failed to initialize GL3W!");
	} else {
		Debug::log("GL3W initialized");
	}

	id = windows.add(this);
}

Window::~Window() {
	windows.remove(id);
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::display() {
	glfwSwapBuffers(window);
}

int Window::shouldClose() {
	return glfwWindowShouldClose(window);
}

void Window::closeWindow() {
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}
