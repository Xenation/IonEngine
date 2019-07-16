#include "Material.h"

#include <imgui.h>
#include "ShaderProgram.h"
#include "SpecializedShaderProgram.h"
#include "UniformBuffer.h"
#include "RenderPass.h"
#include "SimpleList.h"
#include "Texture.h"



Material* Material::errorMaterial = nullptr;
HollowSet<Material*> Material::materials(8, 8);

Material* Material::find(std::string name) {
	uint materialProcessed = 0;
	for (uint i = 0; i < materials.capacity && materialProcessed < materials.count; i++) {
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


void Material::gui() {
	if (ImGui::TreeNode((specializedProgram->parentShader->name + "/" + specializedProgram->getRenderPass()->name + "/" + name).c_str())) {
		for (uint texIndex = 0; texIndex < textureFieldCount; texIndex++) {
			TextureField& texField = textureFields[texIndex];
			std::string texName = texField.texture->getName();
			ImGui::Text("Texture: %s (unit=%i)", texName.c_str(), texField.textureUnit);
		}
		for (uint memberIndex = 0; memberIndex < uniformLayout->memberCount; memberIndex++) {
			std::string memberName = uniformLayout->memberNames[memberIndex];
			bool fieldChange = false;
			switch (uniformLayout->members[memberIndex]) {
			case GLSL_BOOL:
				fieldChange = ImGui::Checkbox(memberName.c_str(), uniformLayout->getBoolPtr(memberIndex));
				break;
			case GLSL_INT:
				fieldChange = ImGui::InputInt(memberName.c_str(), uniformLayout->getIntPtr(memberIndex), 1, 10);
				break;
			case GLSL_UINT:
				fieldChange = ImGui::InputScalar(memberName.c_str(), ImGuiDataType_U32, uniformLayout->getUintPtr(memberIndex));
				break;
			case GLSL_FLOAT:
				fieldChange = ImGui::InputFloat(memberName.c_str(), uniformLayout->getFloatPtr(memberIndex), 0.01, 0.1, 3);
				break;
			case GLSL_DOUBLE:
				fieldChange = ImGui::InputDouble(memberName.c_str(), uniformLayout->getDoublePtr(memberIndex), 0.01, 0.1);
				break;
			case GLSL_VEC2:
				fieldChange = ImGui::InputFloat2(memberName.c_str(), (float*) uniformLayout->getVec2fPtr(memberIndex), 3);
				break;
			case GLSL_VEC3:
				fieldChange = ImGui::InputFloat3(memberName.c_str(), (float*) uniformLayout->getVec3fPtr(memberIndex), 3);
				break;
			case GLSL_VEC4:
				fieldChange = ImGui::InputFloat4(memberName.c_str(), (float*) uniformLayout->getVec4fPtr(memberIndex), 3);
				break;
			case GLSL_IVEC2:
				fieldChange = ImGui::InputInt2(memberName.c_str(), (int*) uniformLayout->getVec2iPtr(memberIndex));
				break;
			case GLSL_IVEC3:
				fieldChange = ImGui::InputInt3(memberName.c_str(), (int*) uniformLayout->getVec3iPtr(memberIndex));
				break;
			case GLSL_IVEC4:
				fieldChange = ImGui::InputInt4(memberName.c_str(), (int*) uniformLayout->getVec4iPtr(memberIndex));
				break;
			}
			if (fieldChange) {
				fieldsExpired = true;
			}
		}
		ImGui::TreePop();
	}
}

void Material::setField(unsigned int index, bool value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, int value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, unsigned int value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, float value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, double value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, Vec2f value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, Vec3f value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, Vec4f value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, Color value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value.vec);
}
void Material::setField(unsigned int index, Matrix4x4f value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, Vec2i value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, Vec3i value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
void Material::setField(unsigned int index, Vec4i value) {
	fieldsExpired = true;
	uniformLayout->setMember(index, value);
}
GLuint Material::getTextureLocation(std::string uniformName) {
	return specializedProgram->getUniformLocation(uniformName);
}
void Material::setTextureByLocation(unsigned int location, Texture* texture) {
	for (uint i = 0; i < textureFieldCount; i++) {
		if (textureFields[i].uniformLocation == location) {
			textureFields[i].texture = texture;
		}
	}
}
void Material::setTextureByUnit(unsigned int unit, Texture* texture) {
	for (uint i = 0; i < textureFieldCount; i++) {
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
	for (uint i = 0; i < textureFieldCount; i++) {
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
	for (uint i = 0; i < info->programFieldCount; i++) {
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
	for (uint i = 0; i < textureFieldCount; i++) {
		textureFields[i] = texFields[i];
	}
}

void Material::reloadTextureFields() {
	SimpleList<TextureField> texFields(4, 4);
	fetchTextureFields(texFields);

	TextureField* nTextureFields = new TextureField[texFields.count];
	for (uint i = 0; i < texFields.count; i++) {
		TextureField nField = texFields[i];
		uint existingIndex;
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
	for (uint i = 0; i < info->programFieldCount; i++) {
		if (info->programFields[i]->fieldType == ShaderFieldType::UniformLayout) {
			ShaderUniformLayoutFieldInfo* layoutField = (ShaderUniformLayoutFieldInfo*) info->programFields[i];
			if (layoutField->type & GLSL_IS_SAMPLER) {
				texFields.add({layoutField->name, specializedProgram->getUniformLocation(layoutField->name), layoutField->binding, nullptr});
			}
		}
	}

	// pass for implicit texture units
	for (uint fieldIndex = 0; fieldIndex < info->programFieldCount; fieldIndex++) {
		if (info->programFields[fieldIndex]->fieldType == ShaderFieldType::Uniform) {
			ShaderNativeTypeFieldInfo* nativeField = (ShaderNativeTypeFieldInfo*) info->programFields[fieldIndex];
			if (nativeField->type & GLSL_IS_SAMPLER) {
				uint texUnit;
				GLint maxTexUnit;
				glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexUnit);
				for (texUnit = 0; texUnit < maxTexUnit; texUnit++) {
					uint i;
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
					specializedProgram->loadInt(uniformLocation, (int) texUnit);
				}
			}
		}
	}
}
