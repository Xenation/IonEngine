#pragma once
#include "XMath.h"
#define KEY_STATES_SIZE 512
#define MOUSE_STATES_SIZE 8
#undef DELETE

enum KeyCode : int {
	UNKNOWN = -1,
	SPACE = 32,
	APOSTROPHE = 39,
	COMMA = 44,
	MINUS = 45,
	PERIOD = 46,
	SLASH = 47,
	NUM0 = 48,
	NUM1 = 49,
	NUM2 = 50,
	NUM3 = 51,
	NUM4 = 52,
	NUM5 = 53,
	NUM6 = 54,
	NUM7 = 55,
	NUM8 = 56,
	NUM9 = 57,
	SEMICOLON = 59,
	EQUAL = 61,
	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,
	LEFT_BRACKET = 91,
	BACKSLASH = 92,
	RIGHT_BRACKET = 93,
	GRAVE_ACCENT = 96,
	WORLD_1 = 161,
	WORLD_2 = 162,
	ESCAPE = 256,
	ENTER = 257,
	TAB = 258,
	BACKSPACE = 259,
	INSERT = 260,
	DELETE = 261,
	RIGHT = 262,
	LEFT = 263,
	DOWN = 264,
	UP = 265,
	PAGE_UP = 266,
	PAGE_DOWN = 267,
	HOME = 268,
	END = 269,
	CAPS_LOCK = 280,
	SCROLL_LOCK = 281,
	NUM_LOCK = 282,
	PRINT_SCREEN = 283,
	PAUSE = 284,
	F1 = 290,
	F2 = 291,
	F3 = 292,
	F4 = 293,
	F5 = 294,
	F6 = 295,
	F7 = 296,
	F8 = 297,
	F9 = 298,
	F10 = 299,
	F11 = 300,
	F12 = 301,
	F13 = 302,
	F14 = 303,
	F15 = 304,
	F16 = 305,
	F17 = 306,
	F18 = 307,
	F19 = 308,
	F20 = 309,
	F21 = 310,
	F22 = 311,
	F23 = 312,
	F24 = 313,
	F25 = 314,
	KP_0 = 320,
	KP_1 = 321,
	KP_2 = 322,
	KP_3 = 323,
	KP_4 = 324,
	KP_5 = 325,
	KP_6 = 326,
	KP_7 = 327,
	KP_8 = 328,
	KP_9 = 329,
	KP_DECIMAL = 330,
	KP_DIVIDE = 331,
	KP_MULTIPLY = 332,
	KP_SUBSTRACT = 333,
	KP_ADD = 334,
	KP_ENTER = 335,
	KP_EQUAL = 336,
	LEFT_SHIFT = 340,
	LEFT_CONTROL = 341,
	LEFT_ALT = 342,
	LEFT_SUPER = 343,
	RIGHT_SHIFT = 344,
	RIGHT_CONTROL = 345,
	RIGHT_ALT = 346,
	RIGHT_SUPER = 347,
	MENU = 348,
	LAST = MENU
};

enum class MouseButton : int {
	BTN0 = 0,
	BTN1 = 1,
	BTN2 = 2,
	BTN4 = 3,
	BTN5 = 4,
	BTN6 = 5,
	BTN7 = 6,
	BTN8 = 7,
	LEFT = BTN0,
	RIGHT = BTN1,
	MIDDLE = BTN2
};

struct GLFWwindow;
typedef void (*GLFWkeyFunction)(GLFWwindow* window, int key, int scancode, int action, int mods);
typedef void (*GLFWmouseButtonFunction)(GLFWwindow* window, int button, int action, int mods);
typedef void (*GLFWmousePositionFunction)(GLFWwindow* window, double xpos, double ypos);
class Input {
public:
	static bool keyDownStates[KEY_STATES_SIZE];
	static bool keyPressedStates[KEY_STATES_SIZE];
	static bool keyUpStates[KEY_STATES_SIZE];
	static bool mouseDownStates[MOUSE_STATES_SIZE];
	static bool mousePressedStates[MOUSE_STATES_SIZE];
	static bool mouseUpStates[MOUSE_STATES_SIZE];

	static bool mouseLocked;

	static Vec2f mousePosition;
	static Vec2f mouseDelta;

	static void PollEvents();
	static void LockMouse();
	static void UnlockMouse();

	static bool getKeyDown(KeyCode keyCode);
	static bool getKeyPressed(KeyCode keyCode);
	static bool getKeyUp(KeyCode keyCode);
	static bool getMouseDown(MouseButton mouseBtn);
	static bool getMouseDown(int mouseBtn);
	static bool getMousePressed(MouseButton mouseBtn);
	static bool getMousePressed(int mouseBtn);
	static bool getMouseUp(MouseButton mouseBtn);
	static bool getMouseUp(int mouseBtn);

	static const char* getKeyCodeString(KeyCode keyCode);

	static GLFWkeyFunction getKeyCallback();
	static GLFWmouseButtonFunction getMouseButtonCallback();
	static GLFWmousePositionFunction getMousePosCallback();

	Input() = delete;
	Input(const Input&) = delete;
	~Input() = delete;

private:
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);
};

