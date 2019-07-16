#include "NoclipController.h"

#include "Entity.h"
#include "Time.h"
#include "Input.h"
#include "Transform.h"



NoclipController::NoclipController(Entity* entity) : Component(entity) {

}

NoclipController::~NoclipController() {

}

void NoclipController::onUpdate() {
	if (transform == nullptr) return;
	float dt = Time::deltaTime;
	float currentSpeed = speed;
	if (Input::getKeyPressed(KeyCode::LEFT_SHIFT)) {
		currentSpeed *= 4;
	}
	if (Input::getKeyPressed(KeyCode::W)) {
		transform->translate(transform->forward() * currentSpeed * dt);
	}
	if (Input::getKeyPressed(KeyCode::S)) {
		transform->translate(transform->backward() * currentSpeed * dt);
	}
	if (Input::getKeyPressed(KeyCode::D)) {
		transform->translate(transform->right() * currentSpeed * dt);
	}
	if (Input::getKeyPressed(KeyCode::A)) {
		transform->translate(transform->left() * currentSpeed * dt);
	}
	if (Input::getKeyPressed(KeyCode::Q)) {
		transform->translate(transform->down() * currentSpeed * dt);
	}
	if (Input::getKeyPressed(KeyCode::E)) {
		transform->translate(transform->up() * currentSpeed * dt);
	}
	if (Input::getMouseDown(MouseButton::RIGHT)) {
		Input::LockMouse();
	}
	if (Input::getMousePressed(MouseButton::RIGHT)) {
		eulerRot.y += Input::mouseDelta.x * lookSensivity * dt;
		eulerRot.x += Input::mouseDelta.y * lookSensivity * dt;
		Input::mouseDelta = Vec2f::zero; // TODO remove when input system better
		entity->transform->setRotation(Quaternion::euler(eulerRot));
	}
	if (Input::getMouseUp(MouseButton::RIGHT)) {
		Input::UnlockMouse();
	}
}
