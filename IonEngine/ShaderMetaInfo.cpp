#include "ShaderMetaInfo.h"

using namespace IonEngine;



ShaderFileMetaInfo::ShaderFileMetaInfo() {

}

ShaderFileMetaInfo::~ShaderFileMetaInfo() {
	if (shaderFields != nullptr) {
		for (u32 i = 0; i < shaderFieldCount; i++) {
			delete shaderFields[i];
		}
		delete[] shaderFields;
	}
}


ShaderProgramMetaInfo::ShaderProgramMetaInfo() {

}

ShaderProgramMetaInfo::~ShaderProgramMetaInfo() {
	if (programFields != nullptr) {
		for (u32 i = 0; i < programFieldCount; i++) {
			delete programFields[i];
		}
		delete[] programFields;
	}
}
