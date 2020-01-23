#pragma once
#include <string>

namespace IonEngine {
	class Mesh;
	class Entity;
	class SpecializedShaderProgram;

	class MeshImporter {
	public:

		static Entity* importAll(std::string name);
		static Mesh* import(std::string name);

	private:
		MeshImporter() = delete;
		MeshImporter(const MeshImporter&) = delete;
		~MeshImporter() = delete;

		static SpecializedShaderProgram* stdOpaqueShader;
	};
}
