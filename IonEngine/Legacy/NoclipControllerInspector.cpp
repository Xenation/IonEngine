#include "NoclipControllerInspector.h"

#include <imgui.h>
using namespace IonEngine;
using namespace IonEngine::Editor;



void NoclipControllerInspector::drawGui(Component* component) {
	NoclipController* controller = (NoclipController*) component;
	ImGui::InputFloat("Speed", &controller->speed);
	ImGui::InputFloat("Look Sensivity", &controller->lookSensivity);
}
