#pragma once
#include "EditorWindow.h"

namespace IonEngine::Editor {
	class MaterialEditor : public EditorWindow {
	public:
		MaterialEditor();
		~MaterialEditor();

		void drawGui() override;
	};
}
