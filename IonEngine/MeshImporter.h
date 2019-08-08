#pragma once
#include <string>

namespace IonEngine {
	class Mesh;

	class MeshImporter {
	public:

		static Mesh* Import(std::string name);

	private:
		MeshImporter() = delete;
		MeshImporter(const MeshImporter&) = delete;
		~MeshImporter() = delete;
	};
}
