#include "ComputeShader.h"

#include <filesystem>
#include "Debug.h"
#include "ShaderPreprocessor.h"
using namespace IonEngine;

#define SHADER_DIRECTORY "res/shaders/"



namespace fs = std::filesystem;


ComputeShader::ComputeShader(std::string name) : name(name) {}

ComputeShader::~ComputeShader() {}


void ComputeShader::load() {
	// READ SHADER FILE
	fs::path dirPath = SHADER_DIRECTORY / fs::path(name);
	if (!fs::exists(dirPath)) {
		Debug::logError("ComputeShader", "Could not find a compute shader at " + dirPath.string());
		return;
	}

	ShaderPreprocessor* reader = new ShaderPreprocessor(dirPath);

	if (!reader->read()) {
		Debug::log("ComputeShader", "Failed to Read shader file!");
		delete reader;
		return;
	}

	if (!reader->generate()) {
		Debug::log("ComputeShader", "Failed to Generate shader sources!");
		delete reader;
		return;
	}

	// COMPILE SHADER
	loadComputeShaderFromSourceArray(reader->csSources->source, 3);
	loadProgram();

	delete reader;
}

void ComputeShader::reload() {
	if (loaded) {
		unload();
	}
	load();
}

void ComputeShader::unload() {
	if (program != 0) {
		if (compute != 0) {
			glDetachShader(program, compute);
			glDeleteShader(compute);
			compute = 0;
		}
		glDeleteProgram(program);
		program = 0;
	}
}

void ComputeShader::use() {
	glUseProgram(program);
}

void ComputeShader::unuse() {
	glUseProgram(0);
}

void ComputeShader::loadProgram(bool silent) {
	if (compute <= 0) return;

	program = glCreateProgram();
	std::string label = std::string("ComputeProgram " + name);
	glObjectLabel(GL_PROGRAM, program, label.size(), label.c_str());
	if (compute > 0) {
		glAttachShader(program, compute);
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
			if (compute != 0) {
				error += " [compute shader]";
			}

			error += ", log is: \n" + std::string(infoLog);
			Debug::logError("ComputeProgram", error.c_str());

			delete[] infoLog;
		}

		glDeleteProgram(program);
		program = 0;
		return;
	}
	Debug::log("ComputeProgram", "Program load and link done!");
	loaded = true;
}

void ComputeShader::loadComputeShaderFromSourceArray(const char** src, unsigned int srcPiecesCount, bool silent) {
	GLuint shader = 0;
	GLsizei logSize = 0;
	GLint compileStatus = GL_TRUE;
	char* log = nullptr;

	shader = glCreateShader(GL_COMPUTE_SHADER);
	if (shader == 0) {
		if (!silent) Debug::logError("ComputeLoad", "Program  " + name + " could not create Compute Shader");
		return;
	}
	std::string label = std::string("ComputeShader " + name + "/cs");
	glObjectLabel(GL_SHADER, shader, label.size(), label.c_str());

	glShaderSource(shader, srcPiecesCount, (const GLchar**)src, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

	std::string logStr;
	if (!silent) {
		log = new char[logSize + 1];
		if (log == nullptr) {
			Debug::logError("ComputeLoad", "Program " + name + " could not allocate memory for Compute Shader compilation log!");
			return;
		}
		memset(log, '\0', logSize + 1);

		glGetShaderInfoLog(shader, logSize, &logSize, log);

		logStr = std::string(log);
	}

	if (compileStatus != GL_TRUE) {
		if (!silent) {
			Debug::logError("ComputeLoad", "Program " + name + " could not compile Compute Shader: \n" + logStr);
			delete[] log;
		}
		glDeleteShader(shader);
		return;
	} else {
		if (!silent) {
			Debug::log("ComputeLoad", "Program " + name + " Compute Shader compile success");
			if (logStr.length() > 0) {
				Debug::log("ComputeLoad", ("Compile result Compute Shader:\n" + logStr).c_str());
			}
			delete[] log;
		}
	}

	compute = shader;
}
