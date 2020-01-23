#pragma once
#include <string>
#include "GLUtils.h"


namespace IonEngine {
	// ---- Field Info ----
	enum class ShaderFieldType {
		In,
		InLayout,
		Out,
		Uniform,
		UniformLayout,
		UniformBufferLayout,
		NativeType,
		NativeTypeArray,
	};

	struct ShaderFieldInfo {
		ShaderFieldType fieldType;
		std::string name;

	public:
		ShaderFieldInfo() : fieldType(ShaderFieldType::In) {}
		ShaderFieldInfo(ShaderFieldType fieldType, std::string name) : fieldType(fieldType), name(name) {}

		inline virtual ShaderFieldInfo* copy() {
			return new ShaderFieldInfo(fieldType, std::string(name));
		};
	};

	struct ShaderNativeTypeFieldInfo : public ShaderFieldInfo {
		GLSLType type = GLSL_UNKNOWN;

	public:
		ShaderNativeTypeFieldInfo(ShaderFieldType fieldType, std::string name, GLSLType type) : ShaderFieldInfo(fieldType, name), type(type) {}

		inline virtual ShaderFieldInfo* copy() override {
			return new ShaderNativeTypeFieldInfo(fieldType, std::string(name), type);
		}
	};

	struct ShaderNativeTypeArrayFieldInfo : public ShaderNativeTypeFieldInfo {
		unsigned int arraySize = 0;

	public:
		ShaderNativeTypeArrayFieldInfo(ShaderFieldType fieldType, std::string name, GLSLType type, u32 arraySize) : ShaderNativeTypeFieldInfo(fieldType, name, type), arraySize(arraySize) {}

		inline virtual ShaderFieldInfo* copy() override {
			return new ShaderNativeTypeArrayFieldInfo(fieldType, std::string(name), type, arraySize);
		}
	};

	struct ShaderInLayoutFieldInfo : public ShaderNativeTypeFieldInfo {
		u32 location = 0;

	public:
		ShaderInLayoutFieldInfo(ShaderFieldType fieldType, std::string name, GLSLType type, u32 location) : ShaderNativeTypeFieldInfo(fieldType, name, type), location(location) {}

		inline virtual ShaderFieldInfo* copy() override {
			return new ShaderInLayoutFieldInfo(fieldType, std::string(name), type, location);
		}
	};

	struct ShaderUniformLayoutFieldInfo : public ShaderNativeTypeFieldInfo {
		u32 binding = 0;

	public:
		ShaderUniformLayoutFieldInfo(ShaderFieldType fieldType, std::string name, GLSLType type, u32 binding) : ShaderNativeTypeFieldInfo(fieldType, name, type), binding(binding) {}

		inline virtual ShaderFieldInfo* copy() override {
			return new ShaderUniformLayoutFieldInfo(fieldType, name, type, binding);
		}
	};

	struct ShaderUniformBufferLayoutFieldInfo : public ShaderFieldInfo {
		UniformLayoutType layoutType = UniformLayoutType::STD140;
		u32 binding = 0;
		ShaderFieldInfo** subFields = nullptr;
		u32 subFieldCount = 0;

	public:
		ShaderUniformBufferLayoutFieldInfo(ShaderFieldType fieldType, std::string name, UniformLayoutType layoutType, u32 binding) : ShaderFieldInfo(fieldType, name), layoutType(layoutType), binding(binding) {}
		~ShaderUniformBufferLayoutFieldInfo() {
			if (subFields != nullptr) {
				for (u32 i = 0; i < subFieldCount; i++) {
					delete subFields[i];
				}
				delete[] subFields;
			}
		}

		inline virtual ShaderFieldInfo* copy() override {
			ShaderUniformBufferLayoutFieldInfo* c = new ShaderUniformBufferLayoutFieldInfo(fieldType, std::string(name), layoutType, binding);
			c->subFields = new ShaderFieldInfo*[subFieldCount];
			c->subFieldCount = subFieldCount;
			for (u32 i = 0; i < subFieldCount; i++) {
				c->subFields[i] = subFields[i]->copy();
			}
			return c;
		}

		inline GLSLType* getMembersTypes() {
			GLSLType* types = new GLSLType[subFieldCount];
			for (u32 i = 0; i < subFieldCount; i++) {
				types[i] = ((ShaderNativeTypeFieldInfo*) subFields[i])->type;
			}
			return types;
		}
	};


	// ---- Meta Info ----
	struct ShaderFileMetaInfo { // TODO rename
		ShaderFieldInfo** shaderFields = nullptr;
		u32 shaderFieldCount = 0;

	public:
		ShaderFileMetaInfo();
		~ShaderFileMetaInfo();
	};

	struct ShaderProgramMetaInfo {
		std::string* passNames = nullptr;
		u32 passCount = 0;
		ShaderFieldInfo** programFields = nullptr;
		u32 programFieldCount = 0;

	public:
		ShaderProgramMetaInfo();
		~ShaderProgramMetaInfo();
	};
}