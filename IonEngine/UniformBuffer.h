#pragma once
#include <string>
#include <gl3w.h>
#include "GLUtils.h"
#include "XMath.h"
#include "ShaderMetaInfo.h"

class UniformBuffer;

struct UniformLayout {
	GLuint binding = 0;
	UniformLayoutType type = UniformLayoutType::STD140;
	GLSLType* members = nullptr;
	std::string* memberNames = nullptr;
	unsigned int* membersOffsets = nullptr;
	unsigned int memberCount = 0;
	unsigned int size = 0;
	unsigned int offset = 0;
	unsigned char* buffer = nullptr;

public:
	UniformLayout(unsigned int binding, ShaderUniformBufferLayoutFieldInfo* bufferField);
	UniformLayout(unsigned int binding, unsigned int memberCount, GLSLType* members);
	UniformLayout(unsigned int binding, UniformLayoutType type, unsigned int memberCount, GLSLType* members);
	~UniformLayout();

	void setMember(unsigned int index, bool value);
	void setMember(unsigned int index, int value);
	void setMember(unsigned int index, unsigned int value);
	void setMember(unsigned int index, float value);
	void setMember(unsigned int index, double value);
	void setMember(unsigned int index, Vec2f value);
	void setMember(unsigned int index, Vec3f value);
	void setMember(unsigned int index, Vec4f value);
	void setMember(unsigned int index, Vec2i value);
	void setMember(unsigned int index, Vec3i value);
	void setMember(unsigned int index, Vec4i value);
	void setMember(unsigned int index, Matrix4x4f value);
	void setMember(unsigned int index, unsigned char* bytes, unsigned int byteSize);
	void copyFrom(UniformLayout& layout);
	int indexOf(std::string memberName) const;
	inline bool getBool(unsigned int index) const { return *getBoolPtr(index); }
	inline int getInt(unsigned int index) const { return *getIntPtr(index); }
	inline unsigned int getUint(unsigned int index) const { return *getUintPtr(index); }
	inline float getFloat(unsigned int index) const { return *getFloatPtr(index); }
	inline double getDouble(unsigned int index) const { return *getDoublePtr(index); }
	inline Vec2f getVec2f(unsigned int index) const { return *getVec2fPtr(index); }
	inline Vec3f getVec3f(unsigned int index) const { return *getVec3fPtr(index); }
	inline Vec4f getVec4f(unsigned int index) const { return *getVec4fPtr(index); }
	inline Vec2i getVec2i(unsigned int index) const { return *getVec2iPtr(index); }
	inline Vec3i getVec3i(unsigned int index) const { return *getVec3iPtr(index); }
	inline Vec4i getVec4i(unsigned int index) const { return *getVec4iPtr(index); }
	inline Matrix4x4f getMatrix4x4f(unsigned int index) const { return *getMatrix4x4fPtr(index); }
	inline bool* getBoolPtr(unsigned int index) const { return (bool*) getBytes(index); }
	inline int* getIntPtr(unsigned int index) const { return (int*) getBytes(index); }
	inline unsigned int* getUintPtr(unsigned int index) const { return (unsigned int*) getBytes(index); }
	inline float* getFloatPtr(unsigned int index) const { return (float*) getBytes(index); }
	inline double* getDoublePtr(unsigned int index) const { return (double*) getBytes(index); }
	inline Vec2f* getVec2fPtr(unsigned int index) const { return (Vec2f*) getBytes(index); }
	inline Vec3f* getVec3fPtr(unsigned int index) const { return (Vec3f*) getBytes(index); }
	inline Vec4f* getVec4fPtr(unsigned int index) const { return (Vec4f*) getBytes(index); }
	inline Vec2i* getVec2iPtr(unsigned int index) const { return (Vec2i*) getBytes(index); }
	inline Vec3i* getVec3iPtr(unsigned int index) const { return (Vec3i*) getBytes(index); }
	inline Vec4i* getVec4iPtr(unsigned int index) const { return (Vec4i*) getBytes(index); }
	inline Matrix4x4f* getMatrix4x4fPtr(unsigned int index) const { return (Matrix4x4f*) getBytes(index); }
	unsigned char* getBytes(unsigned int index) const;

	void computeLayoutOffsets();
};

class UniformBuffer {
public:
	UniformBuffer(std::string name);
	~UniformBuffer();

	const std::string name;

	void setLayouts(unsigned int layoutCount, UniformLayout* layouts);
	UniformLayout& getLayout(unsigned int index);
	UniformLayout* findLayout(GLuint binding);
	void updateLayout(unsigned int index);
	void bindLayout(unsigned int index);
	void uploadToGL();
	void deleteFromGL();

private:
	GLuint ubo = 0;
	UniformLayout* layouts = nullptr;
	unsigned int layoutCount = 0;
	unsigned char* buffer = nullptr;
	unsigned int bufferSize = 0;
	bool loadedToGL = false;
};

