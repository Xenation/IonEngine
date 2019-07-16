#include "SpecializedShaderProgram.h"

#include <imgui.h>
#include "Debug.h"
#include "ShaderProgram.h"
#include "RenderPass.h"
#include "Pipeline.h"
#include "Material.h"

#define SHADER_CODE_ERROR_VS "#version 420 core\n#pragma pass opaque\nlayout (location = 0) in vec3 vp;\nlayout (std140, binding = 1) uniform CameraMatrices {\nmat4x4 projectionMatrix;\nmat4x4 viewMatrix;\n};\nuniform mat4x4 modelMatrix;\nlayout (std140, binding = 10) uniform Material {\nfloat placeholder;\n};\nvoid main() { gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vp, 1.0); }\n\0"
#define SHADER_CODE_ERROR_FS "#version 420 core\nout vec4 fc;\nvoid main() { fc = vec4(1.0, 0.0, 1.0, 1.0); }\n\0"



SpecializedShaderProgram* SpecializedShaderProgram::errorShader = nullptr;

void SpecializedShaderProgram::initialize(Pipeline* pipeline) {
	// Loading Error shader
	//errorShader = new SpecializedShaderProgram(nullptr, pipeline->renderPasses[0]);
	//errorShader->load(errorShader->loadShaderFromSource(GL_VERTEX_SHADER, SHADER_CODE_ERROR_VS), 0, 0, 0, errorShader->loadShaderFromSource(GL_FRAGMENT_SHADER, SHADER_CODE_ERROR_FS));
}


SpecializedShaderProgram::SpecializedShaderProgram(ShaderProgram*const parentShader, RenderPass* renderPass) : parentShader(parentShader), renderPass(renderPass), materials(4, 4) {}

SpecializedShaderProgram::~SpecializedShaderProgram() {}

void SpecializedShaderProgram::gui() {
	if (renderPass == nullptr) return;
	uint matProcessed = 0;
	for (uint matIndex = 0; matIndex < materials.capacity && matProcessed < materials.count; matIndex++) {
		if (materials[matIndex] == nullptr) continue;
		materials[matIndex]->gui();
		matProcessed++;
	}
}

void SpecializedShaderProgram::load(const char* vsSource[3], const char* tcsSource[3], const char* tesSource[3], const char* gsSource[3], const char* fsSource[3]) {
	if (renderPass != nullptr) {
		Debug::log("SpecShaderProgram", "Loading Program for " + renderPass->name + " pass ...");
	}

	if (vsSource != nullptr) {
		vertex = loadShaderFromSourceArray(GL_VERTEX_SHADER, vsSource, 3);
	} else {
		Debug::log("SpecShaderProgram", "Tried loading a specialized program with no vertex source! using error fallback ...");
	}
	if (tcsSource != nullptr) {
		tessControl = loadShaderFromSourceArray(GL_TESS_CONTROL_SHADER, tcsSource, 3);
	}
	if (tesSource != nullptr) {
		tessEvaluation = loadShaderFromSourceArray(GL_TESS_EVALUATION_SHADER, tesSource, 3);
	}
	if (gsSource != nullptr) {
		geometry = loadShaderFromSourceArray(GL_GEOMETRY_SHADER, gsSource, 3);
	}
	if (fsSource != nullptr) {
		fragment = loadShaderFromSourceArray(GL_FRAGMENT_SHADER, fsSource, 3);
	} else {
		Debug::log("SpecShaderProgram", "Tried loading a specialized program with no fragment source! using error fallback ...");
	}

	if (vertex == 0 || fragment == 0) {
		vertex = loadShaderFromSource(GL_VERTEX_SHADER, SHADER_CODE_ERROR_VS, true);
		fragment = loadShaderFromSource(GL_VERTEX_SHADER, SHADER_CODE_ERROR_FS, true);
	}

	load(vertex, tessControl, tessEvaluation, geometry, fragment);
}

void SpecializedShaderProgram::reload(const char* vsSource[3], const char* tcsSource[3], const char* tesSource[3], const char* gsSource[3], const char* fsSource[3]) {
	if (loaded) {
		unload();
	}
	load(vsSource, tcsSource, tesSource, gsSource, fsSource);
	uint reloadedMaterials = 0;
	for (uint i = 0; i < materials.capacity && reloadedMaterials < materials.count; reloadedMaterials++) {
		if (materials[i] == nullptr) continue;
		materials[i]->reload();
		reloadedMaterials++;
	}
}

void SpecializedShaderProgram::unload() {
	if (renderPass != nullptr) {
		renderPass->programs.remove(idInPass);
	}
	if (program != 0) {
		if (vertex != 0) {
			glDetachShader(program, vertex);
			glDeleteShader(vertex);
			vertex = 0;
		}
		if (tessControl != 0) {
			glDetachShader(program, tessControl);
			glDeleteShader(tessControl);
			tessControl = 0;
		}
		if (tessEvaluation != 0) {
			glDetachShader(program, tessEvaluation);
			glDeleteShader(tessEvaluation);
			tessEvaluation = 0;
		}
		if (geometry != 0) {
			glDetachShader(program, geometry);
			glDeleteShader(geometry);
			geometry = 0;
		}
		if (fragment != 0) {
			glDetachShader(program, fragment);
			glDeleteShader(fragment);
			fragment = 0;
		}
		glDeleteProgram(program);
		program = 0;
	}
}

void SpecializedShaderProgram::use() {
	glUseProgram(program);
}

void SpecializedShaderProgram::unuse() {
	glUseProgram(0);
}

GLuint SpecializedShaderProgram::getUniformLocation(const char* str) {
	return glGetUniformLocation(program, str);
}

GLuint SpecializedShaderProgram::getUniformLocation(std::string str) {
	return getUniformLocation(str.c_str());
}

void SpecializedShaderProgram::loadBool(GLuint location, bool b) {
	glUniform1i(location, (int) b);
}
void SpecializedShaderProgram::loadInt(GLuint location, int i) {
	glUniform1i(location, i);
}
void SpecializedShaderProgram::loadUInt(GLuint location, uint u) {
	glUniform1ui(location, u);
}
void SpecializedShaderProgram::loadFloat(GLuint location, float f) {
	glUniform1f(location, f);
}
void SpecializedShaderProgram::loadDouble(GLuint location, double d) {
	glUniform1d(location, d);
}
void SpecializedShaderProgram::loadVec2f(GLuint location, Vec2f vec) {
	glUniform2f(location, vec.x, vec.y);
}
void SpecializedShaderProgram::loadVec3f(GLuint location, Vec3f vec) {
	glUniform3f(location, vec.x, vec.y, vec.z);
}
void SpecializedShaderProgram::loadVec4f(GLuint location, Vec4f vec) {
	glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}
void SpecializedShaderProgram::loadMatrix4x4f(GLuint location, Matrix4x4f matrix) {
	glUniformMatrix4fv(location, 1, false, matrix.data);
}


void SpecializedShaderProgram::load(GLuint vs, GLuint tcs, GLuint tes, GLuint gs, GLuint fs, bool silent) {
	if (vs <= 0 || fs <= 0) return;

	program = glCreateProgram();
	std::string label("Program " + parentShader->name + "/" + renderPass->name);
	glObjectLabel(GL_PROGRAM, program, label.size(), label.c_str());
	if (vs > 0) {
		glAttachShader(program, vs);
	}
	if (tcs > 0) {
		glAttachShader(program, tcs);
	}
	if (tes > 0) {
		glAttachShader(program, tes);
	}
	if (gs > 0) {
		glAttachShader(program, gs);
	}
	if (fs > 0) {
		glAttachShader(program, fs);
	}

	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		if (!silent) {
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			char* infoLog = new char[maxLength];
			memset(infoLog, 0, maxLength * sizeof(char));

			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

			std::string error = "Could not link";
			if (vs != 0) {
				error += " [vertex shader]";
			}
			if (tcs != 0) {
				error += " [tesselation control shader]";
			}
			if (tes != 0) {
				error += " [tesselation evaluation shader]";
			}
			if (gs != 0) {
				error += " [geometry shader]";
			}
			if (fs != 0) {
				error += " [fragment shader]";
			}

			error += ", log is: " + std::string(infoLog);
			Debug::logError("ShaderProgram", error.c_str());

			delete[] infoLog;
		}

		glDeleteProgram(program);
		program = 0;
		return;
	}
	if (renderPass != nullptr) {
		idInPass = renderPass->programs.add(this);
	}
	Debug::log("ShaderProgram", "Program load and link done!");
	loaded = true;
}

GLuint SpecializedShaderProgram::loadShaderFromSource(GLenum type, const char* src, bool silent) {
	return loadShaderFromSourceArray(type, &src, 1, silent);
}

GLuint SpecializedShaderProgram::loadShaderFromSourceArray(GLenum type, const char** src, uint srcPiecesCount, bool silent) {
	GLuint shader = 0;
	GLsizei logSize = 0;
	GLint compileStatus = GL_TRUE;
	char* log = nullptr;
	std::string typeStr;
	std::string shortTypeStr;
	switch (type) {
	case GL_VERTEX_SHADER:
		typeStr = std::string("Vertex");
		shortTypeStr = std::string("vs");
		break;
	case GL_TESS_CONTROL_SHADER:
		typeStr = std::string("Tesselation Control");
		shortTypeStr = std::string("tcs");
		break;
	case GL_TESS_EVALUATION_SHADER:
		typeStr = std::string("Tesselation Evaluation");
		shortTypeStr = std::string("tes");
		break;
	case GL_FRAGMENT_SHADER:
		typeStr = std::string("Fragment");
		shortTypeStr = std::string("fs");
		break;
	case GL_GEOMETRY_SHADER:
		typeStr = std::string("Geometry");
		shortTypeStr = std::string("gs");
		break;
	}

	shader = glCreateShader(type);
	if (shader == 0) {
		if (!silent) Debug::logError("ShaderLoad", "Program  " + parentShader->name + " could not create " + typeStr + " Shader");
		return 0;
	}
	std::string label("Shader " + parentShader->name + "/" + renderPass->name + "/" + shortTypeStr);
	glObjectLabel(GL_SHADER, shader, label.size(), label.c_str());

	glShaderSource(shader, srcPiecesCount, (const GLchar**) src, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

	std::string logStr;
	if (!silent) {
		log = new char[logSize + 1];
		if (log == nullptr) {
			Debug::logError("ShaderLoad", "Program " + parentShader->name + " could not allocate memory for " + typeStr + " Shader compilation log!");
			return 0;
		}
		memset(log, '\0', logSize + 1);

		glGetShaderInfoLog(shader, logSize, &logSize, log);

		logStr = std::string(log);
	}

	if (compileStatus != GL_TRUE) {
		if (!silent) {
			Debug::logError("ShaderLoad", "Program " + parentShader->name + " could not compile " + typeStr + " Shader: \n" + logStr);
			delete[] log;
		}
		glDeleteShader(shader);
		return 0;
	} else {
		if (!silent) {
			Debug::log("ShaderLoad", "Program " + parentShader->name + " " + typeStr + " Shader compile success");
			if (logStr.length() > 0) {
				Debug::log("ShaderLoad", ("Compile result " + typeStr + " Shader:\n" + logStr).c_str());
			}
			delete[] log;
		}
	}

	return shader;
}
