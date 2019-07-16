#include "Transform.h"

#include "Debug.h"
#include "Entity.h"

#define CHILD_ARRAY_START_SIZE 4
#define CHILD_ARRAY_INCREASE 4



Transform::Transform(Entity* entity)
	: Component(entity, this) {

}

Transform::~Transform() {
	
}


Matrix4x4f Transform::getLocalToWorldMatrix() {
	return localToWorldMatrix;
}

Matrix4x4f Transform::getWorldToLocalMatrix() {
	return worldToLocalMatrix;
}

Vec4f Transform::localToWorld(Vec4f vec) {
	return localToWorldMatrix * vec;
}

Vec3f Transform::localToWorldPos(Vec3f pos) {
	return localToWorldMatrix.multPoint(pos);
}

Vec3f Transform::localToWorldDir(Vec3f dir) {
	return localToWorldMatrix.multDirection(dir);
}

Vec4f Transform::worldToLocal(Vec4f vec) {
	return worldToLocalMatrix * vec;
}

Vec3f Transform::worldToLocalPos(Vec3f pos) {
	return worldToLocalMatrix.multPoint(pos);
}

Vec3f Transform::worldToLocalDir(Vec3f dir) {
	return worldToLocalMatrix.multDirection(dir);
}

void Transform::parentChanged() {
	updateMatricesFromLTP();
}

void Transform::updateMatricesFromLTP() {
	// Update LTW
	localToWorldMatrix = localToParentMatrix;
	if (entity->getParent() != nullptr) {
		Entity* currentParent = entity->getParent();
		while (currentParent != nullptr) {
			if (currentParent->transform != nullptr) {
				localToWorldMatrix = currentParent->transform->getLocalToWorldMatrix() * localToParentMatrix;
				break;
			}
			currentParent = currentParent->getParent();
		}
	}
	entity->ltwChangeNotifyChildren();

	// Update WTL
	worldToLocalMatrix = localToWorldMatrix.inverseAffine();
}

void Transform::updateMatricesFromLTW() {
	// Update LTP
	Matrix4x4f parentWTL = Matrix4x4f::identity;
	if (entity->getParent() != nullptr) {
		Entity* currentParent = entity->getParent();
		while (currentParent != nullptr) {
			if (currentParent->transform != nullptr) {
				parentWTL = currentParent->transform->getWorldToLocalMatrix();
				break;
			}
			currentParent = currentParent->getParent();
		}
	}
	localToParentMatrix = parentWTL * localToWorldMatrix;
	entity->ltwChangeNotifyChildren();

	// Update WTL
	worldToLocalMatrix = localToWorldMatrix.inverseAffine();
}
