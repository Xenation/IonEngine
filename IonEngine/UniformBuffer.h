#pragma once
#include <string>
#include <gl3w.h>
#include "GLUtils.h"
#include "XMath.h"
#include "ShaderMetaInfo.h"

namespace IonEngine {
	class UniformBuffer;

	struct UniformLayout {
		GLuint binding = 0;
		UniformLayoutType type = UniformLayoutType::STD140;
		GLSLType* members = nullptr;
		std::string* memberNames = nullptr;
		u32* membersOffsets = nullptr;
		u32* memberArraySizes = nullptr;
		u32 memberCount = 0;
		u32 size = 0;
		u32 offset = 0;
		u8* buffer = nullptr;

	public:
		UniformLayout(u32 binding, ShaderUniformBufferLayoutFieldInfo* bufferField);
		UniformLayout(u32 binding, u32 memberCount, GLSLType* members);
		UniformLayout(u32 binding, UniformLayoutType type, u32 memberCount, GLSLType* members);
		~UniformLayout();

		void setMember(u32 index, bool value);
		void setMember(u32 index, i32 value);
		void setMember(u32 index, u32 value);
		void setMember(u32 index, float value);
		void setMember(u32 index, double value);
		void setMember(u32 index, Vec2f value);
		void setMember(u32 index, Vec3f value);
		void setMember(u32 index, Vec4f value);
		void setMember(u32 index, Vec2i value);
		void setMember(u32 index, Vec3i value);
		void setMember(u32 index, Vec4i value);
		void setMember(u32 index, Matrix4x4f value);
		void setMember(u32 index, bool value, u32 arrayIndex);
		void setMember(u32 index, i32 value, u32 arrayIndex);
		void setMember(u32 index, u32 value, u32 arrayIndex);
		void setMember(u32 index, float value, u32 arrayIndex);
		void setMember(u32 index, double value, u32 arrayIndex);
		void setMember(u32 index, Vec2f value, u32 arrayIndex);
		void setMember(u32 index, Vec3f value, u32 arrayIndex);
		void setMember(u32 index, Vec4f value, u32 arrayIndex);
		void setMember(u32 index, Vec2i value, u32 arrayIndex);
		void setMember(u32 index, Vec3i value, u32 arrayIndex);
		void setMember(u32 index, Vec4i value, u32 arrayIndex);
		void setMember(u32 index, Matrix4x4f value, u32 arrayIndex);
		void setMember(u32 index, u8* bytes, u32 byteSize);
		void setMember(u32 index, u32 arrayIndex, u8* bytes, u32 byteSize);
		void copyFrom(UniformLayout& layout);
		int indexOf(std::string memberName) const;
		inline bool getBool(u32 index) const { return *getBoolPtr(index); }
		inline i32 getInt(u32 index) const { return *getIntPtr(index); }
		inline u32 getUint(u32 index) const { return *getUintPtr(index); }
		inline float getFloat(u32 index) const { return *getFloatPtr(index); }
		inline double getDouble(u32 index) const { return *getDoublePtr(index); }
		inline Vec2f getVec2f(u32 index) const { return *getVec2fPtr(index); }
		inline Vec3f getVec3f(u32 index) const { return *getVec3fPtr(index); }
		inline Vec4f getVec4f(u32 index) const { return *getVec4fPtr(index); }
		inline Vec2i getVec2i(u32 index) const { return *getVec2iPtr(index); }
		inline Vec3i getVec3i(u32 index) const { return *getVec3iPtr(index); }
		inline Vec4i getVec4i(u32 index) const { return *getVec4iPtr(index); }
		inline Matrix4x4f getMatrix4x4f(u32 index) const { return *getMatrix4x4fPtr(index); }
		inline bool getBool(u32 index, u32 arrayIndex) const { return *getBoolPtr(index, arrayIndex); }
		inline i32 getInt(u32 index, u32 arrayIndex) const { return *getIntPtr(index, arrayIndex); }
		inline u32 getUint(u32 index, u32 arrayIndex) const { return *getUintPtr(index, arrayIndex); }
		inline float getFloat(u32 index, u32 arrayIndex) const { return *getFloatPtr(index, arrayIndex); }
		inline double getDouble(u32 index, u32 arrayIndex) const { return *getDoublePtr(index, arrayIndex); }
		inline Vec2f getVec2f(u32 index, u32 arrayIndex) const { return *getVec2fPtr(index, arrayIndex); }
		inline Vec3f getVec3f(u32 index, u32 arrayIndex) const { return *getVec3fPtr(index, arrayIndex); }
		inline Vec4f getVec4f(u32 index, u32 arrayIndex) const { return *getVec4fPtr(index, arrayIndex); }
		inline Vec2i getVec2i(u32 index, u32 arrayIndex) const { return *getVec2iPtr(index, arrayIndex); }
		inline Vec3i getVec3i(u32 index, u32 arrayIndex) const { return *getVec3iPtr(index, arrayIndex); }
		inline Vec4i getVec4i(u32 index, u32 arrayIndex) const { return *getVec4iPtr(index, arrayIndex); }
		inline Matrix4x4f getMatrix4x4f(u32 index, u32 arrayIndex) const { return *getMatrix4x4fPtr(index, arrayIndex); }
		inline bool* getBoolPtr(u32 index) const { return (bool*) getBytes(index); }
		inline i32* getIntPtr(u32 index) const { return (i32*) getBytes(index); }
		inline u32* getUintPtr(u32 index) const { return (u32*) getBytes(index); }
		inline float* getFloatPtr(u32 index) const { return (float*) getBytes(index); }
		inline double* getDoublePtr(u32 index) const { return (double*) getBytes(index); }
		inline Vec2f* getVec2fPtr(u32 index) const { return (Vec2f*) getBytes(index); }
		inline Vec3f* getVec3fPtr(u32 index) const { return (Vec3f*) getBytes(index); }
		inline Vec4f* getVec4fPtr(u32 index) const { return (Vec4f*) getBytes(index); }
		inline Vec2i* getVec2iPtr(u32 index) const { return (Vec2i*) getBytes(index); }
		inline Vec3i* getVec3iPtr(u32 index) const { return (Vec3i*) getBytes(index); }
		inline Vec4i* getVec4iPtr(u32 index) const { return (Vec4i*) getBytes(index); }
		inline Matrix4x4f* getMatrix4x4fPtr(u32 index) const { return (Matrix4x4f*) getBytes(index); }
		inline bool* getBoolPtr(u32 index, u32 arrayIndex) const { return (bool*) getBytes(index, arrayIndex); }
		inline i32* getIntPtr(u32 index, u32 arrayIndex) const { return (i32*) getBytes(index, arrayIndex); }
		inline u32* getUintPtr(u32 index, u32 arrayIndex) const { return (u32*) getBytes(index, arrayIndex); }
		inline float* getFloatPtr(u32 index, u32 arrayIndex) const { return (float*) getBytes(index, arrayIndex); }
		inline double* getDoublePtr(u32 index, u32 arrayIndex) const { return (double*) getBytes(index, arrayIndex); }
		inline Vec2f* getVec2fPtr(u32 index, u32 arrayIndex) const { return (Vec2f*) getBytes(index, arrayIndex); }
		inline Vec3f* getVec3fPtr(u32 index, u32 arrayIndex) const { return (Vec3f*) getBytes(index, arrayIndex); }
		inline Vec4f* getVec4fPtr(u32 index, u32 arrayIndex) const { return (Vec4f*) getBytes(index, arrayIndex); }
		inline Vec2i* getVec2iPtr(u32 index, u32 arrayIndex) const { return (Vec2i*) getBytes(index, arrayIndex); }
		inline Vec3i* getVec3iPtr(u32 index, u32 arrayIndex) const { return (Vec3i*) getBytes(index, arrayIndex); }
		inline Vec4i* getVec4iPtr(u32 index, u32 arrayIndex) const { return (Vec4i*) getBytes(index, arrayIndex); }
		inline Matrix4x4f* getMatrix4x4fPtr(u32 index, u32 arrayIndex) const { return (Matrix4x4f*) getBytes(index, arrayIndex); }
		u8* getBytes(u32 index) const;
		u8* getBytes(u32 index, u32 arrayIndex) const;

		void computeLayoutOffsets();
	};

	class UniformBuffer {
	public:
		UniformBuffer(std::string name);
		~UniformBuffer();

		const std::string name;

		void setLayouts(u32 layoutCount, UniformLayout* layouts);
		UniformLayout& getLayout(u32 index);
		UniformLayout* findLayout(GLuint binding);
		void updateLayout(u32 index);
		void bindLayout(u32 index);
		void uploadToGL();
		void deleteFromGL();

	private:
		GLuint ubo = 0;
		UniformLayout* layouts = nullptr;
		u32 layoutCount = 0;
		u8* buffer = nullptr;
		u32 bufferSize = 0;
		bool loadedToGL = false;
	};
}
