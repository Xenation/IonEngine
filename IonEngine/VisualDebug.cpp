#include "VisualDebug.h"

#include "Mesh.h"
#include "ShaderProgram.h"
#include "SpecializedShaderProgram.h"
#include "Material.h"

#define SHADER_CODE_DEBUG_VS "#version 420 core\n#pragma pass opaque\nlayout (location = 0) in vec3 vp;\nlayout (location = 1) in vec4 vc;\nlayout (std140, binding = 1) uniform CameraMatrices {\nmat4x4 projectionMatrix;\nmat4x4 viewMatrix;\n};\nlayout (std140, binding = 10) uniform Material {\nfloat placeholder;\n};\nout vec4 c;\nvoid main() { gl_Position = projectionMatrix * viewMatrix * vec4(vp, 1.0); c = vc; }\n\0"
#define SHADER_CODE_DEBUG_FS "#version 420 core\nin vec4 c;\nout vec4 fc;\nvoid main() { fc = c; }\n\0"



bool VisualDebug::initialized = false;
ShaderProgram* VisualDebug::debugShader = nullptr;
Material* VisualDebug::debugMaterial = nullptr;
Mesh* VisualDebug::linesMesh = nullptr;
SimpleList<VisualDebug::LineCall> VisualDebug::lineCalls(4, 4);


void VisualDebug::drawLine(Vec3f from, Vec3f to, Color color) {
	drawLine(from, to, color, color);
}

void VisualDebug::drawLine(Vec3f from, Vec3f to, Color fromColor, Color toColor) {
	lineCalls.add({ from, to, fromColor, toColor });
}

void VisualDebug::render() {
	prepareRender();
	if (debugMaterial == nullptr) return;
	debugMaterial->specializedProgram->use();
	debugMaterial->use();
	linesMesh->render();
	debugMaterial->specializedProgram->unuse();
	cleanupRender();
}

void VisualDebug::prepareRender() {
	// Shader
	if (debugShader == nullptr) {
		debugShader = ShaderProgram::createFromRaw("RAW_DEBUG", SHADER_CODE_DEBUG_VS, SHADER_CODE_DEBUG_FS);
		debugShader->load();
		debugMaterial = new Material("DebugMat", debugShader, "opaque");
	}
	// Lines
	if (linesMesh == nullptr) {
		linesMesh = new Mesh("Line Mesh", lineCalls.count * 2, lineCalls.count * 2);
		linesMesh->setAttributesDefinition(2, new int[2]{3, 4}, new GLenum[2]{ GL_FLOAT, GL_FLOAT });
		linesMesh->setUsageHint(GL_DYNAMIC_DRAW);
		linesMesh->setTopology(GL_LINES);
	} else {
		linesMesh->resize(lineCalls.count * 2, lineCalls.count * 2, false, Mesh::ResizeMode::GrowOrShrinkQuarter);
	}
	for (unsigned int i = 0; i < lineCalls.count; i++) {
		linesMesh->setAttributeElement(0, i * 2, lineCalls[i].from);
		linesMesh->setAttributeElement(0, i * 2 + 1, lineCalls[i].to);
		linesMesh->setAttributeElement(1, i * 2, lineCalls[i].fromColor.vec);
		linesMesh->setAttributeElement(1, i * 2 + 1, lineCalls[i].toColor.vec);
		linesMesh->setIndex(i * 2, i * 2);
		linesMesh->setIndex(i * 2 + 1, i * 2 + 1);
	}
	if (linesMesh->isLoadedToGL()) {
		linesMesh->updateInGL();
	} else {
		linesMesh->uploadToGL();
	}
}

void VisualDebug::cleanupRender() {
	lineCalls.clear();
}
