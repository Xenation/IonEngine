#pragma once
class Window;
class Gui {
public:
	Gui(Window* window);
	~Gui();

	void update();
	virtual void onUpdate();
	void render();

private:
	Window* window;

	bool debugDisplayed;
	bool statsDisplayed;
	bool sceneDisplayed;
	bool shadersDisplayed;
	bool physicsDisplayed;

	float frameTimes[300];
	float maxFrameTime = 0;

	void initFrameTimes();
	void addFrameTime(float time);
};

