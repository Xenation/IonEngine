#pragma once
#include <string>
#include <gl3w.h>
#include "XMath.h"

namespace IonEngine {
	class Texture {
	public:
		struct Descriptor {
			bool allocateLocal = true;
			unsigned int width = 32;
			unsigned int height = 32;
			GLenum format = GL_RGBA;
			GLenum internalFormat = GL_RGBA8;
			GLenum target = GL_TEXTURE_2D;
			unsigned int multisamples = 0; // 0: no multisamples
			bool mipmapped = true;
			float anisotropy = 1.0f;
			GLenum minFilter = GL_LINEAR;
			GLenum magFilter = GL_LINEAR;
			GLenum wrapS = GL_REPEAT;
			GLenum wrapT = GL_REPEAT;
			GLenum wrapR = GL_REPEAT;
			GLenum compareMode = GL_NONE;
			GLenum compareFunc = GL_LEQUAL;
			Color borderColor = Color::clear;
		};

		static Texture*const defWhite;
		static Texture*const defBlack;
		static Texture*const defMRA;

		Texture(std::string name = "");
		Texture(const Texture&) = delete;
		~Texture();

		static void initializeDefaults();

		Texture* copy();
		void createEmpty(Descriptor descriptor);
		void createEmpty(unsigned int width, unsigned int height, GLenum format, GLenum internalFormat, unsigned int mulisamples = 0, bool noalloc = false, bool mipmapped = true, float anisotropy = 1.0f);
		void loadFromFile(const char* filePath, bool mipmapped = true, float anisotropy = 1.0f);
		void combineTextures(Texture* rTexture, Texture* gTexture, Texture* bTexture, Texture* aTexture);
		void setTextureData(unsigned char* data, unsigned int dataSize, GLenum format, GLenum internalFormat);
		void fillWithColor(Color color);
		void deleteLocal();
		void uploadToGL();
		void deleteFromGL();
		void setName(std::string n);
		void bind();
		void unbind();
		inline std::string getName() const { return name; }
		inline bool isLoadedToGL() const { return loadedToGL; }
		inline bool isCachedInLocal() const { return cachedInLocal; }
		inline GLuint getTextureID() const { return textureID; }
		inline GLenum getTextureTarget() const { return desc.target; }

	private:
		std::string name;
		GLuint textureID = 0;
		bool loadedToGL = false;
		bool cachedInLocal = false;
		unsigned char* textureData = nullptr;
		unsigned int textureDataSize = 0;
		Descriptor desc;
	};
}
