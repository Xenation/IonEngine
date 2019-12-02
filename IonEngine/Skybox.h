#pragma once
namespace IonEngine {
	class Mesh;
	class ShaderProgram;
	class Material;

	class Skybox {
	public:
		Skybox();
		~Skybox();

		void render();
		Material* getMaterial() const { return skyMaterial; };

	private:
		static Mesh* skyMesh;
		
		Material* skyMaterial = nullptr;
	};
}
