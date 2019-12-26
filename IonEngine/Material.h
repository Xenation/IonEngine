#pragma once
#include <string>
#include "XMath.h"
#include "GLUtils.h"
#include "HollowSet.h"
#include "SimpleList.h"

namespace IonEngine {
	class ShaderProgram;
	class SpecializedShaderProgram;
	class UniformBuffer;
	struct UniformLayout;
	class Renderer;
	class RenderPass;
	class Texture;

	class Material {
	public:
		static Material* errorMaterial;

		static Material* find(std::string name);

		struct TextureField {
			std::string name;
			GLuint uniformLocation;
			u32 textureUnit;
			Texture* texture;
		};

		const std::string name;
		SpecializedShaderProgram*const specializedProgram;
		HollowSet<Renderer*> renderers;

		Material(std::string name, ShaderProgram* shaderProgram, std::string renderPassName);
		Material(std::string name, ShaderProgram* shaderProgram, RenderPass* renderPass);
		Material(std::string name, SpecializedShaderProgram* specializedProgram);
		~Material();

		void setField(u32 index, bool value);
		void setField(u32 index, i32 value);
		void setField(u32 index, u32 value);
		void setField(u32 index, float value);
		void setField(u32 index, double value);
		void setField(u32 index, Vec2f value);
		void setField(u32 index, Vec3f value);
		void setField(u32 index, Vec4f value);
		void setField(u32 index, Color value);
		void setField(u32 index, Matrix4x4f value);
		void setField(u32 index, Vec2i value);
		void setField(u32 index, Vec3i value);
		void setField(u32 index, Vec4i value);
		void setField(u32 index, u32 arrayIndex, bool value);
		void setField(u32 index, u32 arrayIndex, i32 value);
		void setField(u32 index, u32 arrayIndex, u32 value);
		void setField(u32 index, u32 arrayIndex, float value);
		void setField(u32 index, u32 arrayIndex, double value);
		void setField(u32 index, u32 arrayIndex, Vec2f value);
		void setField(u32 index, u32 arrayIndex, Vec3f value);
		void setField(u32 index, u32 arrayIndex, Vec4f value);
		void setField(u32 index, u32 arrayIndex, Color value);
		void setField(u32 index, u32 arrayIndex, Matrix4x4f value);
		void setField(u32 index, u32 arrayIndex, Vec2i value);
		void setField(u32 index, u32 arrayIndex, Vec3i value);
		void setField(u32 index, u32 arrayIndex, Vec4i value);
		GLuint getTextureLocation(std::string uniformName);
		void setTextureByLocation(u32 location, Texture* texture);
		void setTextureByUnit(u32 unit, Texture* texture);

		TextureField* getTextureFields(u32& count) { count = textureFieldCount; return textureFields; }
		UniformLayout* getUniformLayout() { return uniformLayout; }
		void markFieldChanged() { fieldsExpired = true; }

		void updateFields();
		void use();
		void reload();

	private:
		static HollowSet<Material*> materials;

		UniformBuffer* uniformBuffer;
		UniformLayout* uniformLayout;
		TextureField* textureFields = nullptr;
		u32 textureFieldCount = 0;
		u32 idInProgram = 0;
		u32 idInMaterialStore = 0;
		bool fieldsExpired = true;

		void setupFields();
		void initializeUniformBuffer();
		void initializeTextureFields();
		void reloadTextureFields();
		void fetchTextureFields(SimpleList<TextureField>& texFields);
	};
}
