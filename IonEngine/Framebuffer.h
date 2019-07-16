#pragma once
#include <string>
#include <gl3w.h>
#include "XTypes.h"
#include "XMath.h"

class Texture;
class Material;
class Mesh;

class Framebuffer {
public:
	struct Attachment {
		GLenum slot;
		GLenum format;
		Texture* texture;

	public:
		Attachment() : slot(0), format(GL_RGBA), texture(nullptr) {}
		Attachment(GLenum attachPoint, GLenum format) : slot(attachPoint), format(format), texture(nullptr) {}
	};

	static Mesh* fullscreenQuadMesh;

	Color clearColor = Color::clear;

	Framebuffer(std::string name, uint width, uint height, uint samples = 0);
	Framebuffer(const Framebuffer&) = delete;
	~Framebuffer();

	Framebuffer* copy(std::string name);
	void createAttachments(uint count, Attachment* attachments);
	void resize(uint width, uint height);
	
	void bind();
	void unbind();
	void blitTo(Framebuffer* framebuffer);
	void blitTo(Framebuffer* framebuffer, Material* material);

	inline uint getWidth() const { return width; }
	inline uint getHeight() const { return height; }

private:
	static void createFullscreenQuadMesh();

	std::string name;
	GLuint fbo = 0;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int samples = 0;
	Attachment* attachments = nullptr;
	unsigned int attachmentCount = 0;

	void createAttachment(int index);
};

