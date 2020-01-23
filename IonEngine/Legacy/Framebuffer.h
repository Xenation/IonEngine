#pragma once
#include <string>
#include <gl3w.h>
#include "XMath.h"

namespace IonEngine {
	class Texture;
	class Material;
	class Mesh;

	class Framebuffer {
	public:
		struct Attachment {
			struct Descriptor {
				GLenum format = GL_RGBA;
				GLenum internalFormat = GL_RGBA8;
				bool mipmapped = false;
				u32 multisample = 0;
				GLenum minFilter = GL_NEAREST;
				GLenum magFilter = GL_NEAREST;
				GLenum wrapS = GL_CLAMP_TO_BORDER;
				GLenum wrapT = GL_CLAMP_TO_BORDER;
				GLenum wrapR = GL_CLAMP_TO_BORDER;
				GLenum compareMode = GL_NONE;
				GLenum compareFunc = GL_LEQUAL;
				Color borderColor = Color::clear;
			};

			GLenum slot;
			Descriptor descriptor;
			Texture* texture;

		public:
			Attachment() : slot(0), descriptor(), texture(nullptr) {}
			Attachment(GLenum attachPoint, Descriptor desc) : slot(attachPoint), descriptor(desc), texture(nullptr) {}
			Attachment(GLenum attachPoint, GLenum format, GLenum internalFormat) : slot(attachPoint), descriptor(), texture(nullptr) {
				descriptor.format = format;
				descriptor.internalFormat = internalFormat;
			}
		};

		static Mesh* fullscreenQuadMesh;

		Color clearColor = Color::clear;

		Framebuffer(std::string name, u32 width, u32 height, u32 samples = 0);
		Framebuffer(const Framebuffer&) = delete;
		~Framebuffer();

		Framebuffer* copy(std::string name);
		void createAttachments(u32 count, Attachment* attachments);
		void resize(u32 width, u32 height);

		void bind();
		void unbind();
		void blitTo(Framebuffer* framebuffer);
		void blitTo(Framebuffer* framebuffer, Material* material, bool bindDepth = false);

		inline u32 getWidth() const { return width; }
		inline u32 getHeight() const { return height; }
		Texture* getTexture(u32 index);

	private:
		static void createFullscreenQuadMesh();

		std::string name;
		GLuint fbo = 0;
		u32 width = 0;
		u32 height = 0;
		u32 samples = 0;
		Attachment* attachments = nullptr;
		u32 attachmentCount = 0;
		GLenum* drawBuffers = nullptr;

		void createAttachment(u32 index);
	};
}
