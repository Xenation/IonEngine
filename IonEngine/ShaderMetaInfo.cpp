#include "ShaderMetaInfo.h"



ShaderFileMetaInfo::ShaderFileMetaInfo() {

}

ShaderFileMetaInfo::~ShaderFileMetaInfo() {
	if (shaderFields != nullptr) {
		for (uint i = 0; i < shaderFieldCount; i++) {
			delete shaderFields[i];
		}
		delete[] shaderFields;
	}
}


ShaderProgramMetaInfo::ShaderProgramMetaInfo() {

}

ShaderProgramMetaInfo::~ShaderProgramMetaInfo() {
	if (programFields != nullptr) {
		for (uint i = 0; i < programFieldCount; i++) {
			delete programFields[i];
		}
		delete[] programFields;
	}
}
