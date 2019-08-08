#include "VisualDebug.h"

#include "Mesh.h"
#include "ShaderProgram.h"
#include "SpecializedShaderProgram.h"
#include "Material.h"
using namespace IonEngine;

#undef far
#undef near
#define SHADER_CODE_DEBUG_VS "#version 420 core\n#pragma pass opaque\nlayout (location = 0) in vec3 vp;\nlayout (location = 1) in vec4 vc;\nlayout (std140, binding = 1) uniform CameraMatrices {\nmat4x4 projectionMatrix;\nmat4x4 viewMatrix;\n};\nlayout (std140, binding = 10) uniform Material {\nfloat placeholder;\n};\nout vec4 c;\nvoid main() { gl_Position = projectionMatrix * viewMatrix * vec4(vp, 1.0); c = vc; }\n\0"
#define SHADER_CODE_DEBUG_FS "#version 420 core\nin vec4 c;\nout vec4 fc;\nvoid main() { fc = c; }\n\0"
#define SHADER_CODE_DEBUG_MESH_VS "#version 420 core\n#pragma pass opaque\nlayout (location = 0) in vec3 vp;\nlayout (std140, binding = 1) uniform CameraMatrices {\nmat4x4 projectionMatrix;\nmat4x4 viewMatrix;\n};\nuniform mat4x4 ltwMatrix;\nvoid main() { gl_Position = projectionMatrix * viewMatrix * ltwMatrix * vec4(vp, 1.0); }\n\0"
#define SHADER_CODE_DEBUG_MESH_FS "#version 420 core\nlayout (std140, binding = 10) uniform Material {\nvec4 color;\n};\nout vec4 fc;\nvoid main() { fc = color; }\n\0"
constexpr Vec3f icoSphereVertices[42] = {Vec3f(0.000000, -0.500000, 0.000000), Vec3f(0.361804, -0.223610, 0.262863), Vec3f(-0.138194, -0.223610, 0.425325), Vec3f(-0.447213, -0.223608, 0.000000), Vec3f(-0.138194, -0.223610, -0.425325), Vec3f(0.361804, -0.223610, -0.262863), Vec3f(0.138194, 0.223610, 0.425325), Vec3f(-0.361804, 0.223610, 0.262863), Vec3f(-0.361804, 0.223610, -0.262863), Vec3f(0.138194, 0.223610, -0.425325), Vec3f(0.447213, 0.223608, 0.000000), Vec3f(0.000000, 0.500000, 0.000000), Vec3f(-0.081228, -0.425327, 0.249998), Vec3f(0.212661, -0.425327, 0.154506), Vec3f(0.131434, -0.262869, 0.404506), Vec3f(0.425324, -0.262868, 0.000000), Vec3f(0.212661, -0.425327, -0.154506), Vec3f(-0.262865, -0.425326, 0.000000), Vec3f(-0.344095, -0.262868, 0.249998), Vec3f(-0.081228, -0.425327, -0.249998), Vec3f(-0.344095, -0.262868, -0.249998), Vec3f(0.131434, -0.262869, -0.404506), Vec3f(0.475529, 0.000000, 0.154506), Vec3f(0.475529, 0.000000, -0.154506), Vec3f(0.000000, 0.000000, 0.500000), Vec3f(0.293893, 0.000000, 0.404508), Vec3f(-0.475529, 0.000000, 0.154506), Vec3f(-0.293893, 0.000000, 0.404508), Vec3f(-0.293893, 0.000000, -0.404508), Vec3f(-0.475529, 0.000000, -0.154506), Vec3f(0.293893, 0.000000, -0.404508), Vec3f(0.000000, 0.000000, -0.500000), Vec3f(0.344095, 0.262868, 0.249998), Vec3f(-0.131434, 0.262869, 0.404506), Vec3f(-0.425324, 0.262868, 0.000000), Vec3f(-0.131434, 0.262869, -0.404506), Vec3f(0.344095, 0.262868, -0.249998), Vec3f(0.081228, 0.425327, 0.249998), Vec3f(0.262865, 0.425326, 0.000000), Vec3f(-0.212661, 0.425327, 0.154506), Vec3f(-0.212661, 0.425327, -0.154506), Vec3f(0.081228, 0.425327, -0.249998)};
constexpr unsigned int icoSphereIndices[240] = {0, 12, 13, 1, 15, 13, 0, 17, 12, 0, 19, 17, 0, 16, 19, 1, 22, 15, 2, 24, 14, 3, 26, 18, 4, 28, 20, 5, 30, 21, 1, 25, 22, 2, 27, 24, 3, 29, 26, 4, 31, 28, 5, 23, 30, 6, 37, 32, 7, 39, 33, 8, 40, 34, 9, 41, 35, 10, 38, 36, 38, 11, 41, 38, 41, 36, 36, 41, 9, 41, 11, 40, 41, 40, 35, 35, 40, 8, 40, 11, 39, 40, 39, 34, 34, 39, 7, 39, 11, 37, 39, 37, 33, 33, 37, 6, 37, 11, 38, 37, 38, 32, 32, 38, 10, 23, 10, 36, 23, 36, 30, 30, 36, 9, 31, 9, 35, 31, 35, 28, 28, 35, 8, 29, 8, 34, 29, 34, 26, 26, 34, 7, 27, 7, 33, 27, 33, 24, 24, 33, 6, 25, 6, 32, 25, 32, 22, 22, 32, 10, 30, 9, 31, 30, 31, 21, 21, 31, 4, 28, 8, 29, 28, 29, 20, 20, 29, 3, 26, 7, 27, 26, 27, 18, 18, 27, 2, 24, 6, 25, 24, 25, 14, 14, 25, 1, 22, 10, 23, 22, 23, 15, 15, 23, 5, 16, 5, 21, 16, 21, 19, 19, 21, 4, 19, 4, 20, 19, 20, 17, 17, 20, 3, 17, 3, 18, 17, 18, 12, 12, 18, 2, 15, 5, 16, 15, 16, 13, 13, 16, 0, 12, 2, 14, 12, 14, 13, 13, 14, 1};
constexpr unsigned int wireMeshVertexIncrease = 128;
constexpr unsigned int wireMeshIndexIncrease = 128;
constexpr unsigned int solidMeshVertexIncrease = 128;
constexpr unsigned int solidMeshIndexIncrease = 128;



bool VisualDebug::initialized = false;
ShaderProgram* VisualDebug::debugShader = nullptr;
Material* VisualDebug::debugMaterial = nullptr;
ShaderProgram* VisualDebug::debugMeshShader = nullptr;
Material* VisualDebug::debugMeshMaterial = nullptr;
Mesh* VisualDebug::wireMesh = nullptr;
Mesh* VisualDebug::solidMesh = nullptr;
Mesh* VisualDebug::sphereMesh = nullptr;
unsigned int VisualDebug::wireVerticesIndex = 0;
unsigned int VisualDebug::wireIndicesIndex = 0;
unsigned int VisualDebug::solidVerticesIndex = 0;
unsigned int VisualDebug::solidIndicesIndex = 0;
SimpleList<VisualDebug::MeshCall> VisualDebug::meshCalls(64, 64);
unsigned int VisualDebug::ltwMatrixLocation = 0;


void VisualDebug::drawLine(const Vec3f& from, const Vec3f& to, const Color& color) {
	drawLine(from, to, color, color);
}

void VisualDebug::drawLine(const Vec3f& from, const Vec3f& to, const Color& fromColor, const Color& toColor) {
	initialize();
	addWireVertex(from, fromColor);
	addWireVertex(to, toColor);
	addWireIndex(wireVerticesIndex - 2);
	addWireIndex(wireVerticesIndex - 1);
}

void VisualDebug::drawRay(const Ray3f& ray, const Color& color) {
	drawRay(ray, 1.0f, color);
}

void VisualDebug::drawRay(const Ray3f& ray, float length, const Color& color) {
	initialize();
	addWireVertex(ray.origin, color);
	addWireVertex(ray.origin + ray.direction * length, color);
	addWireIndex(wireVerticesIndex - 2);
	addWireIndex(wireVerticesIndex - 1);
}

void VisualDebug::drawFrustum(const Matrix4x4f& viewMatrix, const Matrix4x4f& projMatrix, float near, float far, const Color& color) {
	initialize();
	Matrix4x4f inv = viewMatrix.inverseAffine() * projMatrix.inversePerspective();
	addWireVertex((inv * Vec4f(-near, -near, -near, near)).xyz, color); // nbl
	addWireVertex((inv * Vec4f(-near, near, -near, near)).xyz, color); // ntl
	addWireVertex((inv * Vec4f(near, near, -near, near)).xyz, color); // ntr
	addWireVertex((inv * Vec4f(near, -near, -near, near)).xyz, color); // nbr
	addWireVertex((inv * Vec4f(-far, -far, far, far)).xyz, color); // fbl
	addWireVertex((inv * Vec4f(-far, far, far, far)).xyz, color); // ftl
	addWireVertex((inv * Vec4f(far, far, far, far)).xyz, color); // ftr
	addWireVertex((inv * Vec4f(far, -far, far, far)).xyz, color); // fbr
	// Near
	addWireIndex(wireVerticesIndex - 8);
	addWireIndex(wireVerticesIndex - 7);
	addWireIndex(wireVerticesIndex - 7);
	addWireIndex(wireVerticesIndex - 6);
	addWireIndex(wireVerticesIndex - 6);
	addWireIndex(wireVerticesIndex - 5);
	addWireIndex(wireVerticesIndex - 5);
	addWireIndex(wireVerticesIndex - 8);
	// Far
	addWireIndex(wireVerticesIndex - 4);
	addWireIndex(wireVerticesIndex - 3);
	addWireIndex(wireVerticesIndex - 3);
	addWireIndex(wireVerticesIndex - 2);
	addWireIndex(wireVerticesIndex - 2);
	addWireIndex(wireVerticesIndex - 1);
	addWireIndex(wireVerticesIndex - 1);
	addWireIndex(wireVerticesIndex - 4);
	// Edges
	addWireIndex(wireVerticesIndex - 8);
	addWireIndex(wireVerticesIndex - 4);
	addWireIndex(wireVerticesIndex - 7);
	addWireIndex(wireVerticesIndex - 3);
	addWireIndex(wireVerticesIndex - 6);
	addWireIndex(wireVerticesIndex - 2);
	addWireIndex(wireVerticesIndex - 5);
	addWireIndex(wireVerticesIndex - 1);
}

void VisualDebug::drawWireCube(const Vec3f& center, const Vec3f& size, const Color& color) {
	initialize();
	Vec3f extents = size * 0.5f;
	addWireVertex(Vec3f(center.x - extents.x, center.y - extents.y, center.z - extents.z), color);
	addWireVertex(Vec3f(center.x - extents.x, center.y - extents.y, center.z + extents.z), color);
	addWireVertex(Vec3f(center.x + extents.x, center.y - extents.y, center.z + extents.z), color);
	addWireVertex(Vec3f(center.x + extents.x, center.y - extents.y, center.z - extents.z), color);
	addWireVertex(Vec3f(center.x - extents.x, center.y + extents.y, center.z - extents.z), color);
	addWireVertex(Vec3f(center.x - extents.x, center.y + extents.y, center.z + extents.z), color);
	addWireVertex(Vec3f(center.x + extents.x, center.y + extents.y, center.z + extents.z), color);
	addWireVertex(Vec3f(center.x + extents.x, center.y + extents.y, center.z - extents.z), color);
	addWireIndex(wireVerticesIndex - 8);
	addWireIndex(wireVerticesIndex - 7);
	addWireIndex(wireVerticesIndex - 7);
	addWireIndex(wireVerticesIndex - 6);
	addWireIndex(wireVerticesIndex - 6);
	addWireIndex(wireVerticesIndex - 5);
	addWireIndex(wireVerticesIndex - 5);
	addWireIndex(wireVerticesIndex - 8);
	addWireIndex(wireVerticesIndex - 4);
	addWireIndex(wireVerticesIndex - 3);
	addWireIndex(wireVerticesIndex - 3);
	addWireIndex(wireVerticesIndex - 2);
	addWireIndex(wireVerticesIndex - 2);
	addWireIndex(wireVerticesIndex - 1);
	addWireIndex(wireVerticesIndex - 1);
	addWireIndex(wireVerticesIndex - 4);
	addWireIndex(wireVerticesIndex - 8);
	addWireIndex(wireVerticesIndex - 4);
	addWireIndex(wireVerticesIndex - 7);
	addWireIndex(wireVerticesIndex - 3);
	addWireIndex(wireVerticesIndex - 6);
	addWireIndex(wireVerticesIndex - 2);
	addWireIndex(wireVerticesIndex - 5);
	addWireIndex(wireVerticesIndex - 1);
}

void VisualDebug::drawWireMesh(const Mesh* mesh, const Vec3f& position, const Rotor3f& rotation, const Vec3f& scale, const Color& color) {
	Matrix4x4f ltw = Matrix4x4f::transformation(position, scale, rotation);
	drawWireMesh(mesh, ltw, color);
}

void VisualDebug::drawWireMesh(const Mesh* mesh, const Matrix4x4f& ltwMatrix, const Color& color) {
	if (mesh->getTopology() != GL_TRIANGLES) {
		Debug::logError("VisualDebug", "drawWireMesh using non Triangle topology is not supported!");
		return;
	}
	initialize();
	int stride = 0;
	int vertexCount = mesh->getVertexCount();
	unsigned char* vertices = (unsigned char*) mesh->getAttributePointer(0, stride);
	for (int vi = 0; vi < vertexCount * stride; vi += stride) {
		addWireVertex(ltwMatrix.multPoint(*((Vec3f*) (vertices + vi))), color);
	}
	int indexCount = mesh->getIndexCount();
	unsigned int* indices = mesh->getIndicesPointer();
	for (int ii = 0; ii < indexCount; ii += 3) {
		addWireIndex(wireVerticesIndex - (vertexCount - indices[ii]));
		addWireIndex(wireVerticesIndex - (vertexCount - indices[ii + 1]));
		addWireIndex(wireVerticesIndex - (vertexCount - indices[ii + 1]));
		addWireIndex(wireVerticesIndex - (vertexCount - indices[ii + 2]));
		addWireIndex(wireVerticesIndex - (vertexCount - indices[ii + 2]));
		addWireIndex(wireVerticesIndex - (vertexCount - indices[ii]));
	}
}

void VisualDebug::drawWireSphere(const Vec3f& center, float radius, const Color& color) {
	initialize();
	// TODO optimize
	drawWireMesh(sphereMesh, Matrix4x4f::transformation(center, Vec3f::one * radius, Rotor3f::identity), color);
}

void VisualDebug::drawWireDisk(const Vec3f& center, const Vec3f& normal, float radius, const Color& color) {
	Vec3f right;
	if (normal == Vec3f::up) {
		right = Vec3f::forward.cross(normal);
	} else {
		right = Vec3f::up.cross(normal);
	}
	right.normalize();
	Vec3f up = normal.cross(right);
	up.normalize();
	const unsigned int subDivs = 32;
	for (unsigned int i = 0; i < subDivs; i++) {
		float perim = (i / (float) subDivs) * IonEngine::Math::two_pi;
		addWireVertex(center + cosf(perim) * radius * right + sinf(perim) * radius * up, color);
	}
	for (unsigned int i = 0; i < subDivs - 1; i++) {
		addWireIndex(wireVerticesIndex - i - 1);
		addWireIndex(wireVerticesIndex - i - 2);
	}
	addWireIndex(wireVerticesIndex - subDivs);
	addWireIndex(wireVerticesIndex - 1);
}

void VisualDebug::drawWirePlaneDisk(const Plane& plane, float normalLength, float diskRadius, const Color& color) {
	Vec3f center = plane.normal * plane.distance;
	drawLine(center, center + plane.normal * normalLength, color);
	drawWireDisk(center, plane.normal, diskRadius, color);
	drawWireDisk(center, plane.normal, diskRadius * 0.66f, color);
}

void VisualDebug::drawWirePlaneDisk(const Plane& plane, const Vec3f& unprojectedCenter, float normalLength, float diskRadius, const Color& color) {
	Vec3f center = plane.projectPoint(unprojectedCenter);
	drawLine(center, center + plane.normal * normalLength, color);
	drawWireDisk(center, plane.normal, diskRadius, color);
	drawWireDisk(center, plane.normal, diskRadius * 0.66f, color);
}

void VisualDebug::drawCube(const Vec3f& center, const Vec3f& size, const Color& color) {
	initialize();
	Vec3f extents = size * 0.5f;
	addSolidVertex(Vec3f(center.x - extents.x, center.y - extents.y, center.z - extents.z), color);
	addSolidVertex(Vec3f(center.x - extents.x, center.y - extents.y, center.z + extents.z), color);
	addSolidVertex(Vec3f(center.x + extents.x, center.y - extents.y, center.z + extents.z), color);
	addSolidVertex(Vec3f(center.x + extents.x, center.y - extents.y, center.z - extents.z), color);
	addSolidVertex(Vec3f(center.x - extents.x, center.y + extents.y, center.z - extents.z), color);
	addSolidVertex(Vec3f(center.x - extents.x, center.y + extents.y, center.z + extents.z), color);
	addSolidVertex(Vec3f(center.x + extents.x, center.y + extents.y, center.z + extents.z), color);
	addSolidVertex(Vec3f(center.x + extents.x, center.y + extents.y, center.z - extents.z), color);
	// Bottom
	addSolidIndex(solidVerticesIndex - 8);
	addSolidIndex(solidVerticesIndex - 7);
	addSolidIndex(solidVerticesIndex - 6);
	addSolidIndex(solidVerticesIndex - 8);
	addSolidIndex(solidVerticesIndex - 6);
	addSolidIndex(solidVerticesIndex - 5);
	// Top
	addSolidIndex(solidVerticesIndex - 4);
	addSolidIndex(solidVerticesIndex - 2);
	addSolidIndex(solidVerticesIndex - 3);
	addSolidIndex(solidVerticesIndex - 4);
	addSolidIndex(solidVerticesIndex - 1);
	addSolidIndex(solidVerticesIndex - 2);
	// Front
	addSolidIndex(solidVerticesIndex - 8);
	addSolidIndex(solidVerticesIndex - 1);
	addSolidIndex(solidVerticesIndex - 4);
	addSolidIndex(solidVerticesIndex - 8);
	addSolidIndex(solidVerticesIndex - 5);
	addSolidIndex(solidVerticesIndex - 1);
	// Back
	addSolidIndex(solidVerticesIndex - 6);
	addSolidIndex(solidVerticesIndex - 7);
	addSolidIndex(solidVerticesIndex - 3);
	addSolidIndex(solidVerticesIndex - 6);
	addSolidIndex(solidVerticesIndex - 3);
	addSolidIndex(solidVerticesIndex - 2);
	// Left
	addSolidIndex(solidVerticesIndex - 7);
	addSolidIndex(solidVerticesIndex - 8);
	addSolidIndex(solidVerticesIndex - 4);
	addSolidIndex(solidVerticesIndex - 7);
	addSolidIndex(solidVerticesIndex - 4);
	addSolidIndex(solidVerticesIndex - 3);
	// Right
	addSolidIndex(solidVerticesIndex - 5);
	addSolidIndex(solidVerticesIndex - 6);
	addSolidIndex(solidVerticesIndex - 2);
	addSolidIndex(solidVerticesIndex - 5);
	addSolidIndex(solidVerticesIndex - 2);
	addSolidIndex(solidVerticesIndex - 1);
}

void VisualDebug::drawMesh(const Mesh* mesh, const Vec3f& position, const Rotor3f& rotation, const Vec3f& scale, const Color& color) {
	Matrix4x4f ltw = Matrix4x4f::transformation(position, scale, rotation);
	drawMesh(mesh, ltw, color);
}

void VisualDebug::drawMesh(const Mesh* mesh, const Matrix4x4f& ltwMatrix, const Color& color) {
	initialize();
	meshCalls.add({mesh, ltwMatrix, color});
}

void VisualDebug::drawSphere(const Vec3f& position, float radius, const Color& color) {
	initialize();
	// TODO optimize
	// avoid 1 draw call per sphere
	meshCalls.add({sphereMesh, Matrix4x4f::transformation(position, Vec3f::one * radius, Rotor3f::identity), color});
}

void VisualDebug::render() {
	if (!initialized) return;
	glDepthMask(GL_FALSE);
	wireMesh->updateInGL();
	solidMesh->updateInGL();

	debugMeshMaterial->specializedProgram->use();
	debugMeshMaterial->use();
	for (unsigned int i = 0; i < meshCalls.count; i++) {
		debugMeshMaterial->setField(0, meshCalls[i].color.vec);
		debugMeshMaterial->updateFields();
		debugMeshMaterial->specializedProgram->loadMatrix4x4f(ltwMatrixLocation, meshCalls[i].transf);
		meshCalls[i].mesh->render();
	}
	debugMeshMaterial->specializedProgram->unuse();
	meshCalls.clear();

	debugMaterial->specializedProgram->use();
	debugMaterial->use();
	solidMesh->render();
	wireMesh->render();
	debugMaterial->specializedProgram->unuse();

	wireVerticesIndex = 0;
	wireIndicesIndex = 0;
	wireMesh->setDrawnIndexCount(0);
	solidVerticesIndex = 0;
	solidIndicesIndex = 0;
	solidMesh->setDrawnIndexCount(0);
	glDepthMask(GL_TRUE);
}

void VisualDebug::initialize() {
	if (initialized) return;
	initializeMaterials();
	initializeMeshes();

	initialized = true;
}

void VisualDebug::initializeMaterials() {
	debugShader = ShaderProgram::createFromRaw("RAW_DEBUG", SHADER_CODE_DEBUG_VS, SHADER_CODE_DEBUG_FS);
	debugShader->load();
	debugMaterial = new Material("DebugMat", debugShader, "opaque");
	debugMeshShader = ShaderProgram::createFromRaw("RAW_DEBUG_MESH", SHADER_CODE_DEBUG_MESH_VS, SHADER_CODE_DEBUG_MESH_FS);
	debugMeshShader->load();
	debugMeshMaterial = new Material("DebugMeshMat", debugMeshShader, "opaque");
	debugMeshMaterial->setField(0, Color::magenta.vec);
	ltwMatrixLocation = debugMeshMaterial->specializedProgram->getUniformLocation("ltwMatrix");
}

void VisualDebug::initializeMeshes() {
	wireMesh = new Mesh("VD Wire Mesh", wireMeshVertexIncrease, wireMeshIndexIncrease);
	wireMesh->setAttributesDefinition(2, new int[2]{3, 4}, new GLenum[2]{GL_FLOAT, GL_FLOAT}); // attr0: vertex pos, attr1: color
	wireMesh->setDrawnIndexCount(0);
	wireMesh->setUsageHint(GL_DYNAMIC_DRAW);
	wireMesh->setTopology(GL_LINES);
	wireMesh->uploadToGL();
	solidMesh = new Mesh("VD Solid Mesh", wireMeshVertexIncrease, wireMeshIndexIncrease);
	solidMesh->setAttributesDefinition(2, new int[2]{3, 4}, new GLenum[2]{GL_FLOAT, GL_FLOAT}); // attr0: vertex pos, attr1: color
	solidMesh->setDrawnIndexCount(0);
	solidMesh->setUsageHint(GL_DYNAMIC_DRAW);
	solidMesh->uploadToGL();

	sphereMesh = new Mesh("VD Sphere", 42, 240);
	sphereMesh->setAttributesDefinition(1, new int[1]{3});
	sphereMesh->setAttribute(0, (float*) icoSphereVertices);
	sphereMesh->setIndices((unsigned int*) icoSphereIndices);
	sphereMesh->uploadToGL();
}

void VisualDebug::addWireVertex(Vec3f pos, Color col) {
	int vertexCapacity = wireMesh->getVertexCount();
	if (wireVerticesIndex >= vertexCapacity) {
		vertexCapacity += wireMeshVertexIncrease;
		wireMesh->resize(vertexCapacity, wireMesh->getIndexCount(), true, Mesh::ResizeMode::GrowOrShrinkQuarter);
	}
	wireMesh->setAttributeElement(0, wireVerticesIndex, pos);
	wireMesh->setAttributeElement(1, wireVerticesIndex++, col.vec);
}

void VisualDebug::addWireIndex(int index) {
	int indexCapacity = wireMesh->getIndexCount();
	if (wireIndicesIndex >= indexCapacity) {
		indexCapacity += wireMeshIndexIncrease;
		wireMesh->resize(wireMesh->getVertexCount(), indexCapacity, true, Mesh::ResizeMode::GrowOrShrinkQuarter);
	}
	wireMesh->setIndex(wireIndicesIndex++, index);
	wireMesh->setDrawnIndexCount(wireIndicesIndex);
}

void VisualDebug::addSolidVertex(const Vec3f& pos, const Color& col) {
	int vertexCapacity = solidMesh->getVertexCount();
	if (solidVerticesIndex >= vertexCapacity) {
		vertexCapacity += solidMeshVertexIncrease;
		solidMesh->resize(vertexCapacity, solidMesh->getIndexCount(), true, Mesh::ResizeMode::GrowOrShrinkQuarter);
	}
	solidMesh->setAttributeElement(0, solidVerticesIndex, pos);
	solidMesh->setAttributeElement(1, solidVerticesIndex++, col.vec);
}

void VisualDebug::addSolidIndex(int index) {
	int indexCapacity = solidMesh->getIndexCount();
	if (solidIndicesIndex >= indexCapacity) {
		indexCapacity += solidMeshIndexIncrease;
		solidMesh->resize(solidMesh->getVertexCount(), indexCapacity, true, Mesh::ResizeMode::GrowOrShrinkQuarter);
	}
	solidMesh->setIndex(solidIndicesIndex++, index);
	solidMesh->setDrawnIndexCount(solidIndicesIndex);
}
