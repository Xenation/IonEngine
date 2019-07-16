#pragma once
#include "XMath.h"
#include "Component.h"
class NoclipController : public Component {
public:
	float speed = 4.f;
	float lookSensivity = 2.f;

	NoclipController(Entity* entity);
	~NoclipController();

	virtual void onUpdate() override;

private:
	Vec3f eulerRot = Vec3f::zero;
};

