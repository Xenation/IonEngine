#include "EditorWindow.h"

#include <imgui.h>
#include "Engine.h"
#include "Gui.h"
using namespace IonEngine::Editor;



EditorWindow::EditorWindow(std::string title, bool closingHides) : title(title), closingHides(closingHides) {
	id = Engine::gui->registerEditorWindow(this);
}

EditorWindow::~EditorWindow() {
	Engine::gui->unregisterEditorWindow(id);
}


void EditorWindow::render() {
	if (hidden) return;
	bool open = true;
	//if (ImGui::Begin((title + "##" + std::to_string(id)).c_str(), &open) && open) { // Used to prevent id conflicts, but also breaks position persistence
	if (ImGui::Begin(title.c_str(), &open) && open) {
		drawGui();
	}
	ImGui::End();
	if (!open) {
		if (closingHides) {
			hidden = true;
		} else {
			delete this;
		}
	}
}

void EditorWindow::drawGui() {

}
