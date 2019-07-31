#include "TransformInspector.h"

#include <imgui.h>



void TransformInspector::drawGui(Component* component) {
	Transform* transform = (Transform*) component;
	Vec3f pos = transform->getPosition();
	if (ImGui::InputFloat3("position", pos.data)) {
		transform->setPosition(pos);
	}
	Rotor3f rotor = transform->getRotation();
	if (ImGui::InputFloat4("rotor", rotor.data)) {
		rotor.normalize();
		transform->setRotation(rotor);
	}
	Vec3f scale = transform->getScale();
	if (ImGui::InputFloat3("scale", scale.data)) {
		transform->setScale(scale);
	}
}
