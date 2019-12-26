#pragma once
#include <string>
#include <gl3w.h>

namespace IonEngine {
	class ComputeShader {
	public:
		const std::string name;

		ComputeShader(std::string name);
		~ComputeShader();
		ComputeShader(const ComputeShader&) = delete;
		ComputeShader& operator=(const ComputeShader&) = delete;

		void load();
		void reload();
		void unload();
		void use();
		void unuse();

	private:
		GLuint program = 0;
		GLuint compute = 0;
		bool loaded = false;

		void loadProgram(bool silent = false);
		void loadComputeShaderFromSourceArray(const char** src, u32 srcPiecesCount, bool silent = false);
	};
}
