#include "ParticleSystem.h"

#include "Time.h"
#include "Entity.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "SpecializedShaderProgram.h"
#include "Perlin.h"



ParticleSystem::ParticleSystem(Entity* entity) : Renderer(entity) {
	
}

ParticleSystem::~ParticleSystem() {
	if (particles != nullptr) {
		delete[] particles;
	}
}

void ParticleSystem::startEmit() {
	if (isEmitting) return;
	isEmitting = true;
	uint nMax = ceilToInt(emitRate * maxLifetime);
	emissionInterval = 1.0f / emitRate;
	if (nMax != maxParticles) {
		resizeParticleData(nMax);
	}
}

void ParticleSystem::onUpdate() {
	// Kill expired Particles
	for (uint i = 0; i < maxParticles; i++) {
		if (!particles[i].alive) continue;
		if (Time::time - particles[i].creationTime > particles[i].lifetime) {
			particles[i].alive = false;
		}
	}

	// New Particle generation
	if (isEmitting && Time::time - lastEmissionTime > emissionInterval) {
		lastEmissionTime = Time::time;
		createParticle(entity->transform->getWorldPosition() + emitOffset + Vec3f(perlinf(Time::time * 100, 0, 0) * emitZoneExtents.x, perlinf(0, Time::time * 100, 0) * emitZoneExtents.y, perlinf(0, 0, Time::time * 100) * emitZoneExtents.z), entity->transform->localToWorldDir(emitVelocity), randomRangef(minLifetime, maxLifetime));
	}

	// Particles state update
	for (uint i = 0; i < maxParticles; i++) {
		if (!particles[i].alive) continue; // Skip dead
		// Apply gravity
		particles[i].velocity += Vec3f::down * 9.81f * Time::deltaTime;
		// Change position
		particles[i].position += particles[i].velocity * Time::deltaTime;
	}

	// Particles Mesh update
	updateMesh();
}

void ParticleSystem::stopEmit() {
	isEmitting = false;
}

void ParticleSystem::setMaterial(Material* mat) {
	Renderer::setMaterial(mat);
	modelMatrixLocation = mat->specializedProgram->getUniformLocation("modelMatrix");
}

void ParticleSystem::render() {
	
	//material->specializedProgram->loadMatrix4x4f(modelMatrixLocation, Matrix4x4f::translation(entity->transform->getWorldPosition()));

	if (mesh == nullptr) return;
	mesh->render();

	/*for (uint i = 0; i < maxParticles; i++) {
		if (!particles[i].alive) continue;
		Matrix4x4f particleMatrix = Matrix4x4f::translation(particles[i].position);

		material->specializedProgram->loadMatrix4x4f(modelMatrixLocation, particleMatrix);

		Mesh::quadCentered->render();
	}*/
}

void ParticleSystem::createParticle(const Vec3f& pos, const Vec3f& vel, float lifetime) {
	for (uint i = 0; i < maxParticles; i++) {
		if (particles[i].alive) continue;
		particles[i].position = pos;
		particles[i].velocity = vel;
		particles[i].creationTime = Time::time;
		particles[i].lifetime = lifetime;
		particles[i].alive = true;
		break;
	}
}

void ParticleSystem::resizeParticleData(uint nCount) {
	if (particles != nullptr) {
		delete[] particles;
		particles = nullptr;
	}
	if (mesh != nullptr) {
		delete[] mesh;
		mesh = nullptr;
	}

	maxParticles = nCount;
	particles = new Particle[maxParticles];
	for (uint i = 0; i < maxParticles; i++) {
		particles[i].alive = false;
	}
	
	mesh = new Mesh("PS_" + std::string(entity->name), nCount, nCount);
	mesh->setTopology(GL_POINTS);
	mesh->setUsageHint(GL_DYNAMIC_DRAW);
	mesh->setAttributesDefinition(3, new int[3]{3, 3, 2});
	uint* indices = new uint[nCount];
	for (uint i = 0; i < nCount; i++) {
		indices[i] = i;
	}
	mesh->setIndices(indices);
	mesh->uploadToGL();
}

void ParticleSystem::updateMesh() {
	if (mesh == nullptr) return;
	for (uint i = 0; i < maxParticles; i++) {
		if (!particles[i].alive) continue;
		mesh->setAttributeElement(0, i, particles[i].position);
		mesh->setAttributeElement(1, i, particles[i].velocity);
		mesh->setAttributeElement(2, i, Vec2f(particles[i].creationTime, particles[i].lifetime));
	}
	mesh->updateInGL();
}
