#include "Framebuffer.h"

#include "Debug.h"
#include "Texture.h"
#include "GLUtils.h"
#include "Material.h"
#include "Mesh.h"
using namespace IonEngine;



Mesh* Framebuffer::fullscreenQuadMesh = nullptr;

Framebuffer::Framebuffer(std::string name, uint width, uint height, uint samples)
	: name(name), width(width), height(height), samples(samples), attachmentCount(0) {
	glGenFramebuffers(1, &fbo);
	std::string fullName = "Framebuffer " + name;
	glObjectLabel(GL_FRAMEBUFFER, fbo, (GLsizei) fullName.size(), fullName.c_str());
	if (fullscreenQuadMesh == nullptr) {
		createFullscreenQuadMesh();
	}
}

Framebuffer::~Framebuffer() {
	if (attachments != nullptr) {
		for (unsigned int i = 0; i < attachmentCount; i++) {
			delete attachments[i].texture;
		}
		delete[] attachments;
	}
	glDeleteFramebuffers(1, &fbo);
}


Framebuffer* Framebuffer::copy(std::string name) {
	Framebuffer* fb = new Framebuffer(name, width, height, samples);
	Attachment* fbAttachments = new Attachment[attachmentCount];
	for (uint i = 0; i < attachmentCount; i++) {
		fbAttachments[i] = Attachment(attachments[i].slot, attachments[i].descriptor.format, attachments[i].descriptor.internalFormat); // Copies attachments definition
	}
	fb->createAttachments(attachmentCount, fbAttachments);
	return fb;
}

void Framebuffer::createAttachments(uint count, Attachment* attachments) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	this->attachmentCount = count;
	this->attachments = attachments;
	for (uint i = 0; i < attachmentCount; i++) {
		createAttachment(i);
	}

	unsigned int drawBufferCount = 0;
	for (uint i = 0; i < attachmentCount; i++) {
		if (attachments[i].slot == GL_DEPTH_STENCIL_ATTACHMENT || attachments[i].slot == GL_DEPTH_ATTACHMENT || attachments[i].slot == GL_STENCIL_ATTACHMENT) continue;
		drawBufferCount++;
	}
	if (drawBufferCount != 0) {
		drawBuffers = new GLenum[drawBufferCount];
		for (uint i = 0; i < attachmentCount; i++) {
			if (attachments[i].slot == GL_DEPTH_STENCIL_ATTACHMENT || attachments[i].slot == GL_DEPTH_ATTACHMENT || attachments[i].slot == GL_STENCIL_ATTACHMENT) continue;
			drawBuffers[i] = attachments[i].slot;
		}
		glDrawBuffers(drawBufferCount, drawBuffers);
	} else { // Depth only
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Debug::logError("FrameBuffer", "Framebuffer " + glFramebufferStatusString(status));
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::createAttachment(int index) {
	Attachment& attachment = attachments[index];
	if (attachment.texture == nullptr) {
		attachment.texture = new Texture(name + "/" + glAttachmentString(attachment.slot));
	}
	if (attachment.descriptor.internalFormat == 0) {
		attachment.descriptor.internalFormat = glGetDefaultInternalFormat(attachment.descriptor.format);
	}

	Texture::Descriptor texDesc;
	texDesc.allocateLocal = false;
	texDesc.width = width;
	texDesc.height = height;
	texDesc.target = (samples == 0) ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;
	texDesc.format = attachment.descriptor.format;
	texDesc.internalFormat = attachment.descriptor.internalFormat;
	texDesc.multisamples = samples;
	texDesc.minFilter = attachment.descriptor.minFilter;
	texDesc.magFilter = attachment.descriptor.magFilter;
	texDesc.wrapS = attachment.descriptor.wrapS;
	texDesc.wrapT = attachment.descriptor.wrapT;
	texDesc.wrapR = attachment.descriptor.wrapR;
	texDesc.mipmapped = attachment.descriptor.mipmapped;
	texDesc.compareMode = attachment.descriptor.compareMode;
	texDesc.compareFunc = attachment.descriptor.compareFunc;
	texDesc.borderColor = attachment.descriptor.borderColor;

	attachment.texture->createEmpty(texDesc);
	attachment.texture->uploadToGL();
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment.slot, texDesc.target, attachment.texture->getTextureID(), 0);
}

void Framebuffer::resize(uint width, uint height) {
	this->width = width;
	this->height = height;
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// Unbind and delete old textures
	if (attachments != nullptr) {
		for (uint i = 0; i < attachmentCount; i++) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i].slot, (samples == 0) ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, 0, 0);
			attachments[i].texture->deleteFromGL();
		}
	}
	// Generate new textures
	for (uint i = 0; i < attachmentCount; i++) {
		createAttachment(i);
	}
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		Debug::logError("FrameBuffer", "Framebuffer " + glFramebufferStatusString(status));
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bind() {
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
}

void Framebuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::blitTo(Framebuffer* framebuffer) {
	GLuint otherFBO = 0; // 0 to blit to default when target framebuffer is null
	GLint destWidth = width;
	GLint destHeight = height;
	if (framebuffer != nullptr) { // Blit to default
		otherFBO = framebuffer->fbo;
		destWidth = framebuffer->width;
		destHeight = framebuffer->height;
	}
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, otherFBO);
	glBlitFramebuffer(0, 0, width, height, 0, 0, destWidth, destHeight, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void Framebuffer::blitTo(Framebuffer* framebuffer, Material* material, bool bindDepth) {
	GLuint otherFBO = 0;
	GLint destWidth = width;
	GLint destHeight = height;
	if (framebuffer != nullptr) {
		otherFBO = framebuffer->fbo;
		destWidth = framebuffer->width;
		destHeight = framebuffer->height;
	}
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, otherFBO);
	glBlitFramebuffer(0, 0, width, height, 0, 0, destWidth, destHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	unsigned int currentBinding = 0;
	for (unsigned int i = 0; i < attachmentCount; i++) {
		if (!bindDepth && (attachments[i].slot == GL_DEPTH_STENCIL_ATTACHMENT || attachments[i].slot == GL_DEPTH_ATTACHMENT || attachments[i].slot == GL_STENCIL_ATTACHMENT)) continue;
		material->setTextureByUnit(currentBinding++, attachments[i].texture);
	}
	material->use();
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	fullscreenQuadMesh->render();
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

Texture* Framebuffer::getTexture(unsigned int index) {
	if (index >= attachmentCount) return nullptr;
	return attachments[index].texture;
}

void Framebuffer::createFullscreenQuadMesh() {
	fullscreenQuadMesh = new Mesh("FullscreenQuad", 4, 6);
	fullscreenQuadMesh->setAttributesDefinition(2, new int[2]{3, 2}, new GLenum[2]{GL_FLOAT, GL_FLOAT});
	float* data = new float[12]{
		-1, -1, 0,
		-1, 1, 0,
		1, 1, 0,
		1, -1, 0
	};
	fullscreenQuadMesh->setAttribute(0, data);
	delete[] data;
	data = new float[8]{
		0, 0,
		0, 1,
		1, 1,
		1, 0,
	};
	fullscreenQuadMesh->setAttribute(1, data);
	delete[] data;
	fullscreenQuadMesh->setIndices(new uint[6]{
		1, 0, 3,
		1, 3, 2
	});
	fullscreenQuadMesh->uploadToGL();
}
