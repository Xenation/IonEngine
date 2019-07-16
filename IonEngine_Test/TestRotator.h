#pragma once
#include <Component.h>

class TestRotator : public Component {
public:
	TestRotator(Entity* entity);
	~TestRotator();

	virtual void onUpdate() override;
};

