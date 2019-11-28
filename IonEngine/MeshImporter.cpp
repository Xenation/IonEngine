#include "MeshImporter.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include "Mesh.h"
#include "Debug.h"
#include "StringUtils.h"
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

Mesh* MeshImporter::Import(std::string name) {

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;

	bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, (meshesDirectory + name).c_str());

	if (!err.empty()) {
		Debug::logError("MeshImporter", err);
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
