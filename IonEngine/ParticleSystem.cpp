#include "ParticleSystem.h"

#include "Time.h"
#include "Entity.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "SpecializedShaderProgram.h"
#include "Perlin.h"
#include "Random.h"
using namespace IonEngine;



ParticleSystem::ParticleSystem(Entity* entity) : Renderer(entity), particles(0, 0, true) {
	
}

ParticleSystem::~ParticleSystem() {
	
}

void ParticleSystem::startEmit() {
	if (isEmitting) return;
	isEmitting = true;
	uint nMax = ceilToInt(emitRate * maxLifetime);
	emissionInterval = 1.0f / emitRate;
	particles = SimpleSet<Particle>(nMax, 32);
	if (nMax != maxParticles) {
		resizeParticleData(nMax);
	}
}

void ParticleSystem::onUpdate() {
	// Kill expired Particles
	for (uint i = 0; i < particles.count; i++) {
		if (Time::time - particles[i].creationTime > particles[i].lifetime) {
			particles.removeAt(i);
			i--;
		}
	}

	// New Particle generation
	if (isEmitting && Time::time - lastEmissionTime > emissionInterval) {
		lastEmissionTime = Time::time;
		createParticle(entity->transform->getWorldPosition() + emitOffset + Vec3f(perlinf(Time::time * 100, 0, 0) * emitZoneExtents.x, perlinf(0, Time::time * 100, 0) * emitZoneExtents.y, perlinf(0, 0, Time::time * 100) * emitZoneExtents.z), entity->transform->localToWorldDir(emitVelocity), Random::i->range(minLifetime, maxLifetime));
	}

	// Particles state update
	for (uint i = 0; i < particles.count; i++) {
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
	if (!isEnabled() || mesh == nullptr) return;
	
	//material->specializedProgram->loadMatrix4x4f(modelMatrixLocation, Matrix4x4f::translation(entity->transform->getWorldPosition()));

	mesh->render();

	/*for (uint i = 0; i < maxParticles; i++) {
		if (!particles[i].alive) continue;
		Matrix4x4f particleMatrix = Matrix4x4f::translation(particles[i].position);

		material->specializedProgram->loadMatrix4x4f(modelMatrixLocation, particleMatrix);

		Mesh::quadCentered->render();
	}*/
}

void ParticleSystem::renderNoUniform() {
	if (!isEnabled() || mesh == nullptr) return;

	mesh->render();
}

void ParticleSystem::createParticle(const Vec3f& pos, const Vec3f& vel, float lifetime) {
	particles.add({pos, vel, Time::time, lifetime});
}

void ParticleSystem::resizeParticleData(unsigned int nCount) {
	if (mesh == nullptr) {
		mesh = new Mesh("Particles_" + std::string(entity->name), nCount, nCount);
		mesh->setTopology(GL_POINTS);
		mesh->setUsageHint(GL_DYNAMIC_DRAW);
		mesh->setAttributesDefinition(3, new int[3]{3, 3, 2});
		unsigned int* indices = new unsigned int[nCount];
		for (unsigned int i = 0; i < nCount; i++) {
			indices[i] = i;
		}
		mesh->setIndices(indices);
		mesh->uploadToGL();
	} else {
		mesh->resize(nCount, nCount);
	}
}

void ParticleSystem::updateMesh() {
	if (mesh == nullptr) return;
	for (unsigned int i = 0; i < particles.count; i++) {
		mesh->setAttributeElement(0, i, particles[i].position);
		mesh->setAttributeElement(1, i, particles[i].velocity);
		mesh->setAttributeElement(2, i, Vec2f(particles[i].creationTime, particles[i].lifetime));
	}
	mesh->setDrawnIndexCount(particles.count);
	mesh->updateInGL();
	mesh->recalculateBoundsFromIndices();
	worldBounds = mesh->getBounds();
}
