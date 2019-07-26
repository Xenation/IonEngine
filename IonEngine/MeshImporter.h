#pragma once
#include <string>

class Mesh;

class MeshImporter {
public:

	static Mesh* Import(std::string name);

private:
	MeshImporter() = delete;
	MeshImporter(const MeshImporter&) = delete;
	~MeshImporter() = delete;
};

