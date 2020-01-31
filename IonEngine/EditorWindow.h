#pragma once
#include <string>

namespace IonEngine::Editor {
	class EditorWindow {
	public:
		bool hidden = false;
		bool closingHides;
		std::string title;

		EditorWindow(std::string title, bool closingHides = false);
		virtual ~EditorWindow();

		void render();
		virtual void drawGui();

	private:
		unsigned int id = 0;
	};
}
