#include "MaterialEditor.h"

#include <imgui.h>
#include "ShaderProgram.h"
#include "SpecializedShaderProgram.h"
#include "Material.h"
#include "RenderPass.h"
#include "Texture.h"
#include "UniformBuffer.h"
using namespace IonEngine::Editor;



MaterialEditor::MaterialEditor() : EditorWindow("Materials", true) {}

MaterialEditor::~MaterialEditor() {}


void MaterialEditor::drawGui() {
	if (ImGui::Button("Reload Shaders")) {
		ShaderProgram::reloadAll();
	}

	for (u32 shaderIndex = 0; shaderIndex < ShaderProgram::shaderCount; shaderIndex++) {
		u32 count = 0;
		SpecializedShaderProgram** specializedPrograms = ShaderProgram::shaders[shaderIndex]->getAllSpecializedPrograms(count);
		for (u32 specializedIndex = 0; specializedIndex < count; specializedIndex++) {
			SpecializedShaderProgram* specializedProgram = specializedPrograms[specializedIndex];
			if (specializedProgram->getRenderPass() == nullptr) continue;
			u32 counted = 0;
			for (u32 matIndex = 0; matIndex < specializedProgram->materials.capacity || counted < specializedProgram->materials.count; matIndex++) {
				if (specializedProgram->materials[matIndex] == nullptr) continue;
				Material* material = specializedProgram->materials[matIndex];

				if (ImGui::TreeNode(material->name.c_str())) {
					ImGui::Text("Specialized Program: %s", (specializedProgram->parentShader->name + "/" + specializedProgram->getRenderPass()->name).c_str());
					ImGui::TextUnformatted("Parameters:");
					u32 textureFieldCount = 0;
					Material::TextureField* textureFields = material->getTextureFields(textureFieldCount);
					for (u32 texIndex = 0; texIndex < textureFieldCount; texIndex++) {
						Material::TextureField& texField = textureFields[texIndex];
						std::string texName = texField.texture->getName();
						ImGui::Text("Texture: %s (unit=%i)", texName.c_str(), texField.textureUnit);
					}
					UniformLayout* uniformLayout = material->getUniformLayout();
					for (u32 memberIndex = 0; memberIndex < uniformLayout->memberCount; memberIndex++) {
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
							fieldChange = ImGui::InputFloat(memberName.c_str(), uniformLayout->getFloatPtr(memberIndex), 0.01f, 0.1f, 3);
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
							material->markFieldChanged();
						}
					}
					ImGui::TreePop();
				}

				counted++;
			}
		}
	}
}
