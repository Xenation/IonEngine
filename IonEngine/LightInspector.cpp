#include "LightInspector.h"

#include <imgui.h>
#include "VisualDebug.h"
using namespace IonEngine;
using namespace IonEngine::Editor;



void LightInspector::drawGui(Component* component) {
	Light* light = (Light*) component;
	ImGui::ColorEdit3("Color", light->color.vec.data);
	ImGui::SliderFloat("Intensity", &light->intensity, 0.0f, 5.0f);
	const char* types[3] = {"Directional", "Point", "Spot"};
	int type = light->getType();
	if (ImGui::Combo("Type", &type, types, 3)) {
		light->setType((Light::Type) type);
	}
	ImGui::Indent();
	switch (light->getType()) {
	case Light::Directional:
		VisualDebug::drawLine(light->getPosition(), light->getPosition() + light->getDirection() * 2.0f, light->color);
		VisualDebug::drawWireDisk(light->getPosition(), light->getDirection(), 0.25f, light->color);
		VisualDebug::drawWireDisk(light->getPosition() + light->getDirection() * 0.3f, light->getDirection(), 0.25f, light->color);
		VisualDebug::drawWireDisk(light->getPosition() + light->getDirection() * 0.6f, light->getDirection(), 0.25f, light->color);
		VisualDebug::drawWireDisk(light->getPosition() + light->getDirection() * 1.0f, light->getDirection(), 0.25f, light->color);
		break;
	case Light::Point:
		ImGui::DragFloat("Range", &light->range, 0.1f);
		VisualDebug::drawWireSphere(light->getPosition(), light->range, light->color);
		break;
	case Light::Spot:
		ImGui::DragFloat("Range", &light->range, 0.1f);
		float degAngle = light->angle * rad2deg;
		if (ImGui::DragFloat("Angle", &degAngle, 0.1f)) {
			light->angle = degAngle * deg2rad;
		}
		float degInnerAngle = light->innerAngle * rad2deg;
		if (ImGui::DragFloat("InnerAngle", &degInnerAngle, 0.1f)) {
			light->innerAngle = degInnerAngle * deg2rad;
		}
		float endRadius = tanf(light->angle * 0.5) * light->range;
		VisualDebug::drawWireCone(light->getPosition(), light->getPosition() + light->getDirection() * light->range, endRadius, light->color);
		break;
	}
	ImGui::Unindent();
}