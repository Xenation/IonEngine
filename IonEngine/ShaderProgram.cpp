#include "ShaderProgram.h"

#include <filesystem>
#include <imgui.h>
#include "Debug.h"
#include "ShaderPreprocessor.h"
#include "SpecializedShaderProgram.h"
#include "Pipeline.h"
#include "RenderPass.h"

#define SHADER_DIRECTORY "res/shaders/"



namespace fs = std::filesystem;

ShaderProgram** ShaderProgram::shaders = nullptr;
uint ShaderProgram::shaderCount = 0;
Pipeline* ShaderProgram::defaultPipeline = nullptr;

void ShaderProgram::initializeAll(Pipeline* pipeline) {
	defaultPipeline = pipeline;

	Debug::log("==== Shaders Init ====");
	// Shader search
	if (!fs::exists(SHADER_DIRECTORY)) {
		Debug::log("ShaderInit", "Shader directory " + std::string(SHADER_DIRECTORY) + " not found!");
		return;
	}
	for (fs::directory_entry entry : fs::directory_iterator(SHADER_DIRECTORY)) { // Counting pass
		if (!entry.is_directory()) continue;
		shaderCount++;
	}
	Debug::log("ShaderInit", "Found " + std::to_string(shaderCount) + " potential shaders:");
	shaders = new ShaderProgram*[shaderCount];
	uint index = 0;
	for (fs::directory_entry entry : fs::directory_iterator(SHADER_DIRECTORY)) {
		if (!entry.is_directory()) continue;
		shaders[index] = new ShaderProgram(entry.path().filename().string());
		Debug::log("ShaderInit", "    " + shaders[index]->name);
		index++;
	}

}

void ShaderProgram::reloadAll() {
	Debug::log("==== Shaders Reload ====");
	for (uint i = 0; i < shaderCount; i++) {
		shaders[i]->reload();
	}
}

ShaderProgram* ShaderProgram::find(std::string name) {
	for (uint i = 0; i < shaderCount; i++) {
		ShaderProgram* shader = shaders[i];
		if (shader->name == name) {
			return shader;
		}
	}
	Debug::log("ShaderFind", "Could not find shader program named " + name);
	return nullptr;
}

ShaderProgram* ShaderProgram::createFromRaw(std::string name, std::string vs, std::string fs) {
	ShaderProgram* rawProgram = new ShaderProgram(name);
	rawProgram->rawVS = vs;
	rawProgram->rawFS = fs;
	rawProgram->useRaw = true;
	return rawProgram;
}

void ShaderProgram::guiAll() {
	for (uint programIndex = 0; programIndex < shaderCount; programIndex++) {
		shaders[programIndex]->gui();
	}
}



ShaderProgram::ShaderProgram(std::string name) : name(name) {

}

ShaderProgram::~ShaderProgram() {
	if (loaded) {
		unload();
	}
	if (specializedPrograms != nullptr) {
		for (uint i = 0; i < specializedProgramsCount; i++) {
			delete specializedPrograms[i];
		}
		delete[] specializedPrograms;
	}
	if (info != nullptr) {
		delete info;
	}
}

void ShaderProgram::gui() {
	for (uint specProgramIndex = 0; specProgramIndex < specializedProgramsCount; specProgramIndex++) {
		specializedPrograms[specProgramIndex]->gui();
	}
}

void ShaderProgram::load() {
	if (loaded) return;
	ShaderPreprocessor* reader = readShaders();
	if (reader == nullptr) return;
	info = reader->programInfo;
	
	specializedProgramsCount = info->passCount;
	specializedPrograms = new SpecializedShaderProgram*[specializedProgramsCount];
	for (uint i = 0; i < specializedProgramsCount; i++) {
		specializedPrograms[i] = new SpecializedShaderProgram(this, defaultPipeline->getRenderPass(info->passNames[i]));
		specializedPrograms[i]->load((reader->vsSources == nullptr) ? nullptr : reader->vsSources[i].source, (reader->tcsSources == nullptr) ? nullptr : reader->tcsSources[i].source, (reader->tesSources == nullptr) ? nullptr : reader->tesSources[i].source, (reader->gsSources == nullptr) ? nullptr : reader->gsSources[i].source, (reader->fsSources == nullptr) ? nullptr : reader->fsSources[i].source);
	}

	loaded = true;
	delete reader;
}

void ShaderProgram::reload() {
	if (!loaded) return;

	ShaderPreprocessor* reader = readShaders();
	if (reader == nullptr) return;
	
	if (reader->programInfo->passCount != info->passCount) {
		if (specializedPrograms != nullptr) { // TODO avoid deleting specialized programs that already existed
			for (uint i = 0; i < specializedProgramsCount; i++) {
				delete specializedPrograms[i];
			}
			delete[] specializedPrograms;
		}
		specializedProgramsCount = info->passCount;
		specializedPrograms = new SpecializedShaderProgram*[specializedProgramsCount];
		for (uint i = 0; i < specializedProgramsCount; i++) {
			specializedPrograms[i] = new SpecializedShaderProgram(this, defaultPipeline->getRenderPass(info->passNames[i]));
			specializedPrograms[i]->load((reader->vsSources == nullptr) ? nullptr : reader->vsSources[i].source, (reader->tcsSources == nullptr) ? nullptr : reader->tcsSources[i].source, (reader->tesSources == nullptr) ? nullptr : reader->tesSources[i].source, (reader->gsSources == nullptr) ? nullptr : reader->gsSources[i].source, (reader->fsSources == nullptr) ? nullptr : reader->fsSources[i].source);
		}
	} else {
		info = reader->programInfo;
		for (uint i = 0; i < specializedProgramsCount; i++) {
			specializedPrograms[i]->reload((reader->vsSources == nullptr) ? nullptr : reader->vsSources[i].source, (reader->tcsSources == nullptr) ? nullptr : reader->tcsSources[i].source, (reader->tesSources == nullptr) ? nullptr : reader->tesSources[i].source, (reader->gsSources == nullptr) ? nullptr : reader->gsSources[i].source, (reader->fsSources == nullptr) ? nullptr : reader->fsSources[i].source);
		}
	}
	
	delete reader;
}

void ShaderProgram::unload() {
	if (!loaded) return;
	for (uint i = 0; i < specializedProgramsCount; i++) {
		specializedPrograms[i]->unload();
	}
	loaded = false;
}

ShaderPreprocessor* ShaderProgram::readShaders() {
	ShaderPreprocessor* reader = (useRaw) ? new ShaderPreprocessor(rawVS, rawFS) : new ShaderPreprocessor(fs::path(SHADER_DIRECTORY) / name);

	if (!reader->read()) {
		Debug::log("ShaderProgram", "Failed to Read shader files!");
		delete reader;
		return nullptr;
	}
	if (!reader->generate()) {
		Debug::log("ShaderProgram", "Failed to generate specialized shader sources!");
		delete reader;
		return nullptr;
	}
	return reader;
}

SpecializedShaderProgram* ShaderProgram::getSpecializedProgram(std::string renderPassName) {
	RenderPass* renderPass = defaultPipeline->getRenderPass(renderPassName);
	return getSpecializedProgram(renderPass);
}

SpecializedShaderProgram* ShaderProgram::getSpecializedProgram(RenderPass* renderPass) {
	for (uint i = 0; i < specializedProgramsCount; i++) {
		if (specializedPrograms[i]->getRenderPass() == renderPass) {
			return specializedPrograms[i];
		}
	}
	return nullptr;
}
