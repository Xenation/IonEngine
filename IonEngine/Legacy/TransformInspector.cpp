#include "TransformInspector.h"

#include <imgui.h>
using namespace IonEngine;
using namespace IonEngine::Editor;



void TransformInspector::drawGui(Component* component) {
	Transform* transform = (Transform*) component;
	Vec3f pos = transform->getPosition();
	if (ImGui::InputFloat3("position", pos.data)) {
		transform->setPosition(pos);
	}
	Rotor3f rotor = transform->getRotation();
	Vec3f euler = Matrix4x4f::rotation(rotor).eulerRotation() * rad2deg;
	if (ImGui::InputFloat3("rotation", euler.data)) {
		rotor = Rotor3f::euler(euler * deg2rad);
		transform->setRotation(rotor);
	}
	Vec3f scale = transform->getScale();
	if (ImGui::InputFloat3("scale", scale.data)) {
		transform->setScale(scale);
	}
}
