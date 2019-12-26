#include "EntityEditor.h"

#include <typeindex>
#include <imgui.h>
#include "Entity.h"
#include "BaseComponentInspector.h"
#include "ComponentInspector.h"
using namespace IonEngine;
using namespace IonEngine::Editor;



HollowSet<EntityEditor*> EntityEditor::activeEditors(8, 8);


EntityEditor::EntityEditor(Entity* entity) : EditorWindow(entity->name), entity(entity) {
	id = activeEditors.add(this);
}

EntityEditor::~EntityEditor() {
	activeEditors.remove(id);
}


void EntityEditor::drawGui() {
	HollowSet<Component*>& components = entity->getRawComponentsSet();
	u32 counted = 0;
	for (u32 i = 0; i < components.capacity && counted < components.capacity; i++) {
		if (components[i] == nullptr) continue;
		bool enabled = components[i]->isEnabled();
		if (ImGui::Checkbox(std::string("##" + std::to_string(i)).c_str(), &enabled)) {
			(enabled) ? components[i]->enable() : components[i]->disable();
		}
		ImGui::SameLine();
		if (ImGui::TreeNode(typeid(*components[i]).name())) {
			BaseComponentInspector* inspector = BaseComponentInspector::getInspector(std::type_index(typeid(*components[i])));
			if (inspector != nullptr) {
				inspector->drawGui(components[i]);
			} else {
				ImGui::Text("No Valid Inspector");
			}
			ImGui::TreePop();
		}
		counted++;
	}
}

EntityEditor* EntityEditor::getActiveEditor(Entity* entity) {
	u32 counted = 0;
	for (u32 i = 0; i < activeEditors.capacity && counted < activeEditors.count; i++) {
		if (activeEditors[i] == nullptr) continue;
		if (activeEditors[i]->entity == entity) {
			return activeEditors[i];
		}
		counted++;
	}
	return nullptr;
}
