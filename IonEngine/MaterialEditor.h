#pragma once
#include "EditorWindow.h"

class MaterialEditor : public EditorWindow {
public:
	MaterialEditor();
	~MaterialEditor();

	void drawGui() override;
};

