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
			uint textureUnit;
			Texture* texture;
		};

		const std::string name;
		SpecializedShaderProgram*const specializedProgram;
		HollowSet<Renderer*> renderers;

		Material(std::string name, ShaderProgram* shaderProgram, std::string renderPassName);
		Material(std::string name, ShaderProgram* shaderProgram, RenderPass* renderPass);
		Material(std::string name, SpecializedShaderProgram* specializedProgram);
		~Material();

		void setField(unsigned int index, bool value);
		void setField(unsigned int index, int value);
		void setField(unsigned int index, unsigned int value);
		void setField(unsigned int index, float value);
		void setField(unsigned int index, double value);
		void setField(unsigned int index, Vec2f value);
		void setField(unsigned int index, Vec3f value);
		void setField(unsigned int index, Vec4f value);
		void setField(unsigned int index, Color value);
		void setField(unsigned int index, Matrix4x4f value);
		void setField(unsigned int index, Vec2i value);
		void setField(unsigned int index, Vec3i value);
		void setField(unsigned int index, Vec4i value);
		void setField(unsigned int index, unsigned int arrayIndex, bool value);
		void setField(unsigned int index, unsigned int arrayIndex, int value);
		void setField(unsigned int index, unsigned int arrayIndex, unsigned int value);
		void setField(unsigned int index, unsigned int arrayIndex, float value);
		void setField(unsigned int index, unsigned int arrayIndex, double value);
		void setField(unsigned int index, unsigned int arrayIndex, Vec2f value);
		void setField(unsigned int index, unsigned int arrayIndex, Vec3f value);
		void setField(unsigned int index, unsigned int arrayIndex, Vec4f value);
		void setField(unsigned int index, unsigned int arrayIndex, Color value);
		void setField(unsigned int index, unsigned int arrayIndex, Matrix4x4f value);
		void setField(unsigned int index, unsigned int arrayIndex, Vec2i value);
		void setField(unsigned int index, unsigned int arrayIndex, Vec3i value);
		void setField(unsigned int index, unsigned int arrayIndex, Vec4i value);
		GLuint getTextureLocation(std::string uniformName);
		void setTextureByLocation(unsigned int location, Texture* texture);
		void setTextureByUnit(unsigned int unit, Texture* texture);

		TextureField* getTextureFields(unsigned int& count) { count = textureFieldCount; return textureFields; }
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
		uint textureFieldCount = 0;
		unsigned int idInProgram = 0;
		unsigned int idInMaterialStore = 0;
		bool fieldsExpired = true;

		void setupFields();
		void initializeUniformBuffer();
		void initializeTextureFields();
		void reloadTextureFields();
		void fetchTextureFields(SimpleList<TextureField>& texFields);
	};
}
