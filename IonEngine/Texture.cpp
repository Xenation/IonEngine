#include "Texture.h"

#include <vector>
#include <lodepng.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Debug.h"
#include "GLUtils.h"
using namespace IonEngine;



Texture*const Texture::defWhite = new Texture("white");
Texture*const Texture::defBlack = new Texture("black");
Texture*const Texture::defMRA = new Texture("defMRA");

void Texture::initializeDefaults() {
	defWhite->createEmpty(32, 32, GL_RGBA, GL_RGBA8, 0U, false, false);
	defWhite->fillWithColor(Color::white);
	defWhite->uploadToGL();

	defBlack->createEmpty(32, 32, GL_RGBA, GL_RGBA8, 0U, false, false);
	defBlack->fillWithColor(Color::black);
	defBlack->uploadToGL();

	defMRA->combineTextures(defBlack, defWhite, defWhite, nullptr);
	defMRA->uploadToGL();
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
		texture->desc = desc;
		// samples are 0 if texture has local data
		texture->setTextureData(new unsigned char[textureDataSize], textureDataSize, desc.format, desc.internalFormat);
	} else {
		texture->createEmpty(desc);
	}
	if (loadedToGL) {
		texture->uploadToGL();
	}
	return texture;
}

void Texture::createEmpty(Descriptor descriptor) {
	if (cachedInLocal) {
		deleteLocal();
	}
	desc = descriptor;
	if (desc.multisamples == 0 && desc.allocateLocal) {
		textureDataSize = glFormatByteSize(desc.format, desc.width * desc.height);
		textureData = new unsigned char[textureDataSize];
	}
}

void Texture::createEmpty(unsigned int width, unsigned int height, GLenum format, GLenum internalFormat, unsigned int multisamples, bool noalloc, bool mipmapped, float anisotropy) {
	if (cachedInLocal) {
		deleteLocal();
	}
	desc.allocateLocal = !noalloc;
	desc.mipmapped = mipmapped;
	desc.anisotropy = anisotropy;
	desc.internalFormat = internalFormat;
	desc.format = format;
	desc.width = width;
	desc.height = height;
	desc.multisamples = multisamples;
	if (multisamples != 0) {
		desc.target = GL_TEXTURE_2D_MULTISAMPLE;
	} else if (desc.allocateLocal) {
		textureDataSize = glFormatByteSize(desc.format, desc.width * desc.height);
		textureData = new unsigned char[textureDataSize];
	}
}

void Texture::loadFromFile(const char* filePath) {
	unsigned char* pngData;
	size_t pngDataSize;

	lodepng::State state;
	unsigned int error;
	error = lodepng_load_file(&pngData, &pngDataSize, filePath);
	if (error) {
		Debug::log("Texture", ("Unable to load texture from file '" + std::string(filePath) + "': png load error " + std::to_string(error) + ": " + std::string(lodepng_error_text(error))).c_str());
		return;
	}
	error = lodepng_inspect(&desc.width, &desc.height, &state, pngData, pngDataSize);
	if (error) {
		Debug::log("Texture", ("Unable to load texture from file: png inspect error " + std::to_string(error) + ": " + std::string(lodepng_error_text(error))).c_str());
		return;
	}

	switch (state.info_png.color.colortype) {
	case LodePNGColorType::LCT_GREY:
		desc.format = GL_RED;
		switch (state.info_png.color.bitdepth) {
		case 1:
		case 2:
		case 4:
		case 8:
			desc.internalFormat = GL_R8;
			state.info_raw.bitdepth = 8;
			break;
		case 16:
			desc.internalFormat = GL_R16;
			state.info_raw.bitdepth = 16;
			break;
		}
		break;
	case LodePNGColorType::LCT_GREY_ALPHA:
		desc.format = GL_RG;
		switch (state.info_png.color.bitdepth) {
		case 8:
			desc.internalFormat = GL_RG8;
			state.info_raw.bitdepth = 8;
			break;
		case 16:
			desc.internalFormat = GL_RG16;
			state.info_raw.bitdepth = 16;
			break;
		}
		break;
	case LodePNGColorType::LCT_PALETTE:
		Debug::log("Texture", "Unable to load texture from file: palette png format not supported!");
		break;
	case LodePNGColorType::LCT_RGB:
		desc.format = GL_RGB;
		switch (state.info_png.color.bitdepth) {
		case 8:
			desc.internalFormat = GL_RGB8;
			state.info_raw.bitdepth = 8;
			break;
		case 16:
			desc.internalFormat = GL_RGB16;
			state.info_raw.bitdepth = 16;
			break;
		}
		break;
	case LodePNGColorType::LCT_RGBA:
		desc.format = GL_RGBA;
		switch (state.info_png.color.bitdepth) {
		case 8:
			desc.internalFormat = GL_RGBA8;
			state.info_raw.bitdepth = 8;
			break;
		case 16:
			desc.internalFormat = GL_RGBA16;
			state.info_raw.bitdepth = 16;
			break;
		}
		break;
	}
	state.info_raw.colortype = state.info_png.color.colortype;

	error = lodepng_decode(&textureData, &desc.width, &desc.height, &state, pngData, pngDataSize);
	if (error) {
		Debug::log("Texture", ("Unable to load texture from file: png decode error " + std::to_string(error) + ": " + std::string(lodepng_error_text(error))).c_str());
		return;
	}

	textureDataSize = glFormatByteSize(desc.internalFormat, desc.width * desc.height);

	delete[] pngData;

	cachedInLocal = true;
}

void Texture::loadFromFile_stbi(const char* filePath, bool mipmapped, float anisotropy) {
	int channelCount = 0;
	textureData = stbi_load(filePath, (int*) &desc.width, (int*) &desc.height, &channelCount, 0);
	if (textureData == nullptr) {
		Debug::logError("Texture", "Unable to load texture from file '" + std::string(filePath) + "': " + std::string(stbi_failure_reason()));
		return;
	}
	textureDataSize = glFormatByteSize(desc.internalFormat, desc.width * desc.height);

	desc.mipmapped = mipmapped;
	desc.anisotropy = anisotropy;

	if (desc.mipmapped) {
		desc.minFilter = GL_LINEAR_MIPMAP_LINEAR;
	}

	switch (channelCount) {
		case 1:
			desc.format = GL_RED;
			desc.internalFormat = GL_R8;
			break;
		case 2:
			desc.format = GL_RG;
			desc.internalFormat = GL_RG8;
			break;
		case 3:
			desc.format = GL_RGB;
			desc.internalFormat = GL_RGB8;
			break;
		case 4:
			desc.format = GL_RGBA;
			desc.internalFormat = GL_RGBA8;
			break;
	}

	cachedInLocal = true;
}

void Texture::combineTextures(Texture* rTex, Texture* gTex, Texture* bTex, Texture* aTex) {
	if (rTex == nullptr) return;
	int channelCount = 1;
	desc = rTex->desc;
	desc.format = GL_RED;
	desc.internalFormat = GL_R8;
	if (gTex != nullptr) {
		desc.format = GL_RG;
		desc.internalFormat = GL_RG8;
		channelCount++;
		if (gTex->desc.width != desc.width || gTex->desc.height != desc.height) {
			return;
		}
	}
	if (bTex != nullptr) {
		desc.format = GL_RGB;
		desc.internalFormat = GL_RGB8;
		channelCount++;
		if (bTex->desc.width != desc.width || bTex->desc.height != desc.height) {
			return;
		}
	}
	if (aTex != nullptr) {
		desc.format = GL_RGBA;
		desc.internalFormat = GL_RGBA8;
		channelCount++;
		if (aTex->desc.width != desc.width || aTex->desc.height != desc.height) {
			return;
		}
	}
	textureDataSize = rTex->desc.width * rTex->desc.height * channelCount;
	textureData = new unsigned char[textureDataSize];

	Texture* channelTextures[4] {rTex, gTex, bTex, aTex};
	unsigned int writtenChannelIndex = 0;
	for (int ci = 0; ci < 4; ci++) {
		if (channelTextures[ci] == nullptr) continue;
		for (unsigned int pi = 0; pi < desc.width * desc.height; pi++) {
			unsigned int otherChannelCount = glFormatChannelCount(channelTextures[ci]->desc.format);
			textureData[pi * channelCount + writtenChannelIndex] = channelTextures[ci]->textureData[pi * otherChannelCount];
		}
		writtenChannelIndex++;
	}

	cachedInLocal = true;
}

void Texture::setTextureData(unsigned char* data, unsigned int dataSize, GLenum format, GLenum internalFormat) {
	textureData = data;
	textureDataSize = dataSize;
	desc.internalFormat = internalFormat;
	desc.format = format;
	cachedInLocal = true;
}

void Texture::fillWithColor(Color color) {
	if (desc.internalFormat != GL_R8 && desc.internalFormat != GL_RG8 && desc.internalFormat != GL_RGB8 && desc.internalFormat != GL_RGBA8) {
		Debug::logError("Texture", "Tried filling an unsupported format!");
		return;
	}
	unsigned int channelCount = glFormatChannelCount(desc.format);
	// Assumes each channel uses exactly a byte
	for (unsigned int pi = 0; pi < desc.width * desc.height; pi++) {
		color.toBytesRGBA8(textureData + pi * channelCount);
		//textureData[pi * channelCount] = color.toInt();
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

	glBindTexture(desc.target, textureID);

	glTexParameteri(desc.target, GL_TEXTURE_MIN_FILTER, desc.minFilter);
	glTexParameteri(desc.target, GL_TEXTURE_MAG_FILTER, desc.magFilter);
	glTexParameteri(desc.target, GL_TEXTURE_WRAP_S, desc.wrapS);
	glTexParameteri(desc.target, GL_TEXTURE_WRAP_T, desc.wrapT);
	glTexParameteri(desc.target, GL_TEXTURE_WRAP_R, desc.wrapR);
	if (desc.wrapS == GL_CLAMP_TO_BORDER || desc.wrapT == GL_CLAMP_TO_BORDER || desc.wrapR == GL_CLAMP_TO_BORDER) {
		glTexParameterfv(desc.target, GL_TEXTURE_BORDER_COLOR, desc.borderColor.data);
	}
	glTexParameterf(desc.target, GL_TEXTURE_MAX_ANISOTROPY, desc.anisotropy);
	glTexParameteri(desc.target, GL_TEXTURE_COMPARE_MODE, desc.compareMode);
	glTexParameteri(desc.target, GL_TEXTURE_COMPARE_FUNC, desc.compareFunc);

	if (desc.target == GL_TEXTURE_2D_MULTISAMPLE) {
		glTexImage2DMultisample(desc.target, desc.multisamples, desc.internalFormat, desc.width, desc.height, GL_FALSE);
	} else {
		glTexImage2D(desc.target, 0, desc.internalFormat, desc.width, desc.height, 0, desc.format, GL_UNSIGNED_BYTE, textureData);
		if (desc.mipmapped) {
			glGenerateMipmap(desc.target);
		}
	}

	glBindTexture(desc.target, 0);

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
	glBindTexture(desc.target, textureID);
}

void Texture::unbind() {
	glBindTexture(desc.target, 0);
}
