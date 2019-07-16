#pragma once
#include <string>
#include <gl3w.h>
#include "XTypes.h"
#include "XMath.h"
#include "HollowSet.h"

class ShaderProgram;
class RenderPass;
class Pipeline;
class Material;

class SpecializedShaderProgram {
public:
	static SpecializedShaderProgram* errorShader;

	static void initialize(Pipeline* pipeline);


	ShaderProgram*const parentShader;
	HollowSet<Material*> materials;

	SpecializedShaderProgram(ShaderProgram*const parentShader, RenderPass* renderPass);
	SpecializedShaderProgram(const SpecializedShaderProgram&) = delete;
	~SpecializedShaderProgram();

	void gui();

	void load(const char* vsSource[3], const char* tcsSource[3], const char* tesSource[3], const char* gsSource[3], const char* fsSource[3]);
	void reload(const char* vsSource[3], const char* tcsSource[3], const char* tesSource[3], const char* gsSource[3], const char* fsSource[3]);
	void unload();
	void use();
	void unuse();

	GLuint getUniformLocation(const char* str);
	GLuint getUniformLocation(std::string str);
	void loadBool(GLuint location, bool b);
	void loadInt(GLuint location, int i);
	void loadUInt(GLuint location, uint u);
	void loadFloat(GLuint location, float f);
	void loadDouble(GLuint location, double d);
	void loadVec2f(GLuint location, Vec2f vec);
	void loadVec3f(GLuint location, Vec3f vec);
	void loadVec4f(GLuint location, Vec4f vec);
	void loadMatrix4x4f(GLuint location, Matrix4x4f matrix);

	inline RenderPass* getRenderPass() { return renderPass; }

private:
	RenderPass* renderPass;
	bool loaded = false;
	GLuint program = 0;
	GLuint vertex = 0;
	GLuint tessControl = 0;
	GLuint tessEvaluation = 0;
	GLuint geometry = 0;
	GLuint fragment = 0;

	uint idInPass = 0;

	void load(GLuint vs, GLuint tcs, GLuint tes, GLuint gs, GLuint fs, bool silent = false);
	GLuint loadShaderFromSource(GLenum type, const char* src, bool silent = false);
	GLuint loadShaderFromSourceArray(GLenum type, const char** src, uint srcPiecesCount, bool silent = false);

};

