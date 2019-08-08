#pragma once
#include "HollowSet.h"

namespace IonEngine {
	class Window;
}

namespace IonEngine::Editor {
	class EditorWindow;
	class SceneEditor;
	class MaterialEditor;
	class StatsEditor;
	class ConsoleWindow;

	class Gui {
	public:
		Gui(Window* window);
		~Gui();

		unsigned int registerEditorWindow(EditorWindow* editor);
		void unregisterEditorWindow(unsigned int id);

		void initialize();
		void update();
		virtual void onUpdate();
		void render();

	private:
		Window* window;

		HollowSet<EditorWindow*> editorWindows;
		SceneEditor* sceneEditor;
		MaterialEditor* materialEditor;
		StatsEditor* statsEditor;
		ConsoleWindow* consoleWindow;
		bool debugDisplayed;
		bool physicsDisplayed;
	};
}
