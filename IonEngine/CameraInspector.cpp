#include "CameraInspector.h"

#include <imgui.h>



void CameraInspector::drawGui(Component* component) {
	Camera* camera = (Camera*) component;
	float fov = camera->getFov();
	float nearPlane = camera->getNearPlane();
	float farPlane = camera->getFarPlane();

	if (ImGui::InputFloat("FOV", &fov)) {
		camera->setFov(fov);
	}
	if (ImGui::InputFloat("Near Plane", &nearPlane)) {
		camera->setNearPlane(nearPlane);
	}
	if (ImGui::InputFloat("Far Plane", &farPlane)) {
		camera->setFarPlane(farPlane);
	}
}
