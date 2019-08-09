#include "LightInspector.h"

#include <imgui.h>
using namespace IonEngine;
using namespace IonEngine::Editor;



void LightInspector::drawGui(Component* component) {
	Light* light = (Light*) component;
	ImGui::ColorEdit3("Color", light->color.vec.data);
	ImGui::SliderFloat("Intensity", &light->intensity, 0.0f, 5.0f);
	const char* types[3] = {"Directional", "Point", "Spot"};
	ImGui::Combo("Type", (int*) &light->type, types, 3);
	ImGui::Indent();
	switch (light->type) {
	case Light::Directional:
		// Nothing to display
		break;
	case Light::Point:
		ImGui::Text("Point");
		// TODO implement
		break;
	case Light::Spot:
		ImGui::Text("Spot");
		// TODO implement
		break;
	}
	ImGui::Unindent();
}
