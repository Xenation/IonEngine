#pragma once
#include <string>
#include <gl3w.h>
class Texture {
public:
	Texture(std::string name = "");
	Texture(const Texture&) = delete;
	~Texture();

	Texture* copy();
	void createEmpty(unsigned int width, unsigned int height, GLenum format, GLenum internalFormat, unsigned int mulisamples = 0, bool noalloc = false, bool mipmapped = true);
	void loadFromFile(const char* filePath);
	void setTextureData(unsigned char* data, unsigned int dataSize, GLenum format, GLenum internalFormat);
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
	inline GLenum getTextureTarget() const { return target; }

private:
	std::string name;
	GLuint textureID = 0;
	bool loadedToGL = false;
	bool cachedInLocal = false;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned char* textureData = nullptr;
	unsigned int textureDataSize = 0;
	GLenum pixelInternalFormat = GL_RGBA8;
	GLenum pixelFormat = GL_RGBA;
	GLenum target = GL_TEXTURE_2D;
	unsigned int samples = 0; // 0: no multisamples
	bool mipmapped = true;
};

