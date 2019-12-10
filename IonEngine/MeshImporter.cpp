#include "MeshImporter.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include "Mesh.h"
#include "Debug.h"
#include "StringUtils.h"
#include "Entity.h"
#include "MeshRenderer.h"
#include "ShaderProgram.h"
#include "SpecializedShaderProgram.h"
#include "Material.h"
#include "Texture.h"
using namespace IonEngine;



namespace fsys = std::filesystem;

constexpr const char* meshesDirectory = "res/meshes/";


struct Vertex {
	Vec3f position;
	Vec3f normal;
	Vec2f uv;

	bool operator==(const Vertex& o) const {
		return position == o.position && normal == o.normal && uv == o.uv;
	}
};

inline void hash_combine(size_t& seed, size_t hash) {
	hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
	seed ^= hash;
}

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			size_t seed = 0;
			hash<float> hasher;
			hash_combine(seed, hasher(vertex.position.x));
			hash_combine(seed, hasher(vertex.position.y));
			hash_combine(seed, hasher(vertex.position.z));
			hash_combine(seed, hasher(vertex.normal.x));
			hash_combine(seed, hasher(vertex.normal.y));
			hash_combine(seed, hasher(vertex.normal.z));
			hash_combine(seed, hasher(vertex.uv.x));
			hash_combine(seed, hasher(vertex.uv.y));
			return seed;
		}
	};
}


SpecializedShaderProgram* MeshImporter::stdOpaqueShader = nullptr;

Entity* MeshImporter::importAll(std::string name) {

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;

	bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, (meshesDirectory + name).c_str(), "res/meshes/");

	if (!err.empty()) {
		Debug::logError("MeshImporter", "\n" + err);
	}
	if (!success) {
		return nullptr;
	}


	std::vector<Material*> convertedMaterials;
	if (stdOpaqueShader == nullptr) {
		ShaderProgram* stdShader = ShaderProgram::find("standard");
		stdShader->load();
		stdOpaqueShader = stdShader->getSpecializedProgram("opaque");
	}
	for (size_t m = 0; m < materials.size(); m++) {
		Material* mat = new Material(materials[m].name, stdOpaqueShader);

		Texture* texAlbedo;
		if (!materials[m].diffuse_texname.empty()) {
			texAlbedo = new Texture(materials[m].name + "_alb");
			texAlbedo->loadFromFile_stbi(("res/meshes/" + materials[m].diffuse_texname).c_str(), true, 16.0f);
			texAlbedo->uploadToGL();
		} else {
			texAlbedo = Texture::defWhite;
		}
		mat->setTextureByUnit(0, texAlbedo);

		Texture* texNormal;
		if (!materials[m].bump_texname.empty()) {
			texNormal = new Texture(materials[m].name + "_normal");
			texNormal->loadFromFile_stbi(("res/meshes/" + materials[m].bump_texname).c_str(), true, 16.0f);
			texNormal->uploadToGL();
		} else {
			texNormal = Texture::defBlack;
		}
		mat->setTextureByUnit(1, texNormal);
		
		Texture* texMetallic;
		if (!materials[m].ambient_texname.empty()) {
			texMetallic = new Texture(materials[m].name + "_metal");
			texMetallic->loadFromFile_stbi(("res/meshes/" + materials[m].ambient_texname).c_str(), true, 16.0f);
		} else {
			texMetallic = Texture::defBlack;
		}
		Texture* texRough;
		if (!materials[m].specular_highlight_texname.empty()) {
			texRough = new Texture(materials[m].name + "_rough");
			texRough->loadFromFile_stbi(("res/meshes/" + materials[m].specular_highlight_texname).c_str(), true, 16.0f);
		} else {
			texRough = Texture::defWhite;
		}
		Texture* texMRA;
		if (texMetallic != Texture::defBlack && texRough != Texture::defWhite) {
			texMRA = new Texture(materials[m].name + "_mra");
			texMRA->combineTextures(texMetallic, texRough, nullptr, nullptr);
			texMRA->uploadToGL();
			if (texMetallic != Texture::defBlack) {
				delete texMetallic;
			}
			if (texRough != Texture::defWhite) {
				delete texRough;
			}
		} else {
			texMRA = Texture::defMRA;
		}
		mat->setTextureByUnit(2, texMRA);

		mat->setField(0, materials[m].ambient[0]);
		mat->setField(1, 1.0f);
		mat->setField(2, 0.0f);
		convertedMaterials.push_back(mat);
	}

	Entity* root = new Entity();

	for (size_t s = 0; s < shapes.size(); s++) {
		Entity* ent = new Entity();
		ent->setParent(root);
		ent->name = shapes[s].name.c_str();

		// Map all unique vertices with their index in the final mesh array
		std::unordered_map<Vertex, uint32_t> meshVertices;
		int vertexIndex = 0;
		for (size_t i = 0; i < shapes[s].mesh.indices.size(); i++) {
			Vertex vert;
			vert.position = {
				attrib.vertices[3 * shapes[s].mesh.indices[i].vertex_index + 0],
				attrib.vertices[3 * shapes[s].mesh.indices[i].vertex_index + 1],
				attrib.vertices[3 * shapes[s].mesh.indices[i].vertex_index + 2]
			};
			vert.normal = {
				attrib.normals[3 * shapes[s].mesh.indices[i].normal_index + 0],
				attrib.normals[3 * shapes[s].mesh.indices[i].normal_index + 1],
				attrib.normals[3 * shapes[s].mesh.indices[i].normal_index + 2]
			};
			vert.uv = {
				attrib.texcoords[2 * shapes[s].mesh.indices[i].texcoord_index + 0],
				attrib.texcoords[2 * shapes[s].mesh.indices[i].texcoord_index + 1]
			};

			if (meshVertices.count(vert) == 0) {
				meshVertices[vert] = static_cast<uint32_t>(vertexIndex);
				vertexIndex++;
			}
		}

		// Create the mesh
		int vCount = vertexIndex;
		int iCount = shapes[s].mesh.indices.size();
		Mesh* entMesh = new Mesh(shapes[s].name, vCount, iCount);
		entMesh->setAttributesDefinition(5, new int[5]{3, 3, 2, 3, 3});

		// Write all unique vertices
		for (std::pair<Vertex, uint32_t> vert : meshVertices) {
			entMesh->setAttributeElement(0, vert.second, vert.first.position);
			entMesh->setAttributeElement(1, vert.second, vert.first.normal);
			entMesh->setAttributeElement(2, vert.second, Vec2f(vert.first.uv.x, 1.0f - vert.first.uv.y));
		}

		// Write all indices, kinda slow
		int indicesIndex = 0;
		for (size_t i = 0; i < shapes[s].mesh.indices.size(); i++) {
			Vertex vert;
			vert.position = {
				attrib.vertices[3 * shapes[s].mesh.indices[i].vertex_index + 0],
				attrib.vertices[3 * shapes[s].mesh.indices[i].vertex_index + 1],
				attrib.vertices[3 * shapes[s].mesh.indices[i].vertex_index + 2]
			};
			vert.normal = {
				attrib.normals[3 * shapes[s].mesh.indices[i].normal_index + 0],
				attrib.normals[3 * shapes[s].mesh.indices[i].normal_index + 1],
				attrib.normals[3 * shapes[s].mesh.indices[i].normal_index + 2]
			};
			vert.uv = {
				attrib.texcoords[2 * shapes[s].mesh.indices[i].texcoord_index + 0],
				attrib.texcoords[2 * shapes[s].mesh.indices[i].texcoord_index + 1]
			};

			entMesh->setIndex(indicesIndex++, meshVertices[vert]);
		}
		
		entMesh->reverseWindingOrder();
		entMesh->computeTangents(0, 2, 3, 4);

		
		// Assign the mesh to the object
		entMesh->uploadToGL();
		MeshRenderer* renderer = ent->addComponent<MeshRenderer>();
		int matId = shapes[s].mesh.material_ids[0];
		if (matId >= 0) {
			renderer->setMaterial(convertedMaterials[matId]);
		}
		renderer->setMesh(entMesh);
	}

	return root;
}

Mesh* MeshImporter::import(std::string name) {

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;

	bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, (meshesDirectory + name).c_str());

	if (!err.empty()) {
		Debug::logError("MeshImporter", "\n" + err);
	}

	if (!success) {
		return nullptr;
	}

	// TODO move to an importer that generates multiple meshes/objects from a single file
	unsigned int vCount = (unsigned int) attrib.vertices.size();
	unsigned int iCount = 0;
	for (size_t s = 0; s < shapes.size(); s++) {
		iCount += (unsigned int) shapes[s].mesh.indices.size();
	}

	Mesh* mesh = new Mesh(name, vCount, iCount);
	mesh->setAttributesDefinition(3, new int[3]{3, 3, 2});

	std::unordered_map<Vertex, uint32_t> vertices;

	unsigned int indicesIndex = 0;
	unsigned int vertexIndex = 0;
	for (size_t s = 0; s < shapes.size(); s++) {
		for (size_t i = 0; i < shapes[s].mesh.indices.size(); i++) {
			Vertex vert;
			vert.position = {
				attrib.vertices[3 * shapes[s].mesh.indices[i].vertex_index + 0],
				attrib.vertices[3 * shapes[s].mesh.indices[i].vertex_index + 1],
				attrib.vertices[3 * shapes[s].mesh.indices[i].vertex_index + 2]
			};
			vert.normal = {
				attrib.normals[3 * shapes[s].mesh.indices[i].normal_index + 0],
				attrib.normals[3 * shapes[s].mesh.indices[i].normal_index + 1],
				attrib.normals[3 * shapes[s].mesh.indices[i].normal_index + 2]
			};
			vert.uv = {
				attrib.texcoords[2 * shapes[s].mesh.indices[i].texcoord_index + 0],
				attrib.texcoords[2 * shapes[s].mesh.indices[i].texcoord_index + 1]
			};

			if (vertices.count(vert) == 0) {
				vertices[vert] = static_cast<uint32_t>(vertexIndex);
				mesh->setAttributeElement(0, vertexIndex, vert.position);
				mesh->setAttributeElement(1, vertexIndex, vert.normal);
				mesh->setAttributeElement(2, vertexIndex, vert.uv);
				vertexIndex++;
			}

			mesh->setIndex(indicesIndex++, vertices[vert]);
		}
	}

	mesh->reverseWindingOrder();

	return mesh;
}
