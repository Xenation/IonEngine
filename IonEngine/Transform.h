#pragma once
#include "Component.h"
#include "XMath.h"
#include "SimpleList.h"
class Transform : public Component {
public:
	Transform(Entity* entity);
	~Transform();

	inline Vec3f getPosition() { return localToParentMatrix.translation(); }
	inline Vec3f getWorldPosition() { return localToWorldMatrix.translation(); }
	inline Vec3f getScale() { return scale; }
	inline Vec3f getWorldScale() { return localToWorldMatrix.scale(); }
	inline Quaternion getRotation() { return rotation; }
	inline Quaternion getWorldRotation() { return localToWorldMatrix.rotation(); }

	inline void setPosition(Vec3f pos) { localToParentMatrix.translation() = pos; updateMatricesFromLTP(); }
	inline void setWorldPosition(Vec3f pos) { localToWorldMatrix.translation() = pos; updateMatricesFromLTW(); }
	inline void translate(Vec3f translation) { localToParentMatrix.translation() += translation; updateMatricesFromLTP(); }
	inline void setScale(Vec3f sca) { scale = sca; localToParentMatrix = Matrix4x4f::transformation(localToParentMatrix.translation(), scale, rotation); updateMatricesFromLTP(); }
	inline void setWorldScale(Vec3f sca) { localToWorldMatrix = Matrix4x4f::transformation(localToWorldMatrix.translation(), sca, localToWorldMatrix.rotation()); updateMatricesFromLTW(); }
	inline void setRotation(Quaternion rot) { rotation = rot; localToParentMatrix = Matrix4x4f::transformation(localToParentMatrix.translation(), scale, rotation); updateMatricesFromLTP(); }
	inline void rotate(Quaternion rot) { rotation *= rot; localToParentMatrix = Matrix4x4f::transformation(localToParentMatrix.translation(), scale, rotation); updateMatricesFromLTP(); }
	inline void setWorldRotation(Quaternion rot) { localToWorldMatrix = Matrix4x4f::transformation(localToWorldMatrix.translation(), localToWorldMatrix.scale(), rot); updateMatricesFromLTW(); }

	Matrix4x4f getLocalToWorldMatrix();
	Matrix4x4f getWorldToLocalMatrix();

	Vec4f localToWorld(Vec4f vec);
	Vec3f localToWorldPos(Vec3f pos);
	Vec3f localToWorldDir(Vec3f dir);
	Vec4f worldToLocal(Vec4f vec);
	Vec3f worldToLocalPos(Vec3f pos);
	Vec3f worldToLocalDir(Vec3f dir);

	inline Vec3f forward() { return localToWorldDir(Vec3f::forward).normalized(); }
	inline Vec3f backward() { return localToWorldDir(Vec3f::backward).normalized(); }
	inline Vec3f right() { return localToWorldDir(Vec3f::right).normalized(); }
	inline Vec3f left() { return localToWorldDir(Vec3f::left).normalized(); }
	inline Vec3f up() { return localToWorldDir(Vec3f::up).normalized(); }
	inline Vec3f down() { return localToWorldDir(Vec3f::down).normalized(); }

	void parentChanged();

private:
	Vec3f scale = Vec3f::one;
	Quaternion rotation = Quaternion::identity;
	Matrix4x4f localToParentMatrix = Matrix4x4f::identity;
	Matrix4x4f localToWorldMatrix = Matrix4x4f::identity;
	Matrix4x4f worldToLocalMatrix = Matrix4x4f::identity;

	void updateMatricesFromLTP();
	void updateMatricesFromLTW();
};

