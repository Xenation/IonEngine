#pragma once
#include "XMath.h"
#include "SimpleList.h"

class Mesh;
class ShaderProgram;
class Material;

class VisualDebug {
public:
	VisualDebug() = delete;
	VisualDebug(const VisualDebug&) = delete;
	~VisualDebug() = delete;

	static void drawLine(Vec3f from, Vec3f to, Color color);
	static void drawLine(Vec3f from, Vec3f to, Color fromColor, Color toColor);

	static void render();

private:

	struct LineCall {
		Vec3f from, to;
		Color fromColor, toColor;
	};

	static bool initialized;
	static ShaderProgram* debugShader;
	static Material* debugMaterial;
	static Mesh* linesMesh;
	static SimpleList<LineCall> lineCalls;

	static void prepareRender();
	static void cleanupRender();
};

