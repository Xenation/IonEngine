#pragma once
#include <string>
#include "XTypes.h"
#include "ShaderMetaInfo.h"

class SpecializedShaderProgram;
class RenderPass;
class Pipeline;
class ShaderPreprocessor;

class ShaderProgram {
public:
	static ShaderProgram** shaders;
	static uint shaderCount;

	static void initializeAll(Pipeline* pipeline);
	static void reloadAll();
	static ShaderProgram* find(std::string name);
	static ShaderProgram* createFromRaw(std::string name, std::string vs, std::string fs);
	static void guiAll();

	const std::string name;
	ShaderProgramMetaInfo* info = nullptr;

	ShaderProgram(std::string name);
	ShaderProgram(const ShaderProgram&) = delete;
	~ShaderProgram();

	void gui();

	void load();
	void reload();
	void unload();

	SpecializedShaderProgram* getSpecializedProgram(std::string renderPassName);
	SpecializedShaderProgram* getSpecializedProgram(RenderPass* renderPass);

private:
	static Pipeline* defaultPipeline;

	SpecializedShaderProgram** specializedPrograms = nullptr;
	uint specializedProgramsCount = 0;
	bool loaded = false;
	// Used when source needs to be in program
	bool useRaw = false;
	std::string rawVS;
	std::string rawFS;

	ShaderPreprocessor* readShaders();
};

