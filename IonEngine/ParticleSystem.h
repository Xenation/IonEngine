#pragma once
#include "Renderer.h"
#include "XMath.h"
#include "SimpleSet.h"

namespace IonEngine {
	class Entity;
	class Mesh;

	class ParticleSystem : public Renderer {
	public:
		struct Particle {
			Vec3f position;
			Vec3f velocity;
			float creationTime;
			float lifetime;
		};

		SimpleSet<Particle> particles;

		float emitRate = 5;
		Vec3f emitVelocity = Vec3f::up;
		Vec3f emitOffset = Vec3f::zero;
		Vec3f emitZoneExtents = Vec3f::one;
		float minLifetime = 5;
		float maxLifetime = 8;

		ParticleSystem(Entity* entity);
		~ParticleSystem();

		void startEmit();
		virtual void onUpdate() override;
		void stopEmit();
		inline bool getEmitting() const { return isEmitting; }

		virtual void setMaterial(Material* mat) override;
		virtual void render() override;

	private:
		Mesh* mesh = nullptr;
		bool isEmitting = false;
		unsigned int maxParticles = 0;
		float emissionInterval = 0;
		float lastEmissionTime = 0;

		unsigned int modelMatrixLocation = 0;

		void createParticle(const Vec3f& pos, const Vec3f& vel, float lifetime);
		void resizeParticleData(unsigned int nCount);
		void updateMesh();
	};
}
