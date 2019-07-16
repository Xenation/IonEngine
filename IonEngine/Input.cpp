#include "Input.h"

#include <string>
#include "ShaderProgram.h"
#include <GLFW\glfw3.h>
#include "Engine.h"
#include "Window.h"
#include "Debug.h"
#undef DELETE



bool Input::keyDownStates[KEY_STATES_SIZE];
bool Input::keyPressedStates[KEY_STATES_SIZE];
bool Input::keyUpStates[KEY_STATES_SIZE];
bool Input::mouseDownStates[MOUSE_STATES_SIZE];
bool Input::mousePressedStates[MOUSE_STATES_SIZE];
bool Input::mouseUpStates[MOUSE_STATES_SIZE];

bool Input::mouseLocked = false;

Vec2f Input::mousePosition = Vec2f::zero;
Vec2f Input::mouseDelta = {INFINITY, INFINITY};


void Input::PollEvents() {
	for (unsigned int i = 0; i < KEY_STATES_SIZE; i++) {
		keyDownStates[i] = false;
		keyUpStates[i] = false;
	}
	for (unsigned int i = 0; i < MOUSE_STATES_SIZE; i++) {
		mouseDownStates[i] = false;
		mouseUpStates[i] = false;
	}
	glfwPollEvents();
}

void Input::LockMouse() {
	glfwSetInputMode(Engine::window->glfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Input::UnlockMouse() {
	glfwSetInputMode(Engine::window->glfwWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

bool Input::getKeyDown(KeyCode keyCode) {
	return keyDownStates[keyCode];
}

bool Input::getKeyPressed(KeyCode keyCode) {
	return keyPressedStates[keyCode];
}

bool Input::getKeyUp(KeyCode keyCode) {
	return keyUpStates[keyCode];
}

bool Input::getMouseDown(MouseButton btn) {
	return getMouseDown((int) btn);
}
bool Input::getMouseDown(int btnCode) {
	return mouseDownStates[btnCode];
}

bool Input::getMousePressed(MouseButton btn) {
	return getMousePressed((int) btn);
}
bool Input::getMousePressed(int btnCode) {
	return mousePressedStates[btnCode];
}

bool Input::getMouseUp(MouseButton btn) {
	return getMouseUp((int) btn);
}
bool Input::getMouseUp(int btnCode) {
	return mouseUpStates[btnCode];
}

const char* Input::getKeyCodeString(KeyCode keyCode) {
	switch (keyCode) {
	case UNKNOWN: return "UNKNOWN";
	case SPACE: return "SPACE";
	case APOSTROPHE: return "APOSTROPHE";
	case COMMA: return "COMMA";
	case MINUS: return "MINUS";
	case PERIOD: return "PERIOD";
	case SLASH: return "SLASH";
	case NUM0: return "NUM0";
	case NUM1: return "NUM1";
	case NUM2: return "NUM2";
	case NUM3: return "NUM3";
	case NUM4: return "NUM4";
	case NUM5: return "NUM5";
	case NUM6: return "NUM6";
	case NUM7: return "NUM7";
	case NUM8: return "NUM8";
	case NUM9: return "NUM9";
	case SEMICOLON: return "SEMICOLON";
	case EQUAL: return "EQUAL";
	case A: return "A";
	case B: return "B";
	case C: return "C";
	case D: return "D";
	case E: return "E";
	case F: return "F";
	case G: return "G";
	case H: return "H";
	case I: return "I";
	case J: return "J";
	case K: return "K";
	case L: return "L";
	case M: return "M";
	case N: return "N";
	case O: return "O";
	case P: return "P";
	case Q: return "Q";
	case R: return "R";
	case S: return "S";
	case T: return "T";
	case U: return "U";
	case V: return "V";
	case W: return "W";
	case X: return "X";
	case Y: return "Y";
	case Z: return "Z";
	case LEFT_BRACKET: return "LEFT_BRACKET";
	case BACKSLASH: return "BACKSLASH";
	case RIGHT_BRACKET: return "RIGHT_BRACKET";
	case GRAVE_ACCENT: return "GRAVE_ACCENT";
	case WORLD_1: return "WORLD_1";
	case WORLD_2: return "WORLD_2";
	case ESCAPE: return "ESCAPE";
	case ENTER: return "ENTER";
	case TAB: return "TAB";
	case BACKSPACE: return "BACKSPACE";
	case INSERT: return "INSERT";
	case DELETE: return "DELETE";
	case RIGHT: return "RIGHT";
	case LEFT: return "LEFT";
	case DOWN: return "DOWN";
	case UP: return "UP";
	case PAGE_UP: return "PAGE_UP";
	case PAGE_DOWN: return "PAGE_DOWN";
	case HOME: return "HOME";
	case END: return "END";
	case CAPS_LOCK: return "CAPS_LOCK";
	case SCROLL_LOCK: return "SCROLL_LOCK";
	case NUM_LOCK: return "NUM_LOCK";
	case PRINT_SCREEN: return "PRINT_SCREEN";
	case PAUSE: return "PAUSE";
	case F1: return "F1";
	case F2: return "F2";
	case F3: return "F3";
	case F4: return "F4";
	case F5: return "F5";
	case F6: return "F6";
	case F7: return "F7";
	case F8: return "F8";
	case F9: return "F9";
	case F10: return "F10";
	case F11: return "F11";
	case F12: return "F12";
	case F13: return "F13";
	case F14: return "F14";
	case F15: return "F15";
	case F16: return "F16";
	case F17: return "F17";
	case F18: return "F18";
	case F19: return "F19";
	case F20: return "F20";
	case F21: return "F21";
	case F22: return "F22";
	case F23: return "F23";
	case F24: return "F24";
	case F25: return "F25";
	case KP_0: return "KP_0";
	case KP_1: return "KP_1";
	case KP_2: return "KP_2";
	case KP_3: return "KP_3";
	case KP_4: return "KP_4";
	case KP_5: return "KP_5";
	case KP_6: return "KP_6";
	case KP_7: return "KP_7";
	case KP_8: return "KP_8";
	case KP_9: return "KP_9";
	case KP_DECIMAL: return "KP_DECIMAL";
	case KP_DIVIDE: return "KP_DIVIDE";
	case KP_MULTIPLY: return "KP_MULTIPLY";
	case KP_SUBSTRACT: return "KP_SUBSTRACT";
	case KP_ADD: return "KP_ADD";
	case KP_ENTER: return "KP_ENTER";
	case KP_EQUAL: return "KP_EQUAL";
	case LEFT_SHIFT: return "LEFT_SHIFT";
	case LEFT_CONTROL: return "LEFT_CONTROL";
	case LEFT_ALT: return "LEFT_ALT";
	case LEFT_SUPER: return "LAFT_SUPER";
	case RIGHT_SHIFT: return "RIGHT_SHIFT";
	case RIGHT_CONTROL: return "RIGHT_CONTROL";
	case RIGHT_ALT: return "RIGHT_ALT";
	case RIGHT_SUPER: return "RIGHT_SUPER";
	case MENU: return "MENU";
	}
}

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		Engine::window->closeWindow();
	}
	if (key == GLFW_KEY_F5 && action == GLFW_PRESS) {
		ShaderProgram::reloadAll();
	}

	if (action == GLFW_PRESS) {
		keyPressedStates[key] = true;
		keyDownStates[key] = true;
		//Debug::log("KeyDown", getKeyCodeString((KeyCode) key));
	} else if (action == GLFW_RELEASE) {
		keyPressedStates[key] = false;
		keyUpStates[key] = true;
		//Debug::log("KeyUp", getKeyCodeString((KeyCode) key));
	}

	//Debug::log("Input", ("Key Event: key=" + std::to_string(key) + " action=" + std::to_string(action)).c_str());
}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	//Debug::log("Input", ("Mouse Button Event: button=" + std::to_string(button) + " action=" + std::to_string(action)).c_str());

	if (action == GLFW_PRESS) {
		mousePressedStates[button] = true;
		mouseDownStates[button] = true;
	} else if (action == GLFW_RELEASE) {
		mousePressedStates[button] = false;
		mouseUpStates[button] = true;
	}
}

void Input::mousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
	Vec2f nMousePosition = Vec2f((float) xpos, (float) ypos);
	if (mouseDelta == Vec2f(INFINITY, INFINITY)) { // kinda ugly ...
		mousePosition = nMousePosition;
	}
	mouseDelta = nMousePosition - mousePosition;
	mousePosition = nMousePosition;
	//Debug::log("Input", ("Mouse Position Event: x=" + std::to_string(xpos) + " y=" + std::to_string(ypos) + " delta=(" + std::to_string(mouseDelta.x) + ", " + std::to_string(mouseDelta.y) + ")").c_str());
}

GLFWkeyFunction Input::getKeyCallback() {
	return keyCallback;
}

GLFWmouseButtonFunction Input::getMouseButtonCallback() {
	return mouseButtonCallback;
}

GLFWmousePositionFunction Input::getMousePosCallback() {
	return mousePositionCallback;
}
