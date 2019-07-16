#include "PhysicsWorld.h"

#include "bullet/btBulletDynamicsCommon.h"
#include "Debug.h"
#include "Rigidbody.h"
#include "PhysicsDebugger.h"



PhysicsWorld::PhysicsWorld() {
	collisionConfiguration = new btDefaultCollisionConfiguration();

	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	overlappingPairCache = new btDbvtBroadphase();

	solver = new btSequentialImpulseConstraintSolver();

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
	dynamicsWorld->setDebugDrawer(new PhysicsDebugger());

	//InitializeDebugWorldContents();
}

PhysicsWorld::~PhysicsWorld() {

	//CleanDebugWorldContents();

	// Delete Physics Sim objects
	delete dynamicsWorld;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfiguration;

}


void PhysicsWorld::simulate(float dt) {
	dynamicsWorld->stepSimulation(dt, 10);
	if (debug) {
		dynamicsWorld->debugDrawWorld();
	}

	//DisplayDebugWorldContents();
}

void PhysicsWorld::registerRigidbody(Rigidbody* rigidbody) {
	dynamicsWorld->addRigidBody(rigidbody->getBulletBody());
}

void PhysicsWorld::unregisterRigidbody(Rigidbody* rigidbody) {
	dynamicsWorld->removeRigidBody(rigidbody->getBulletBody());
}

void PhysicsWorld::InitializeDebugWorldContents() {
	collisionShapes = new btAlignedObjectArray<btCollisionShape*>();

	btCollisionShape* groundShape = new btBoxShape(btVector3(50.0f, 50.0f, 50.0f));
	collisionShapes->push_back(groundShape);
	AddDebugRigidBody(groundShape, Vec3f(0, -56, 0), 0);

	btCollisionShape* colShape = new btSphereShape(1.0f);
	collisionShapes->push_back(colShape);
	AddDebugRigidBody(colShape, Vec3f(2, 10, 0), 1);
}

void PhysicsWorld::DisplayDebugWorldContents() {
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform transf;
		if (body && body->getMotionState()) {
			body->getMotionState()->getWorldTransform(transf);
		} else {
			transf = obj->getWorldTransform();
		}
		Debug::log("PHYS", std::string("world pos object " + std::to_string(i) + " = " + std::to_string(transf.getOrigin().getX()) + ", " + std::to_string(transf.getOrigin().getY()) + ", " + std::to_string(transf.getOrigin().getZ())));
	}
}

void PhysicsWorld::CleanDebugWorldContents() {
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	for (int i = 0; i < collisionShapes->size(); i++) {
		btCollisionShape* shape = (*collisionShapes)[i];
		(*collisionShapes)[i] = nullptr;
		delete shape;
	}

	collisionShapes->clear();
	delete collisionShapes;
}

void PhysicsWorld::AddDebugRigidBody(btCollisionShape* shape, Vec3f pos, float mass) {
	btTransform transform;
	transform.setIdentity();

	bool isDynamic = mass != 0.0f;

	btVector3 localInertia(0, 0, 0);
	if (isDynamic) {
		shape->calculateLocalInertia(mass, localInertia);
	}

	transform.setOrigin(btVector3(pos.x, pos.y, pos.z));

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	dynamicsWorld->addRigidBody(body);
}
