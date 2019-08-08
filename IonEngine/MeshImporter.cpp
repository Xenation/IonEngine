#include "MeshImporter.h"

#include <filesystem>
#include <fstream>
#include "Mesh.h"
#include "Debug.h"
#include "StringUtils.h"
using namespace IonEngine;



namespace fsys = std::filesystem;

constexpr const char* meshesDirectory = "res/meshes/";


Mesh* MeshImporter::Import(std::string name) {
	// Very ugly and barebones implementation of obj reader
	// Only supports: 3D vertex pos and triangular faces
	fsys::path filePath = fsys::path(meshesDirectory) / fsys::path(name);
	std::ifstream file(filePath);

	if (!file.is_open()) {
		Debug::logError("MeshImporter", "Could not open file \"" + filePath.string() + "\"!");
		return nullptr;
	}

	std::string line;

	// Counting pass
	unsigned int vCount = 0;
	unsigned int iCount = 0;
	while (std::getline(file, line)) {
		if (line.length() == 0) continue;
		switch (line[0]) {
		case 'v':
			vCount++;
			break;
		case 'f':
			iCount += 3;
			break;
		}
	}

	Mesh* mesh = new Mesh(name, vCount, iCount);
	mesh->setAttributesDefinition(1, new int[1]{3});

	// Fill pass
	unsigned int vIndex = 0;
	unsigned int iIndex = 0;
	file.clear();
	file.seekg(0, file.beg);
	while (std::getline(file, line)) {
		if (line.length() == 0) continue;
		switch (line[0]) {
		case 'v': {
			Vec3f vpos;
			int compIndex = 0;
			std::string vdata = line.substr(2, line.length() - 2);
			int currentLength = 9;
			for (int offset = 0; offset < vdata.length(); offset += currentLength) {
				if (vdata[offset] == '-') {
					currentLength = 10;
				} else {
					currentLength = 9;
				}
				vpos[compIndex++] = std::stof(vdata.substr(offset, currentLength));
			}
			mesh->setAttributeElement(0, vIndex++, vpos);
			break;
		}
		case 'f': {
			SimpleList<std::string> indicesStr(3, 1);
			std::string idata = line.substr(2, line.length() - 2);
			splitString(idata, ' ', indicesStr);
			for (int i = indicesStr.count - 1; i >= 0; i--) {
				int index = std::atoi(indicesStr[i].c_str()) - 1;
				mesh->setIndex(iIndex++, index);
			}
			break;
		}
		}
	}

	return mesh;
}
