#pragma once
#include "XMath.h"
#include "SimpleList.h"

namespace IonEngine {
	class Mesh;
	class ShaderProgram;
	class Material;

	class VisualDebug {
	public:
		VisualDebug() = delete;
		VisualDebug(const VisualDebug&) = delete;
		~VisualDebug() = delete;

		// Wire
		static void drawLine(const Vec3f& from, const Vec3f& to, const Color& color);
		static void drawLine(const Vec3f& from, const Vec3f& to, const Color& fromColor, const Color& toColor);
		static void drawRay(const Ray3f& ray, const Color& color);
		static void drawRay(const Ray3f& ray, float length, const Color& color);
		static void drawFrustum(const Matrix4x4f& viewMatrix, const Matrix4x4f& projMatrix, float near, float far, const Color& color);
		static void drawWireCube(const Vec3f& center, const Vec3f& size, const Color& color);
		static void drawWireMesh(const Mesh* mesh, const Vec3f& position, const Rotor3f& rotation, const Vec3f& scale, const Color& color);
		static void drawWireMesh(const Mesh* mesh, const Matrix4x4f& ltwMatrix, const Color& color);
		static void drawWireSphere(const Vec3f& center, float radius, const Color& color);
		static void drawWireDisk(const Vec3f& center, const Vec3f& normal, float radius, const Color& color);
		static void drawWirePlaneDisk(const Plane& plane, float normalLength, float diskRadius, const Color& color);
		static void drawWirePlaneDisk(const Plane& plane, const Vec3f& unprojetedCenter, float normalLength, float diskRadius, const Color& color);
		// Solid
		static void drawCube(const Vec3f& center, const Vec3f& size, const Color& color);
		static void drawMesh(const Mesh* mesh, const Vec3f& position, const Rotor3f& rotation, const Vec3f& scale, const Color& color);
		static void drawMesh(const Mesh* mesh, const Matrix4x4f& ltwMatrix, const Color& color);
		static void drawSphere(const Vec3f& center, const float radius, const Color& color);

		static void render();

	private:

		struct MeshCall {
			const Mesh* mesh;
			Matrix4x4f transf;
			Color color;
		};

		static bool initialized;
		static ShaderProgram* debugShader;
		static Material* debugMaterial;
		static ShaderProgram* debugMeshShader;
		static Material* debugMeshMaterial;
		static Mesh* wireMesh;
		static Mesh* solidMesh;
		static Mesh* sphereMesh;
		static unsigned int wireVerticesIndex;
		static unsigned int wireIndicesIndex;
		static unsigned int solidVerticesIndex;
		static unsigned int solidIndicesIndex;
		static SimpleList<MeshCall> meshCalls;
		static unsigned int ltwMatrixLocation;

		static void initialize();
		static void initializeMaterials();
		static void initializeMeshes();

		static void addWireVertex(Vec3f pos, Color col);
		static void addWireIndex(int index);
		static void addSolidVertex(const Vec3f& pos, const Color& col);
		static void addSolidIndex(int index);
	};
}
