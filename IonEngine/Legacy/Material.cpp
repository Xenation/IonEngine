#include "Material.h"

#include "ShaderProgram.h"
#include "SpecializedShaderProgram.h"
#include "UniformBuffer.h"
#include "RenderPass.h"
#include "SimpleList.h"
#include "Texture.h"
using namespace IonEngine;



Material* Material::errorMaterial = nullptr;
HollowSet<Material*> Material::materials(8, 8);

Material* Material::find(std::string name) {
	u32 materialProcessed = 0;
	for (u32 i = 0; i < materials.capacity && materialProcessed < materials.count; i++) {
		if (materials[i] == nullptr) continue;
		if (materials[i]->name == name) {
			return materials[i];
		}
		materialProcessed++;
	}
	return nullptr;
}


Material::Material(std::string name, ShaderProgram* shaderProgram, std::string renderPassName) : Material(name, shaderProgram->getSpecializedProgram(renderPassName)) {}

Material::Material(std::string name, ShaderProgram* shaderProgram, RenderPass* renderPass) : Material(name, shaderProgram->getSpecializedProgram(renderPass)) {}

Material::Material(std::string name, SpecializedShaderProgram* specializedProgram)
	: name(name), specializedProgram(specializedProgram), uniformBuffer(new UniformBuffer(name)), renderers(4, 16) {
	setupFields();
	idInProgram = specializedProgram->materials.add(this);
	idInMaterialStore = materials.add(this);
}

Material::~Material() {
	if (uniformBuffer != nullptr) {
		delete uniformBuffer;
	}
	if (textureFields != nullptr) {
		delete[] textureFields;
	}
	specializedProgram->materials.remove(idInProgram);
	materials.remove(idInMaterialStore);
}


void Material::setField(u32 index, bool value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(u32 index, i32 value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(u32 index, u32 value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(u32 index, float value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(u32 index, double value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(u32 index, Vec2f value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(u32 index, Vec3f value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(u32 index, Vec4f value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(u32 index, Color value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value.vec);
}
void Material::setField(u32 index, Matrix4x4f value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(u32 index, Vec2i value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(u32 index, Vec3i value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(u32 index, Vec4i value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(u32 index, u32 arrayIndex, bool value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value, arrayIndex);
}
void Material::setField(u32 index, u32 arrayIndex, i32 value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value, arrayIndex);
}
void Material::setField(u32 index, u32 arrayIndex, u32 value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value, arrayIndex);
}
void Material::setField(u32 index, u32 arrayIndex, float value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value, arrayIndex);
}
void Material::setField(u32 index, u32 arrayIndex, double value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value, arrayIndex);
}
void Material::setField(u32 index, u32 arrayIndex, Vec2f value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value, arrayIndex);
}
void Material::setField(u32 index, u32 arrayIndex, Vec3f value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value, arrayIndex);
}
void Material::setField(u32 index, u32 arrayIndex, Vec4f value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value, arrayIndex);
}
void Material::setField(u32 index, u32 arrayIndex, Color value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value.vec, arrayIndex);
}
void Material::setField(u32 index, u32 arrayIndex, Matrix4x4f value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value, arrayIndex);
}
void Material::setField(u32 index, u32 arrayIndex, Vec2i value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value, arrayIndex);
}
void Material::setField(u32 index, u32 arrayIndex, Vec3i value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value, arrayIndex);
}
void Material::setField(u32 index, u32 arrayIndex, Vec4i value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value, arrayIndex);
}
GLuint Material::getTextureLocation(std::string uniformName) {
	return specializedProgram->getUniformLocation(uniformName);
}
void Material::setTextureByLocation(u32 location, Texture* texture) {
	for (u32 i = 0; i < textureFieldCount; i++) {
		if (textureFields[i].uniformLocation == location) {
			textureFields[i].texture = texture;
		}
	}
}
void Material::setTextureByUnit(u32 unit, Texture* texture) {
	for (u32 i = 0; i < textureFieldCount; i++) {
		if (textureFields[i].textureUnit == unit) {
			textureFields[i].texture = texture;
		}
	}
}

void Material::updateFields() {
	fieldsExpired = false;
	uniformBuffer->updateLayout(0);
}

void Material::use() {
	// Buffer layout
	if (fieldsExpired) {
		updateFields();
	}
	uniformBuffer->bindLayout(0);
	// Texture Uniform Samplers
	for (u32 i = 0; i < textureFieldCount; i++) {
		if (textureFields[i].texture == nullptr) continue;
		glActiveTexture(GL_TEXTURE0 + textureFields[i].textureUnit);
		textureFields[i].texture->bind();
	}
}

void Material::reload() {
	initializeUniformBuffer();
	reloadTextureFields();
}

void Material::setupFields() {
	// Uniform Buffer Layout
	initializeUniformBuffer();

	// Uniform Samplers
	initializeTextureFields();
}

void Material::initializeUniformBuffer() {
	ShaderUniformBufferLayoutFieldInfo* uniformField = nullptr;
	ShaderProgramMetaInfo* info = specializedProgram->parentShader->info;
	for (u32 i = 0; i < info->programFieldCount; i++) {
		if (info->programFields[i]->fieldType == ShaderFieldType::UniformBufferLayout) {
			ShaderUniformBufferLayoutFieldInfo* field = (ShaderUniformBufferLayoutFieldInfo*) info->programFields[i];
			if (field->binding == 10) {
				uniformField = field;
				break;
			}
		}
	}
	if (uniformField == nullptr) {
		Debug::logError("Material", "Unable to identify a uniform buffer layout in the targeted shader!");
		return;
	}
	uniformBuffer->setLayouts(1, new UniformLayout[1]{UniformLayout(10, uniformField)});
	uniformBuffer->uploadToGL();
	uniformLayout = &uniformBuffer->getLayout(0);
}

void Material::initializeTextureFields() {
	SimpleList<TextureField> texFields(4, 4);
	fetchTextureFields(texFields);

	// copy fields
	textureFieldCount = texFields.count;
	textureFields = new TextureField[textureFieldCount];
	for (u32 i = 0; i < textureFieldCount; i++) {
		textureFields[i] = texFields[i];
	}
}

void Material::reloadTextureFields() {
	SimpleList<TextureField> texFields(4, 4);
	fetchTextureFields(texFields);

	TextureField* nTextureFields = new TextureField[texFields.count];
	for (u32 i = 0; i < texFields.count; i++) {
		TextureField nField = texFields[i];
		u32 existingIndex;
		for (existingIndex = 0; existingIndex < textureFieldCount; existingIndex++) {
			if (textureFields[existingIndex].name == nField.name) break;
		}
		if (existingIndex != textureFieldCount) { // Has found existing
			nField.texture = textureFields[existingIndex].texture;
		}
		nTextureFields[i] = nField;
	}
	
	delete[] textureFields;
	textureFieldCount = texFields.count;
	textureFields = nTextureFields;
}

void Material::fetchTextureFields(SimpleList<TextureField>& texFields) {
	ShaderProgramMetaInfo* info = specializedProgram->parentShader->info;

	// pass for explicit texture units first
	for (u32 i = 0; i < info->programFieldCount; i++) {
		if (info->programFields[i]->fieldType == ShaderFieldType::UniformLayout) {
			ShaderUniformLayoutFieldInfo* layoutField = (ShaderUniformLayoutFieldInfo*) info->programFields[i];
			if (layoutField->type & GLSL_IS_SAMPLER) {
				texFields.add({layoutField->name, specializedProgram->getUniformLocation(layoutField->name), layoutField->binding, nullptr});
			}
		}
	}

	// pass for implicit texture units
	for (u32 fieldIndex = 0; fieldIndex < info->programFieldCount; fieldIndex++) {
		if (info->programFields[fieldIndex]->fieldType == ShaderFieldType::Uniform) {
			ShaderNativeTypeFieldInfo* nativeField = (ShaderNativeTypeFieldInfo*) info->programFields[fieldIndex];
			if (nativeField->type & GLSL_IS_SAMPLER) {
				u32 texUnit;
				GLint maxTexUnit;
				glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexUnit);
				for (texUnit = 0; texUnit < (u32) maxTexUnit; texUnit++) {
					u32 i;
					for (i = 0; i < texFields.count; i++) {
						if (texFields[i].textureUnit == texUnit) break;
					}
					if (i == texFields.count) { // reached end without finding
						break;
					}
				}
				if (texUnit != maxTexUnit) { // Free unit found
					GLuint uniformLocation = specializedProgram->getUniformLocation(nativeField->name);
					texFields.add({nativeField->name, uniformLocation, texUnit, nullptr});
					specializedProgram->loadInt(uniformLocation, (i32) texUnit);
				}
			}
		}
	}
}
