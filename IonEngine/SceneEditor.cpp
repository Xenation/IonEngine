#include "SceneEditor.h"

#include <imgui.h>
#include "Entity.h"
#include "EntityManager.h"
#include "EntityEditor.h"
using namespace IonEngine;
using namespace IonEngine::Editor;



SceneEditor::SceneEditor(EntityManager* manager) : EditorWindow("Scene", true), manager(manager) {}

SceneEditor::~SceneEditor() {}


void SceneEditor::drawGui() {
	ImGui::Text("Update Times");
	ImGui::Indent();
	ImGui::PlotLines("", manager->getUpdateTimesPtr(), 300, 0, '\0', 0.0f, manager->getMaxUpdateTime());
	ImGui::Text("Max Update Time: %.6fms", manager->getMaxUpdateTime() * 1000);
	ImGui::Unindent();
	unsigned int counted = 0;
	HollowSet<Entity*>* entities = manager->getEntitiesSet();
	for (unsigned int i = 0; i < entities->capacity && counted < entities->count; i++) {
		if ((*entities)[i] == nullptr || (*entities)[i]->getParent() != nullptr) continue;
		drawGui((*entities)[i]);
		counted++;
	}
}

void SceneEditor::drawGui(Entity* entity) {
	std::string editIded("Edit##" + std::to_string(entity->id));

	unsigned int childCount = entity->childCount();
	if (childCount != 0) {
		std::string entityName = entity->name + std::string("##") + std::to_string(entity->id);
		if (ImGui::TreeNode(entityName.c_str())) {
			ImGui::SameLine(ImGui::GetWindowWidth() - 40);
			if (ImGui::SmallButton(editIded.c_str())) {
				EntityEditor* editor = EntityEditor::getActiveEditor(entity);
				if (editor == nullptr) {
					editor = new EntityEditor(entity);
				} else {
					delete editor;
				}
			}
			for (unsigned int i = 0; i < childCount; i++) {
				drawGui(entity->getChild(i));
			}
			ImGui::TreePop();
		} else {
			ImGui::SameLine(ImGui::GetWindowWidth() - 40);
			if (ImGui::SmallButton(editIded.c_str())) {
				EntityEditor* editor = EntityEditor::getActiveEditor(entity);
				if (editor == nullptr) {
					editor = new EntityEditor(entity);
				} else {
					delete editor;
				}
			}
		}
	} else {
		ImGui::BulletText(entity->name.c_str());
		ImGui::SameLine(ImGui::GetWindowWidth() - 40);
		if (ImGui::SmallButton(editIded.c_str())) {
			EntityEditor* editor = EntityEditor::getActiveEditor(entity);
			if (editor == nullptr) {
				editor = new EntityEditor(entity);
			} else {
				delete editor;
			}
		}
	}
}
