#pragma once
#include <Component.h>

class TestRotator : public IonEngine::Component {
public:
	TestRotator(IonEngine::Entity* entity);
	~TestRotator();

	virtual void onUpdate() override;
};

