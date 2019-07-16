#pragma once
#include "XMath.h"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btCollisionShape;
template<typename T>
class btAlignedObjectArray;
class Rigidbody;

class PhysicsWorld {
public:
	bool debug = false;

	PhysicsWorld();
	~PhysicsWorld();

	void simulate(float dt);

	void registerRigidbody(Rigidbody* rigidbody);
	void unregisterRigidbody(Rigidbody* rigidbody);

private:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	btAlignedObjectArray<btCollisionShape*>* collisionShapes;

	void InitializeDebugWorldContents();
	void DisplayDebugWorldContents();
	void CleanDebugWorldContents();
	void AddDebugRigidBody(btCollisionShape* colShape, Vec3f pos, float mass);
};

