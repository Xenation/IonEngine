#pragma once
#include "EditorWindow.h"

class StatsEditor : public EditorWindow {
public:
	StatsEditor();
	~StatsEditor();

	void drawGui() override;

private:
	float frameTimes[300];
	float maxFrameTime = 0;

	void initFrameTimes();
	void addFrameTime(float time);
};

