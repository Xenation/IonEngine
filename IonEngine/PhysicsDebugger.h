#pragma once
#include <bullet/btBulletCollisionCommon.h>
#include "Debug.h"
#include "VisualDebug.h"

class PhysicsDebugger : public btIDebugDraw {
public:
	PhysicsDebugger();
	~PhysicsDebugger();

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override { VisualDebug::drawLine({ from.x(), from.y(), from.z() }, { to.x(), to.y(), to.z() }, { color.x(), color.y(), color.z(), 1.0f }); }
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor) override { VisualDebug::drawLine({ from.x(), from.y(), from.z() }, { to.x(), to.y(), to.z() }, { fromColor.x(), fromColor.y(), fromColor.z(), 1.0f }, { toColor.x(), toColor.y(), toColor.z(), 1.0f }); }
	virtual void drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override {}
	virtual void draw3dText(const btVector3& location, const char* textString) override {}

	virtual void reportErrorWarning(const char* warningString) override { Debug::log("PHYS", warningString); }
	virtual void setDebugMode(int debugMode) override { this->debugMode = debugMode; }
	virtual int getDebugMode() const override { return debugMode; }

private:
	int debugMode = DBG_DrawWireframe;
};

