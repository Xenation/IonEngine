#include "Texture.h"

#include <vector>
#include <lodepng.h>
#include "Debug.h"
#include "GLUtils.h"
using namespace IonEngine;



Texture*const Texture::defWhite = new Texture("white");
Texture*const Texture::defBlack = new Texture("black");

void Texture::initializeDefaults() {
	defWhite->createEmpty(32, 32, GL_RGBA, GL_RGBA8, 0U, false, false);
	defWhite->fillWithColor(Color::white);
	defWhite->uploadToGL();

	defBlack->createEmpty(32, 32, GL_RGBA, GL_RGBA8, 0U, false, false);
	defBlack->fillWithColor(Color::black);
	defBlack->uploadToGL();
}


Texture::Texture(std::string name) : name(name) {}

Texture::~Texture() {
	if (cachedInLocal) {
		deleteLocal();
	}
	if (loadedToGL) {
		deleteFromGL();
	}
}


Texture* Texture::copy() {
	Texture* texture = new Texture();
	if (textureData != nullptr) {
		texture->width = width;
		texture->height = height;
		texture->target = target;
		// samples are 0 if texture has local data
		texture->setTextureData(new unsigned char[textureDataSize], textureDataSize, pixelFormat, pixelInternalFormat);
	} else {
		texture->createEmpty(width, height, pixelFormat, samples, true);
	}
	if (loadedToGL) {
		texture->uploadToGL();
	}
	return texture;
}

void Texture::createEmpty(unsigned int width, unsigned int height, GLenum format, GLenum internalFormat, unsigned int multisamples, bool noalloc, bool mipmapped) {
	if (cachedInLocal) {
		deleteLocal();
	}
	this->mipmapped = mipmapped;
	pixelInternalFormat = internalFormat;
	pixelFormat = format;
	this->width = width;
	this->height = height;
	samples = multisamples;
	if (multisamples != 0) {
		target = GL_TEXTURE_2D_MULTISAMPLE;
	} else {
		if (!noalloc) {
			textureDataSize = glFormatByteSize(pixelFormat, width * height);
			textureData = new unsigned char[textureDataSize];
		}
	}
}

void Texture::loadFromFile(const char* filePath) { // TODO update with internalFormat
	unsigned char* pngData;
	size_t pngDataSize;

	lodepng::State state;
	unsigned int error;
	error = lodepng_load_file(&pngData, &pngDataSize, filePath);
	if (error) {
		Debug::log("Texture", ("Unable to load texture from file '" + std::string(filePath) + "': png load error " + std::to_string(error) + ": " + std::string(lodepng_error_text(error))).c_str());
		return;
	}
	error = lodepng_inspect(&width, &height, &state, pngData, pngDataSize);
	if (error) {
		Debug::log("Texture", ("Unable to load texture from file: png inspect error " + std::to_string(error) + ": " + std::string(lodepng_error_text(error))).c_str());
		return;
	}

	switch (state.info_png.color.colortype) {
	case LodePNGColorType::LCT_GREY:
		pixelFormat = GL_RED;
		switch (state.info_png.color.bitdepth) {
		case 1:
		case 2:
		case 4:
		case 8:
			pixelInternalFormat = GL_R8;
			state.info_raw.bitdepth = 8;
			break;
		case 16:
			pixelInternalFormat = GL_R16;
			state.info_raw.bitdepth = 16;
			break;
		}
		break;
	case LodePNGColorType::LCT_GREY_ALPHA:
		pixelFormat = GL_RG;
		switch (state.info_png.color.bitdepth) {
		case 8:
			pixelInternalFormat = GL_RG8;
			state.info_raw.bitdepth = 8;
			break;
		case 16:
			pixelInternalFormat = GL_RG16;
			state.info_raw.bitdepth = 16;
			break;
		}
		break;
	case LodePNGColorType::LCT_PALETTE:
		Debug::log("Texture", "Unable to load texture from file: palette png format not supported!");
		break;
	case LodePNGColorType::LCT_RGB:
		pixelFormat = GL_RGB;
		switch (state.info_png.color.bitdepth) {
		case 8:
			pixelInternalFormat = GL_RGB8;
			state.info_raw.bitdepth = 8;
			break;
		case 16:
			pixelInternalFormat = GL_RGB16;
			state.info_raw.bitdepth = 16;
			break;
		}
		break;
	case LodePNGColorType::LCT_RGBA:
		pixelFormat = GL_RGBA;
		switch (state.info_png.color.bitdepth) {
		case 8:
			pixelInternalFormat = GL_RGBA8;
			state.info_raw.bitdepth = 8;
			break;
		case 16:
			pixelInternalFormat = GL_RGBA16;
			state.info_raw.bitdepth = 16;
			break;
		}
		break;
	}
	state.info_raw.colortype = state.info_png.color.colortype;

	error = lodepng_decode(&textureData, &width, &height, &state, pngData, pngDataSize);
	if (error) {
		Debug::log("Texture", ("Unable to load texture from file: png decode error " + std::to_string(error) + ": " + std::string(lodepng_error_text(error))).c_str());
		return;
	}

	textureDataSize = glFormatByteSize(pixelInternalFormat, width * height);

	delete[] pngData;

	cachedInLocal = true;
}

void Texture::setTextureData(unsigned char* data, unsigned int dataSize, GLenum format, GLenum internalFormat) {
	textureData = data;
	textureDataSize = dataSize;
	pixelInternalFormat = internalFormat;
	pixelFormat = format;
	cachedInLocal = true;
}

void Texture::fillWithColor(Color color) {
	if (pixelInternalFormat != GL_R8 && pixelInternalFormat != GL_RG8 && pixelInternalFormat != GL_RGB8 && pixelInternalFormat != GL_RGBA8) {
		Debug::logError("Texture", "Tried filling an unsupported format!");
		return;
	}
	unsigned int channelCount = glFormatChannelCount(pixelFormat);
	// Assumes each channel uses exactly a byte
	for (unsigned int pi = 0; pi < width * height; pi++) {
		textureData[pi * channelCount] = (unsigned char) color.vec.data;
	}
}

void Texture::deleteLocal() {
	if (textureData != nullptr) {
		delete[] textureData;
		textureData = nullptr;
	}
	cachedInLocal = false;
}

void Texture::uploadToGL() {
	glGenTextures(1, &textureID);

	glBindTexture(target, textureID);

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (target == GL_TEXTURE_2D_MULTISAMPLE) {
		glTexImage2DMultisample(target, samples, pixelInternalFormat, width, height, GL_FALSE);
	} else {
		glTexImage2D(target, 0, pixelInternalFormat, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, textureData);
		if (mipmapped) {
			glGenerateMipmap(target);
		}
	}

	glBindTexture(target, 0);

	loadedToGL = true;

	if (!name.empty()) {
		std::string fullName = "Texture " + name;
		glObjectLabel(GL_TEXTURE, textureID, (GLsizei) fullName.size(), fullName.c_str());
	}
}

void Texture::deleteFromGL() {
	if (textureID != 0) {
		glDeleteTextures(1, &textureID);
		textureID = 0;
	}
	loadedToGL = false;
}

void Texture::setName(std::string n) {
	name = n;
	if (loadedToGL && !name.empty()) {
		std::string fullName = "Texture " + name;
		glObjectLabel(GL_TEXTURE, textureID, (GLsizei) fullName.size(), fullName.c_str());
	}
}

void Texture::bind() {
	glBindTexture(target, textureID);
}

void Texture::unbind() {
	glBindTexture(target, 0);
}
