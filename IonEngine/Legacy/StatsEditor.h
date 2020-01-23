#pragma once
#include "EditorWindow.h"

namespace IonEngine::Editor {
	class StatsEditor : public EditorWindow {
	public:
		StatsEditor();
		~StatsEditor();

		void drawGui() override;

	private:
		float frameTimes[300];
		float maxFrameTime = 0;
		bool viewBounds = false;

		void initFrameTimes();
		void addFrameTime(float time);
	};
}
