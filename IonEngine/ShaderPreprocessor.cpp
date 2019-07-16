#include "ShaderPreprocessor.h"

#include <fstream>
#include <regex>
#include "Debug.h"
#include "SimpleList.h"

#define REGEX_GLSL_COMMENTS R"((?://.*$|/\*.*\*/))"
#define REGEX_GLSL_TYPES "(bool|int|uint|float|double|bvec2|ivec2|uvec2|vec2|dvec2|bvec3|ivec3|uvec3|vec3|dvec3|bvec4|ivec4|uvec4|vec4|dvec4|mat2x2|mat2x3|mat2x4|mat3x2|mat3x3|mat3x4|mat4x2|mat4x3|mat4x4|mat2|mat3|mat4|dmat2x2|dmat2x3|dmat2x4|dmat3x2|dmat3x3|dmat3x4|dmat4x2|dmat4x3|dmat4x4|dmat2|dmat3|dmat4|sampler1D|sampler2D|sampler3D|samplerCube|sampler2DRect|sampler1DArray|sampler2DArray|samplerCubeArray|samplerBuffer|sampler2DMS|sampler2DMSArray|sampler1DShadow|sampler2DShadow|samplerCubeShadow|sampler2DRectShadow|sampler1DArrayShadow|sampler2DArrayShadow|samplerCubeArrayShadow|isampler1D|isampler2D|isampler3D|isamplerCube|isampler2DRect|isampler1DArray|isampler2DArray|isamplerCubeArray|isamplerBuffer|isampler2DMS|isampler2DMSArray|usampler1D|usampler2D|usampler3D|usamplerCube|usampler2DRect|usampler1DArray|usampler2DArray|usamplerCubeArray|usamplerBuffer|usampler2DMS|usampler2DMSArray|image1D|image2D|image3D|imageCube|image2DRect|image1DArray|image2DArray|imageCubeArray|imageBuffer|image2DMS|image2DMSArray|iimage1D|iimage2D|iimage3D|iimageCube|iimage2DRect|iimage1DArray|iimage2DArray|iimageCubeArray|iimageBuffer|iimage2DMS|iimage2DMSArray|uimage1D|uimage2D|uimage3D|uimageCube|uimage2DRect|uimage1DArray|uimage2DArray|uimageCubeArray|uimageBuffer|uimage2DMS|uimage2DMSArray|atomic_uint)"
#define REGEX_GLSL_TYPES_TEST "(?:bool|int|uint|float|double|bvec2|ivec2|uvec2|vec2|dvec2|bvec3|ivec3|uvec3|vec3|dvec3|bvec4|ivec4|uvec4|vec4|dvec4|mat2x2|mat2x3|mat2x4|mat3x2|mat3x3|mat3x4|mat4x2|mat4x3|mat4x4|mat2|mat3|mat4|dmat2x2|dmat2x3|dmat2x4|dmat3x2|dmat3x3|dmat3x4|dmat4x2|dmat4x3|dmat4x4|dmat2|dmat3|dmat4|sampler1D|sampler2D|sampler3D|samplerCube|sampler2DRect|sampler1DArray|sampler2DArray|samplerCubeArray|samplerBuffer|sampler2DMS|sampler2DMSArray|sampler1DShadow|sampler2DShadow|samplerCubeShadow|sampler2DRectShadow|sampler1DArrayShadow|sampler2DArrayShadow|samplerCubeArrayShadow|isampler1D|isampler2D|isampler3D|isamplerCube|isampler2DRect|isampler1DArray|isampler2DArray|isamplerCubeArray|isamplerBuffer|isampler2DMS|isampler2DMSArray|usampler1D|usampler2D|usampler3D|usamplerCube|usampler2DRect|usampler1DArray|usampler2DArray|usamplerCubeArray|usamplerBuffer|usampler2DMS|usampler2DMSArray|image1D|image2D|image3D|imageCube|image2DRect|image1DArray|image2DArray|imageCubeArray|imageBuffer|image2DMS|image2DMSArray|iimage1D|iimage2D|iimage3D|iimageCube|iimage2DRect|iimage1DArray|iimage2DArray|iimageCubeArray|iimageBuffer|iimage2DMS|iimage2DMSArray|uimage1D|uimage2D|uimage3D|uimageCube|uimage2DRect|uimage1DArray|uimage2DArray|uimageCubeArray|uimageBuffer|uimage2DMS|uimage2DMSArray|atomic_uint)"
#define REGEX_GLSL_VERSION R"(^[ \t]*#version[ \t]+(.*)$)"
#define REGEX_GLSL_META_PASSES R"(^[ \t]*#pragma[ \t]+(?:pass|passes)[ \t]+(.*)$)"
#define REGEX_GLSL_FIELD_IN R"([ \t]*in[ \t]+)" REGEX_GLSL_TYPES R"([ \t]+(\w+);)" // To be used inside the result of FIELD_IN_LINE regex
#define REGEX_GLSL_FIELD_IN_LINE R"(^(?:[ \t]*in[ \t]+)" REGEX_GLSL_TYPES_TEST R"([ \t]+\w+;)+$)"
#define REGEX_GLSL_FIELD_IN_LAYOUT R"([ \t]*layout[ \t]*\([ \n]*location[ \t]*=[ \t]*([0-9]+)[ \n]*\)[ \t]*in[ \t]+)" REGEX_GLSL_TYPES R"([ \t]+(\w+);)"
#define REGEX_GLSL_FIELD_OUT R"([ \t]*out[ \t]+)" REGEX_GLSL_TYPES R"([ \t]+(\w+);)"
#define REGEX_GLSL_FIELD_UNIFORM R"([ \t]*uniform[ \t]+)" REGEX_GLSL_TYPES R"([ \t]+(\w+);)"
#define REGEX_GLSL_FIELD_UNIFORM_LAYOUT R"([ \t]*layout[ \t]*\([ \t]*binding[ \t]*=[ \t]([0-9]+)[ \t]*\)[ \t]*uniform[ \t]*)" REGEX_GLSL_TYPES R"([ \t]*(\w+);)"
#define REGEX_GLSL_FIELD_UNIFORM_BUFFER_LAYOUT R"([ \t]*layout[ \t]*\([ \t]*(std140|packed|shared)[ \t]*,[ \t]*binding[ \t]*=[ \t]*([0-9]+)[ \t]*\)[ \t]*uniform[ \t]+(\w+)[ \t\n]*\{[\n]*([^}]*)\};)"
#define REGEX_GLSL_FIELD_NATIVE_TYPE R"([ \t]*)" REGEX_GLSL_TYPES R"([ \t]+(\w+);)"



namespace fsys = std::filesystem;

ShaderPreprocessor::ShaderPreprocessor(fsys::path shaderDir) : shaderDirectoryPath(shaderDir), programInfo(new ShaderProgramMetaInfo()) {}

ShaderPreprocessor::ShaderPreprocessor(std::string rawVS, std::string rawFS) : useRaw(true), rawVS(rawVS), rawFS(rawFS), programInfo(new ShaderProgramMetaInfo()) {}

ShaderPreprocessor::~ShaderPreprocessor() {
	if (vs != nullptr) {
		delete vs;
	}
	if (vsSources != nullptr) {
		delete[] vsSources;
	}
	if (tcs != nullptr) {
		delete tcs;
	}
	if (tcsSources != nullptr) {
		delete[] tcsSources;
	}
	if (tes != nullptr) {
		delete tes;
	}
	if (tesSources != nullptr) {
		delete[] tesSources;
	}
	if (gs != nullptr) {
		delete gs;
	}
	if (gsSources != nullptr) {
		delete[] gsSources;
	}
	if (fs != nullptr) {
		delete fs;
	}
	if (fsSources != nullptr) {
		delete[] fsSources;
	}
}


bool ShaderPreprocessor::read() {
	// When using raw source create VS and FS only
	if (useRaw) {
		vs = createShaderFileInfo(fsys::path(), GL_VERTEX_SHADER, rawVS);
		extractMetaInfo(vs);
		fs = createShaderFileInfo(fsys::path(), GL_FRAGMENT_SHADER, rawFS);
		extractMetaInfo(fs);

		mergeMetaInfo();
		return true;
	}

	// VERTEX SHADER
	fsys::path pathVS = shaderDirectoryPath / fsys::path("vs.glsl");
	if (fsys::exists(pathVS)) {
		vs = createShaderFileInfo(pathVS, GL_VERTEX_SHADER, readRawSource(pathVS));
		extractMetaInfo(vs);
	} else {
		Debug::logError("ShaderReader", "No Vertex shader found at " + shaderDirectoryPath.string());
		return false;
	}

	// TESSELATION CONTROL SHADER
	fsys::path pathTCS = shaderDirectoryPath / fsys::path("tcs.glsl");
	if (fsys::exists(pathTCS)) {
		tcs = createShaderFileInfo(pathTCS, GL_TESS_CONTROL_SHADER, readRawSource(pathTCS));
		extractMetaInfo(tcs);
	}

	// TESSELATION EVALUATION SHADER
	fsys::path pathTES = shaderDirectoryPath / fsys::path("tes.glsl");
	if (fsys::exists(pathTES)) {
		tes = createShaderFileInfo(pathTES, GL_TESS_EVALUATION_SHADER, readRawSource(pathTES));
		extractMetaInfo(tes);
	}

	// GEOMETRY SHADER
	fsys::path pathGS = shaderDirectoryPath / fsys::path("gs.glsl");
	if (fsys::exists(pathGS)) {
		gs = createShaderFileInfo(pathGS, GL_GEOMETRY_SHADER, readRawSource(pathGS));
		extractMetaInfo(gs);
	}

	// FRAGMENT SHADER
	fsys::path pathFS = shaderDirectoryPath / fsys::path("fs.glsl");
	if (fsys::exists(pathFS)) {
		fs = createShaderFileInfo(pathFS, GL_FRAGMENT_SHADER, readRawSource(pathFS));
		extractMetaInfo(fs);
	} else {
		Debug::logError("ShaderReader", "No Fragment shader found at " + shaderDirectoryPath.string());
		return false;
	}

	mergeMetaInfo();

	return true;
}

bool ShaderPreprocessor::generate() {
	if (vs != nullptr) {
		generateSpecializedSources(vs, vsSources);
	} else {
		return false;
	}
	if (tcs != nullptr) {
		generateSpecializedSources(tcs, tcsSources);
	}
	if (tes != nullptr) {
		generateSpecializedSources(tes, tesSources);
	}
	if (gs != nullptr) {
		generateSpecializedSources(gs, gsSources);
	}
	if (fs != nullptr) {
		generateSpecializedSources(fs, fsSources);
	} else {
		return false;
	}

	return true;
}

std::string ShaderPreprocessor::readRawSource(fsys::path filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		Debug::logError("ShaderReader", "Could not open file \"" + filePath.string() + "\"!");
		return nullptr;
	}
	
	file.seekg(0, file.end);
	uint sourceSize = (uint) file.tellg();
	char* source = new char[sourceSize + 1];
	file.seekg(0, file.beg);
	file.read(source, sourceSize);
	uint charCount = (uint) file.gcount();
	source[charCount] = '\0';
	file.close();

	std::string sourceString(source);
	delete[] source;

	return sourceString;
}

ShaderPreprocessor::ShaderFile* ShaderPreprocessor::createShaderFileInfo(fsys::path filePath, GLenum type, std::string rawSource) {
	std::regex versionRegex(REGEX_GLSL_VERSION);

	std::smatch versionMatch;
	if (!std::regex_search(rawSource, versionMatch, versionRegex)) {
		Debug::logError("ShaderReader", "Could not find glsl version in shader \"" + filePath.string() + "\"!");
		return nullptr;
	}

	ShaderFile* shaderFile = new ShaderFile();
	shaderFile->filePath = filePath;
	shaderFile->shaderType = type;
	shaderFile->version = versionMatch.str();
	shaderFile->rawSource = versionMatch.suffix().str();

	return shaderFile;
}

void ShaderPreprocessor::removeComments(ShaderFile* shaderFile) {
	std::regex commentsRegex(REGEX_GLSL_COMMENTS);
	std::smatch commentsMatch;
	std::string currentStr(shaderFile->rawSource);

	SimpleList<uint> startIndices(8, 8);
	SimpleList<uint> lengths(8, 8);

	uint positionOffset = 0;
	while (std::regex_search(currentStr, commentsMatch, commentsRegex)) {
		positionOffset += (uint) commentsMatch.position(0);
		startIndices.add(positionOffset);
		lengths.add((uint) commentsMatch.length(0));
		currentStr = commentsMatch.suffix();
	}

	// Source erase
	for (uint i = 0; i < startIndices.count; i++) {
		shaderFile->rawSource.erase(startIndices[i], lengths[i]);
	}
}

void ShaderPreprocessor::extractMetaInfo(ShaderFile* shaderFile) {
	// Remove Comments in source to avoid false positives
	removeComments(shaderFile);

	// Detect Passes (should only be present in vertex shader)
	std::regex passesRegex(REGEX_GLSL_META_PASSES);
	std::smatch passesMatch;
	if (std::regex_search(shaderFile->rawSource, passesMatch, passesRegex)) {
		std::regex passRegex("\\w+");
		std::string passesString = passesMatch[1].str();
		std::smatch passMatch;
		// count
		uint passCount = 0;
		while (std::regex_search(passesString, passMatch, passRegex)) {
			passCount++;
			passesString = passMatch.suffix();
		}
		if (programInfo->passNames != nullptr) {
			delete[] programInfo->passNames;
		}
		programInfo->passNames = new std::string[passCount];
		programInfo->passCount = passCount;
		// store
		passesString = std::string(passesMatch[1].str());
		uint index = 0;
		while (std::regex_search(passesString, passMatch, passRegex)) {
			programInfo->passNames[index] = passMatch[0].str();
			index++;
			passesString = passMatch.suffix();
		}
	}

	std::string extractedSource(shaderFile->rawSource);
	std::regex inLayoutRegex(REGEX_GLSL_FIELD_IN_LAYOUT);
	SimpleList<std::smatch> inLayoutMatches(4, 4);
	std::regex inRegex(REGEX_GLSL_FIELD_IN);
	SimpleList<std::smatch> inMatches(4, 4);
	std::regex outRegex(REGEX_GLSL_FIELD_OUT);
	SimpleList<std::smatch> outMatches(4, 4);
	std::regex uniformLayoutRegex(REGEX_GLSL_FIELD_UNIFORM_LAYOUT);
	SimpleList<std::smatch> uniformLayoutMatches(4, 4);
	std::regex uniformBufferLayoutRegex(REGEX_GLSL_FIELD_UNIFORM_BUFFER_LAYOUT);
	SimpleList<std::smatch> uniformBufferLayoutMatches(4, 4);
	std::regex uniformRegex(REGEX_GLSL_FIELD_UNIFORM);
	SimpleList<std::smatch> uniformMatches(4, 4);

	SimpleList<ShaderFieldInfo*> fields(8, 8);

	findRegexMatches(extractedSource, inLayoutRegex, inLayoutMatches);
	for (uint i = 0; i < inLayoutMatches.count; i++) {
		std::smatch& inLayoutMatch = inLayoutMatches[i];
		fields.add(new ShaderInLayoutFieldInfo(ShaderFieldType::InLayout, inLayoutMatch[3], glslTypeFromString(inLayoutMatch[2]), std::stoi(inLayoutMatch[1])));
	}
	removeMatches(extractedSource, inLayoutMatches);

	findRegexMatches(extractedSource, inRegex, inMatches);
	for (uint i = 0; i < inMatches.count; i++) {
		std::smatch& inMatch = inMatches[i];
		fields.add(new ShaderNativeTypeFieldInfo(ShaderFieldType::In, inMatch[2], glslTypeFromString(inMatch[1])));
	}
	removeMatches(extractedSource, inMatches);

	findRegexMatches(extractedSource, outRegex, outMatches);
	for (uint i = 0; i < outMatches.count; i++) {
		std::smatch& outMatch = outMatches[i];
		fields.add(new ShaderNativeTypeFieldInfo(ShaderFieldType::Out, outMatch[2], glslTypeFromString(outMatch[1])));
	}
	removeMatches(extractedSource, outMatches);

	findRegexMatches(extractedSource, uniformLayoutRegex, uniformLayoutMatches);
	for (uint i = 0; i < uniformLayoutMatches.count; i++) {
		std::smatch& uniformLayoutMatch = uniformLayoutMatches[i];
		fields.add(new ShaderUniformLayoutFieldInfo(ShaderFieldType::UniformLayout, uniformLayoutMatch[3], glslTypeFromString(uniformLayoutMatch[2]), std::stoi(uniformLayoutMatch[1])));
	}
	removeMatches(extractedSource, uniformLayoutMatches);

	findRegexMatches(extractedSource, uniformBufferLayoutRegex, uniformBufferLayoutMatches);
	for (uint i = 0; i < uniformBufferLayoutMatches.count; i++) {
		std::smatch& uniformBufferLayoutMatch = uniformBufferLayoutMatches[i];
		ShaderUniformBufferLayoutFieldInfo* uniformBufferLayoutInfo = new ShaderUniformBufferLayoutFieldInfo(ShaderFieldType::UniformBufferLayout, uniformBufferLayoutMatch[3], glslUniformLayoutTypeFromString(uniformBufferLayoutMatch[1]), std::stoi(uniformBufferLayoutMatch[2]));

		std::regex nativeTypeRegex(REGEX_GLSL_FIELD_NATIVE_TYPE);
		std::string contents = uniformBufferLayoutMatch[4];
		// Counting pass
		std::smatch uniformLayoutMemberMatch;
		std::string currentContents = contents;
		while (std::regex_search(currentContents, uniformLayoutMemberMatch, nativeTypeRegex)) {
			uniformBufferLayoutInfo->subFieldCount++;
			currentContents = uniformLayoutMemberMatch.suffix();
		}
		uniformBufferLayoutInfo->subFields = new ShaderFieldInfo*[uniformBufferLayoutInfo->subFieldCount];
		// Storing pass
		uint subFieldIndex = 0;
		currentContents = contents;
		while (std::regex_search(currentContents, uniformLayoutMemberMatch, nativeTypeRegex)) {
			uniformBufferLayoutInfo->subFields[subFieldIndex++] = new ShaderNativeTypeFieldInfo(ShaderFieldType::NativeType, uniformLayoutMemberMatch[2], glslTypeFromString(uniformLayoutMemberMatch[1]));
			currentContents = uniformLayoutMemberMatch.suffix();
		}
		fields.add(uniformBufferLayoutInfo);
	}
	removeMatches(extractedSource, uniformBufferLayoutMatches);

	findRegexMatches(extractedSource, uniformRegex, uniformMatches);
	for (uint i = 0; i < uniformMatches.count; i++) {
		std::smatch& uniformMatch = uniformMatches[i];
		fields.add(new ShaderNativeTypeFieldInfo(ShaderFieldType::Uniform, uniformMatch[2], glslTypeFromString(uniformMatch[1])));
	}
	removeMatches(extractedSource, uniformMatches);

	shaderFile->info.shaderFieldCount = fields.count;
	shaderFile->info.shaderFields = new ShaderFieldInfo*[shaderFile->info.shaderFieldCount];
	for (uint i = 0; i < fields.count; i++) {
		shaderFile->info.shaderFields[i] = fields[i];
	}

	//// Detect fields
	//// Counting pass
	//uint fieldCount = 0;
	//std::string currentString = shaderFile->rawSource;

	//std::regex inLineRegex(REGEX_GLSL_FIELD_IN_LINE);
	//std::regex inRegex(REGEX_GLSL_FIELD_IN);
	//std::smatch inLineMatch;
	//while (std::regex_search(currentString, inLineMatch, inLineRegex)) {
	//	std::smatch inMatch;
	//	std::string inLine = inLineMatch.str();
	//	while (std::regex_search(inLine, inMatch, inRegex)) {
	//		fieldCount++;
	//		inLine = inMatch.suffix();
	//	}
	//	currentString = inLineMatch.suffix();
	//}
	//std::regex inLayoutRegex(REGEX_GLSL_FIELD_IN_LAYOUT);
	//std::smatch inLayoutMatch;
	//currentString = shaderFile->rawSource;
	//while (std::regex_search(currentString, inLayoutMatch, inLayoutRegex)) {
	//	currentString = inLayoutMatch.suffix();
	//	fieldCount++;
	//}
	//std::regex outRegex(REGEX_GLSL_FIELD_OUT);
	//std::smatch outMatch;
	//currentString = shaderFile->rawSource;
	//while (std::regex_search(currentString, outMatch, outRegex)) {
	//	fieldCount++;
	//	currentString = outMatch.suffix();
	//}
	//std::regex uniformRegex(REGEX_GLSL_FIELD_UNIFORM);
	//std::smatch uniformMatch;
	//currentString = shaderFile->rawSource;
	//while (std::regex_search(currentString, uniformMatch, uniformRegex)) {
	//	fieldCount++;
	//	currentString = uniformMatch.suffix();
	//}
	//std::regex uniformLayoutRegex(REGEX_GLSL_FIELD_UNIFORM_BUFFER_LAYOUT);
	//std::smatch uniformLayoutMatch;
	//currentString = shaderFile->rawSource;
	//while (std::regex_search(currentString, uniformLayoutMatch, uniformLayoutRegex)) {
	//	fieldCount++;
	//	currentString = uniformLayoutMatch.suffix();
	//}
	//shaderFile->info.shaderFieldCount = fieldCount;
	//shaderFile->info.shaderFields = new ShaderFieldInfo*[fieldCount];

	//// Storing pass
	//uint fieldIndex = 0;
	//currentString = shaderFile->rawSource;
	//while (std::regex_search(currentString, inLineMatch, inLineRegex)) {
	//	std::smatch inMatch;
	//	std::string inLine = inLineMatch.str();
	//	while (std::regex_search(inLine, inMatch, inRegex)) {
	//		shaderFile->info.shaderFields[fieldIndex++] = new ShaderNativeTypeFieldInfo(ShaderFieldType::In, inMatch[2], glslTypeFromString(inMatch[1]));
	//		inLine = inMatch.suffix();
	//	}
	//	currentString = inLineMatch.suffix();
	//}
	//currentString = shaderFile->rawSource;
	//while (std::regex_search(currentString, inLayoutMatch, inLayoutRegex)) {
	//	shaderFile->info.shaderFields[fieldIndex++] = new ShaderInLayoutFieldInfo(ShaderFieldType::InLayout, inLayoutMatch[3], glslTypeFromString(inLayoutMatch[2]), std::stoi(inLayoutMatch[1]));
	//	currentString = inLayoutMatch.suffix();
	//}
	//currentString = shaderFile->rawSource;
	//while (std::regex_search(currentString, outMatch, outRegex)) {
	//	shaderFile->info.shaderFields[fieldIndex++] = new ShaderNativeTypeFieldInfo(ShaderFieldType::Out, outMatch[2], glslTypeFromString(outMatch[1]));
	//	currentString = outMatch.suffix();
	//}
	//currentString = shaderFile->rawSource;
	//while (std::regex_search(currentString, uniformMatch, uniformRegex)) {
	//	shaderFile->info.shaderFields[fieldIndex++] = new ShaderNativeTypeFieldInfo(ShaderFieldType::Uniform, uniformMatch[2], glslTypeFromString(uniformMatch[1]));
	//	currentString = uniformMatch.suffix();
	//}
	//currentString = shaderFile->rawSource;
	//while (std::regex_search(currentString, uniformLayoutMatch, uniformLayoutRegex)) {
	//	std::regex nativeTypeRegex(REGEX_GLSL_FIELD_NATIVE_TYPE);
	//	std::string contents = uniformLayoutMatch[4];
	//	ShaderUniformBufferLayoutFieldInfo* uniformLayoutInfo = new ShaderUniformBufferLayoutFieldInfo(ShaderFieldType::UniformLayout, uniformLayoutMatch[3], glslUniformLayoutTypeFromString(uniformLayoutMatch[1]), std::stoi(uniformLayoutMatch[2]));
	//	// Counting pass
	//	std::smatch uniformLayoutMemberMatch;
	//	std::string currentContents = contents;
	//	while (std::regex_search(currentContents, uniformLayoutMemberMatch, nativeTypeRegex)) {
	//		uniformLayoutInfo->subFieldCount++;
	//		currentContents = uniformLayoutMemberMatch.suffix();
	//	}
	//	uniformLayoutInfo->subFields = new ShaderFieldInfo*[uniformLayoutInfo->subFieldCount];
	//	// Storing pass
	//	uint subFieldIndex = 0;
	//	currentContents = contents;
	//	while (std::regex_search(currentContents, uniformLayoutMemberMatch, nativeTypeRegex)) {
	//		uniformLayoutInfo->subFields[subFieldIndex++] = new ShaderNativeTypeFieldInfo(ShaderFieldType::NativeType, uniformLayoutMemberMatch[2], glslTypeFromString(uniformLayoutMemberMatch[1]));
	//		currentContents = uniformLayoutMemberMatch.suffix();
	//	}
	//	shaderFile->info.shaderFields[fieldIndex++] = uniformLayoutInfo;
	//	currentString = uniformLayoutMatch.suffix();
	//}

}

void ShaderPreprocessor::findRegexMatches(std::string& str, std::regex& reg, SimpleList<std::smatch>& matches) {
	std::smatch match;

	uint positionOffset = 0;

	std::string::const_iterator start = str.cbegin();
	std::string::const_iterator end = str.cend();

	while (std::regex_search(start, end, match, reg)) {
		matches.add(match);
		uint position = match.position(0);
		uint length = match.length(0);
		start += position + length;
	}
}

void ShaderPreprocessor::removeMatches(std::string& str, SimpleList<std::smatch>& matches) {
	for (int i = matches.count - 1; i >= 0; i--) { // reverse iteration to avoid offsetting the iterators of the next matches
		str.erase(matches[i][0].first, matches[i][0].second);
		//str.erase(matches[i].position(0), matches[i].length(0));
	}
}

void ShaderPreprocessor::mergeMetaInfo() {
	// Merge in list
	SimpleList<ShaderFieldInfo*> programFields(8, 16);
	if (vs != nullptr) {
		for (uint i = 0; i < vs->info.shaderFieldCount; i++) {
			ShaderFieldInfo* fieldInfo = vs->info.shaderFields[i];
			if (fieldInfo->fieldType == ShaderFieldType::In || fieldInfo->fieldType == ShaderFieldType::Out) continue;
			programFields.add(fieldInfo->copy());
		}
	}
	if (tcs != nullptr) {
		for (uint i = 0; i < tcs->info.shaderFieldCount; i++) {
			ShaderFieldInfo* fieldInfo = tcs->info.shaderFields[i];
			if (fieldInfo->fieldType == ShaderFieldType::In || fieldInfo->fieldType == ShaderFieldType::Out) continue;
			uint pIndex = 0;
			for (; pIndex < programFields.count; pIndex++) {
				if (programFields[pIndex]->name == fieldInfo->name) break;
			}
			if (pIndex == programFields.count) { // Reached end without finding duplicates
				programFields.add(fieldInfo->copy());
			}
		}
	}
	if (tes != nullptr) {
		for (uint i = 0; i < tes->info.shaderFieldCount; i++) {
			ShaderFieldInfo* fieldInfo = tes->info.shaderFields[i];
			if (fieldInfo->fieldType == ShaderFieldType::In || fieldInfo->fieldType == ShaderFieldType::Out) continue;
			uint pIndex = 0;
			for (; pIndex < programFields.count; pIndex++) {
				if (programFields[pIndex]->name == fieldInfo->name) break;
			}
			if (pIndex == programFields.count) { // Reached end without finding duplicates
				programFields.add(fieldInfo->copy());
			}
		}
	}
	if (gs != nullptr) {
		for (uint i = 0; i < gs->info.shaderFieldCount; i++) {
			ShaderFieldInfo* fieldInfo = gs->info.shaderFields[i];
			if (fieldInfo->fieldType == ShaderFieldType::In || fieldInfo->fieldType == ShaderFieldType::Out) continue;
			uint pIndex = 0;
			for (; pIndex < programFields.count; pIndex++) {
				if (programFields[pIndex]->name == fieldInfo->name) break;
			}
			if (pIndex == programFields.count) { // Reached end without finding duplicates
				programFields.add(fieldInfo->copy());
			}
		}
	}
	if (fs != nullptr) {
		for (uint i = 0; i < fs->info.shaderFieldCount; i++) {
			ShaderFieldInfo* fieldInfo = fs->info.shaderFields[i];
			if (fieldInfo->fieldType == ShaderFieldType::In || fieldInfo->fieldType == ShaderFieldType::Out) continue;
			uint pIndex = 0;
			for (; pIndex < programFields.count; pIndex++) {
				if (programFields[pIndex]->name == fieldInfo->name) break;
			}
			if (pIndex == programFields.count) { // Reached end without finding duplicates
				programFields.add(fieldInfo->copy());
			}
		}
	}

	// Copy to array
	programInfo->programFieldCount = programFields.count;
	programInfo->programFields = new ShaderFieldInfo*[programInfo->programFieldCount];
	for (uint i = 0; i < programFields.count; i++) {
		programInfo->programFields[i] = programFields[i];
	}
}

void ShaderPreprocessor::generateSpecializedSources(ShaderFile* shaderFile, SpecializedShaderSource*& specializedSources) {
	specializedSources = new SpecializedShaderSource[programInfo->passCount];
	for (uint passIndex = 0; passIndex < programInfo->passCount; passIndex++) {
		std::string passUpper = std::string(programInfo->passNames[passIndex]);
		for (char& c : passUpper) { c = toupper((unsigned char) c); }
		std::string defines("\n#define PASS_" + passUpper + "\n");
		specializedSources[passIndex].version = new char[shaderFile->version.size() + 1];
		specializedSources[passIndex].version[shaderFile->version.size()] = '\0';
		specializedSources[passIndex].defines = new char[defines.size() + 1];
		specializedSources[passIndex].defines[defines.size()] = '\0';
		specializedSources[passIndex].noVersionSource = new char[shaderFile->rawSource.size() + 1];
		specializedSources[passIndex].noVersionSource[shaderFile->rawSource.size()] = '\0';
		shaderFile->version.copy(specializedSources[passIndex].version, shaderFile->version.size());
		defines.copy(specializedSources[passIndex].defines, defines.size());
		shaderFile->rawSource.copy(specializedSources[passIndex].noVersionSource, shaderFile->rawSource.size());
	}
}
