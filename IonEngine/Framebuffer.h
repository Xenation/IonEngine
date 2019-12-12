#pragma once
#include <string>
#include <gl3w.h>
#include "XTypes.h"
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
				unsigned int multisample = 0;
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

		Framebuffer(std::string name, uint width, uint height, uint samples = 0);
		Framebuffer(const Framebuffer&) = delete;
		~Framebuffer();

		Framebuffer* copy(std::string name);
		void createAttachments(uint count, Attachment* attachments);
		void resize(uint width, uint height);

		void bind();
		void unbind();
		void blitTo(Framebuffer* framebuffer);
		void blitTo(Framebuffer* framebuffer, Material* material, bool bindDepth = false);

		inline uint getWidth() const { return width; }
		inline uint getHeight() const { return height; }
		Texture* getTexture(unsigned int index);

	private:
		static void createFullscreenQuadMesh();

		std::string name;
		GLuint fbo = 0;
		unsigned int width = 0;
		unsigned int height = 0;
		unsigned int samples = 0;
		Attachment* attachments = nullptr;
		unsigned int attachmentCount = 0;
		GLenum* drawBuffers = nullptr;

		void createAttachment(int index);
	};
}
