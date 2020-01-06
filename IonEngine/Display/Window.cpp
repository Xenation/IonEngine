#include "Window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
using namespace IonEngine;



bool kPressed[32];
void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key >= 48 && key <= 57) {
			kPressed[key - 48] = true;
		}
	}
}

Window::Window(const char* title, Vec2i position, Vec2i size, WindowState state) : state(state) {
	// Maximize flag
	if (state == WindowState::WINDOWED_MAXIMIZED) {
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	} else {
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
	}

	// Query primary display attribs
	const GLFWvidmode* dispMode = glfwGetVideoMode(glfwGetPrimaryMonitor()); // TODO configurable display choice

	// Auto Sizing
	if (size.x <= 0 || size.y <= 0 || state == WindowState::FULLSCREEN_WINDOW) {
		switch (state) {
		case WindowState::WINDOWED: // Make it half display size
			glfwWindowHint(GLFW_RED_BITS, GLFW_DONT_CARE);
			glfwWindowHint(GLFW_GREEN_BITS, GLFW_DONT_CARE);
			glfwWindowHint(GLFW_BLUE_BITS, GLFW_DONT_CARE);
			glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
			size.x = dispMode->width / 2;
			size.y = dispMode->height / 2;
			break;
		case WindowState::FULLSCREEN_EXCLUSIVE:
		case WindowState::FULLSCREEN_WINDOW:
			glfwWindowHint(GLFW_RED_BITS, dispMode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, dispMode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, dispMode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, dispMode->refreshRate);
			size.x = dispMode->width;
			size.y = dispMode->height;
			break;
		case WindowState::WINDOWED_MAXIMIZED:
		default: // Doesn't matter
			glfwWindowHint(GLFW_RED_BITS, GLFW_DONT_CARE);
			glfwWindowHint(GLFW_GREEN_BITS, GLFW_DONT_CARE);
			glfwWindowHint(GLFW_BLUE_BITS, GLFW_DONT_CARE);
			glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
			size.x = 1;
			size.y = 1;
			break;
		}
	}

	// Create the glfw window
	if (state == WindowState::FULLSCREEN_EXCLUSIVE || state == WindowState::FULLSCREEN_WINDOW) {
		windowHandle = glfwCreateWindow(size.x, size.y, title, glfwGetPrimaryMonitor(), nullptr);
	} else {
		windowHandle = glfwCreateWindow(size.x, size.y, title, nullptr, nullptr);
	}

	// Auto Positioning
	if (position == Vec2i::min && state == WindowState::WINDOWED) {
		setPosition(Vec2i(dispMode->width / 2 - size.x / 2, dispMode->height / 2 - size.y / 2));
	}

	// Set pointer and callbacks
	glfwSetWindowUserPointer(windowHandle, this);
	glfwSetFramebufferSizeCallback(windowHandle, framebufferResizeCallback);
	glfwSetWindowSizeCallback(windowHandle, resizeCallback);
	glfwSetWindowPosCallback(windowHandle, positionCallback);
	glfwSetKeyCallback(windowHandle, keyboardCallback);


	// Update the position and size to match reality
	glfwGetWindowPos(windowHandle, &position.x, &position.y);
	glfwGetWindowSize(windowHandle, &size.x, &size.y);
	Debug::logInfo(Debug::DISPLAY, "Created window \"%s\": pos(%d,%d), size(%d,%d)", title, position.x, position.y, size.x, size.y);
}

Window::~Window() {
	glfwDestroyWindow(windowHandle);
}


void Window::setTitle(const char* title) {
	glfwSetWindowTitle(windowHandle, title);
}

Vec2i Window::getPosition() const {
	Vec2i pos;
	glfwGetWindowPos(windowHandle, &pos.x, &pos.y);
	return pos;
}

void Window::setPosition(const Vec2i& pos) {
	if (!isFullscreen()) {
		glfwSetWindowPos(windowHandle, pos.x, pos.y);
	}
}

Vec2i Window::getSize() const {
	Vec2i size;
	glfwGetWindowSize(windowHandle, &size.x, &size.y);
	return size;
}

void Window::setSize(const Vec2i& size) {
	glfwSetWindowSize(windowHandle, size.x, size.y);
}

Vec2i Window::getFramebufferSize() const {
	Vec2i fSize;
	glfwGetFramebufferSize(windowHandle, &fSize.x, &fSize.y);
	return fSize;
}

bool Window::isFullscreen() const {
	return state == WindowState::FULLSCREEN_EXCLUSIVE || state == WindowState::FULLSCREEN_WINDOW;
}

WindowState Window::getState() const {
	return state;
}

void Window::setState(WindowState nState, Vec2i size, Vec2i pos) {
	Debug::logInfo(Debug::DISPLAY, "Changing window state ...");

	// Monitor queries
	GLFWmonitor* monitor = glfwGetPrimaryMonitor(); // TODO assumes main monitor only
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	
	// When setting to fullscreen window the new size needs to be the size of the screen
	// (otherwise glfw considers it as fullscreen exclusive)
	if (nState == WindowState::FULLSCREEN_WINDOW) {
		size.x = mode->width;
		size.y = mode->height;
	}

	// Auto sizing when no size is specified;
	if (size.x <= 0 || size.y <= 0) {
		switch (nState) {
		case WindowState::FULLSCREEN_WINDOW:
		case WindowState::FULLSCREEN_EXCLUSIVE:
			size.x = mode->width;
			size.y = mode->height;
			break;
		case WindowState::WINDOWED:
			size.x = mode->width / 2;
			size.y = mode->height / 2;
			break;
		case WindowState::WINDOWED_MAXIMIZED:
		default:
			break;
		}
	}

	// Auto positioning when no pos is specified
	// (windowed mode only, all other modes will implicitly move the window)
	if (pos == Vec2i::min && nState == WindowState::WINDOWED) {
		pos.x = mode->width / 2 - size.x / 2;
		pos.y = mode->height / 2 - size.y / 2;
	}

	// State switching
	switch (nState) {
	case WindowState::FULLSCREEN_WINDOW:
	case WindowState::FULLSCREEN_EXCLUSIVE:
		glfwSetWindowMonitor(windowHandle, monitor, 0, 0, size.x, size.y, mode->refreshRate);
		break;
	case WindowState::WINDOWED:
		glfwSetWindowMonitor(windowHandle, nullptr, pos.x, pos.y, size.x, size.y, mode->refreshRate);
		break;
	case WindowState::WINDOWED_MAXIMIZED:
		if (state == WindowState::FULLSCREEN_EXCLUSIVE || state == WindowState::FULLSCREEN_WINDOW) {
			glfwSetWindowMonitor(windowHandle, nullptr, 0, 0, 1, 1, GLFW_DONT_CARE);
		}
		glfwMaximizeWindow(windowHandle);
		break;
	}

	state = nState;
}


void Window::framebufferResizeCallback(GLFWwindow* windowHandle, int width, int height) {
	Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
	// TODO handle resize
}

void Window::resizeCallback(GLFWwindow* windowHandle, int width, int height) {
	Debug::logInfo("SIZE CALLBACK");
	Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
	// TODO handle resize
}

void Window::positionCallback(GLFWwindow* windowHandle, int posx, int posy) {
	Debug::logInfo("POS CALLBACK");
	Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
	// TODO handle move
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(windowHandle) != 0;
}

bool Window::nextTMP(u32 idx) {
	return kPressed[idx];
}
