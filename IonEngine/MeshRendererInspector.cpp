#include "MeshRendererInspector.h"

#include <imgui.h>
#include "Mesh.h"
#include "Material.h"
using namespace IonEngine;
using namespace IonEngine::Editor;



void MeshRendererInspector::drawGui(Component* component) {
	MeshRenderer* meshRenderer = (MeshRenderer*) component;
	ImGui::LabelText("mesh", "%s", meshRenderer->getMesh()->getName().c_str());
	ImGui::LabelText("material", "%s", meshRenderer->getMaterial()->name.c_str());
}
