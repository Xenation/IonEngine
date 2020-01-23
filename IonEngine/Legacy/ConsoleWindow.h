#pragma once
#include "EditorWindow.h"

namespace IonEngine::Editor {
	class ConsoleWindow : public EditorWindow {
	public:
		ConsoleWindow();
		~ConsoleWindow();

		void drawGui() override;

	private:
		std::stringstream* consoleOut = nullptr;
	};
}
