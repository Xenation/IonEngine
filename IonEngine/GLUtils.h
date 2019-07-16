#pragma once
#include <string>
#include "XTypes.h"
#include "gl3w.h"

enum GLSLType : unsigned int {
	GLSL_UNKNOWN = 0,
	GLSL_IS_SCALAR	= 0b10000000'00000000'00000000'00000000,
	GLSL_IS_VEC		= 0b01000000'00000000'00000000'00000000,
	GLSL_IS_MAT		= 0b00100000'00000000'00000000'00000000,
	GLSL_IS_SAMPLER	= 0b00010000'00000000'00000000'00000000,
	GLSL_IS_IMAGE	= 0b00001000'00000000'00000000'00000000,
	GLSL_IS_ATOMIC	= 0b00000100'00000000'00000000'00000000,
	GLSL_COMP1_SIZE2= 0b00000000'00000000'00000001'00000000,
	GLSL_COMP1_SIZE3= 0b00000000'00000000'00000010'00000000,
	GLSL_COMP1_SIZE4= 0b00000000'00000000'00000100'00000000,
	GLSL_COMP2_SIZE2= 0b00000000'00000000'00001000'00000000,
	GLSL_COMP2_SIZE3= 0b00000000'00000000'00010000'00000000,
	GLSL_COMP2_SIZE4= 0b00000000'00000000'00100000'00000000,
	GLSL_IS_BOOL	= 0b00000000'00000000'00000000'00000001,
	GLSL_IS_INT		= 0b00000000'00000000'00000000'00000010,
	GLSL_IS_UINT	= 0b00000000'00000000'00000000'00000100,
	GLSL_IS_FLOAT	= 0b00000000'00000000'00000000'00001000,
	GLSL_IS_DOUBLE	= 0b00000000'00000000'00000000'00010000,
	GLSL_IS_1D		= 0b00000000'00000001'00000000'00000000,
	GLSL_IS_2D		= 0b00000000'00000010'00000000'00000000,
	GLSL_IS_3D		= 0b00000000'00000100'00000000'00000000,
	GLSL_IS_CUBE	= 0b00000000'00001000'00000000'00000000,
	GLSL_IS_RECT	= 0b00000000'00010000'00000000'00000000,
	GLSL_IS_ARRAY	= 0b00000000'00100000'00000000'00000000,
	GLSL_IS_MS		= 0b00000000'01000000'00000000'00000000,
	GLSL_IS_BUFFER	= 0b00000000'10000000'00000000'00000000,
	GLSL_IS_SHADOW	= 0b00000001'00000000'00000000'00000000,
	GLSL_IS_VEC2	= GLSL_IS_VEC | GLSL_COMP1_SIZE2,
	GLSL_IS_VEC3	= GLSL_IS_VEC | GLSL_COMP1_SIZE3,
	GLSL_IS_VEC4	= GLSL_IS_VEC | GLSL_COMP1_SIZE4,
	GLSL_IS_MAT2x2	= GLSL_IS_MAT | GLSL_COMP1_SIZE2 | GLSL_COMP2_SIZE2,
	GLSL_IS_MAT2x3	= GLSL_IS_MAT | GLSL_COMP1_SIZE2 | GLSL_COMP2_SIZE3,
	GLSL_IS_MAT2x4	= GLSL_IS_MAT | GLSL_COMP1_SIZE2 | GLSL_COMP2_SIZE4,
	GLSL_IS_MAT3x2	= GLSL_IS_MAT | GLSL_COMP1_SIZE3 | GLSL_COMP2_SIZE2,
	GLSL_IS_MAT3x3	= GLSL_IS_MAT | GLSL_COMP1_SIZE3 | GLSL_COMP2_SIZE3,
	GLSL_IS_MAT3x4	= GLSL_IS_MAT | GLSL_COMP1_SIZE3 | GLSL_COMP2_SIZE4,
	GLSL_IS_MAT4x2	= GLSL_IS_MAT | GLSL_COMP1_SIZE4 | GLSL_COMP2_SIZE2,
	GLSL_IS_MAT4x3	= GLSL_IS_MAT | GLSL_COMP1_SIZE4 | GLSL_COMP2_SIZE3,
	GLSL_IS_MAT4x4	= GLSL_IS_MAT | GLSL_COMP1_SIZE4 | GLSL_COMP2_SIZE4,
	// Scalars
	GLSL_BOOL	= GLSL_IS_SCALAR | GLSL_IS_BOOL,
	GLSL_INT	= GLSL_IS_SCALAR | GLSL_IS_INT,
	GLSL_UINT	= GLSL_IS_SCALAR | GLSL_IS_UINT,
	GLSL_FLOAT	= GLSL_IS_SCALAR | GLSL_IS_FLOAT,
	GLSL_DOUBLE	= GLSL_IS_SCALAR | GLSL_IS_DOUBLE,
	// Vectors
	GLSL_BVEC2	= GLSL_IS_VEC2 | GLSL_IS_BOOL,
	GLSL_IVEC2	= GLSL_IS_VEC2 | GLSL_IS_INT,
	GLSL_UVEC2	= GLSL_IS_VEC2 | GLSL_IS_UINT,
	GLSL_VEC2	= GLSL_IS_VEC2 | GLSL_IS_FLOAT,
	GLSL_DVEC2	= GLSL_IS_VEC2 | GLSL_IS_DOUBLE,
	GLSL_BVEC3	= GLSL_IS_VEC3 | GLSL_IS_BOOL,
	GLSL_IVEC3	= GLSL_IS_VEC3 | GLSL_IS_INT,
	GLSL_UVEC3	= GLSL_IS_VEC3 | GLSL_IS_UINT,
	GLSL_VEC3	= GLSL_IS_VEC3 | GLSL_IS_FLOAT,
	GLSL_DVEC3	= GLSL_IS_VEC3 | GLSL_IS_DOUBLE,
	GLSL_BVEC4	= GLSL_IS_VEC4 | GLSL_IS_BOOL,
	GLSL_IVEC4	= GLSL_IS_VEC4 | GLSL_IS_INT,
	GLSL_UVEC4	= GLSL_IS_VEC4 | GLSL_IS_UINT,
	GLSL_VEC4	= GLSL_IS_VEC4 | GLSL_IS_FLOAT,
	GLSL_DVEC4	= GLSL_IS_VEC4 | GLSL_IS_DOUBLE,
	// Matrices
	GLSL_MAT2x2	= GLSL_IS_FLOAT | GLSL_IS_MAT2x2,
	GLSL_MAT2x3	= GLSL_IS_FLOAT | GLSL_IS_MAT2x3,
	GLSL_MAT2x4	= GLSL_IS_FLOAT | GLSL_IS_MAT2x4,
	GLSL_MAT3x2	= GLSL_IS_FLOAT | GLSL_IS_MAT3x2,
	GLSL_MAT3x3	= GLSL_IS_FLOAT | GLSL_IS_MAT3x3,
	GLSL_MAT3x4	= GLSL_IS_FLOAT | GLSL_IS_MAT3x4,
	GLSL_MAT4x2	= GLSL_IS_FLOAT | GLSL_IS_MAT4x2,
	GLSL_MAT4x3	= GLSL_IS_FLOAT | GLSL_IS_MAT4x3,
	GLSL_MAT4x4	= GLSL_IS_FLOAT | GLSL_IS_MAT4x4,
	GLSL_MAT2	= GLSL_MAT2x2,
	GLSL_MAT3	= GLSL_MAT3x3,
	GLSL_MAT4	= GLSL_MAT4x4,
	GLSL_DMAT2x2 = GLSL_IS_DOUBLE | GLSL_IS_MAT2x2,
	GLSL_DMAT2x3 = GLSL_IS_DOUBLE | GLSL_IS_MAT2x3,
	GLSL_DMAT2x4 = GLSL_IS_DOUBLE | GLSL_IS_MAT2x4,
	GLSL_DMAT3x2 = GLSL_IS_DOUBLE | GLSL_IS_MAT3x2,
	GLSL_DMAT3x3 = GLSL_IS_DOUBLE | GLSL_IS_MAT3x3,
	GLSL_DMAT3x4 = GLSL_IS_DOUBLE | GLSL_IS_MAT3x4,
	GLSL_DMAT4x2 = GLSL_IS_DOUBLE | GLSL_IS_MAT4x2,
	GLSL_DMAT4x3 = GLSL_IS_DOUBLE | GLSL_IS_MAT4x3,
	GLSL_DMAT4x4 = GLSL_IS_DOUBLE | GLSL_IS_MAT4x4,
	GLSL_DMAT2	= GLSL_DMAT2x2,
	GLSL_DMAT3	= GLSL_DMAT3x3,
	GLSL_DMAT4	= GLSL_DMAT4x4,
	// Samplers
	GLSL_SAMPLER1D			= GLSL_IS_SAMPLER | GLSL_IS_FLOAT | GLSL_IS_1D,
	GLSL_SAMPLER2D			= GLSL_IS_SAMPLER | GLSL_IS_FLOAT | GLSL_IS_2D,
	GLSL_SAMPLER3D			= GLSL_IS_SAMPLER | GLSL_IS_FLOAT | GLSL_IS_3D,
	GLSL_SAMPLERCUBE		= GLSL_IS_SAMPLER | GLSL_IS_FLOAT | GLSL_IS_CUBE,
	GLSL_SAMPLER2DRECT		= GLSL_IS_SAMPLER | GLSL_IS_FLOAT | GLSL_IS_2D | GLSL_IS_RECT,
	GLSL_SAMPLER1DARRAY		= GLSL_IS_SAMPLER | GLSL_IS_FLOAT | GLSL_IS_1D | GLSL_IS_ARRAY,
	GLSL_SAMPLER2DARRAY		= GLSL_IS_SAMPLER | GLSL_IS_FLOAT | GLSL_IS_2D | GLSL_IS_ARRAY,
	GLSL_SAMPLERCUBEARRAY	= GLSL_IS_SAMPLER | GLSL_IS_FLOAT | GLSL_IS_CUBE | GLSL_IS_ARRAY,
	GLSL_SAMPLERBUFFER		= GLSL_IS_SAMPLER | GLSL_IS_FLOAT | GLSL_IS_BUFFER,
	GLSL_SAMPLER2DMS		= GLSL_IS_SAMPLER | GLSL_IS_FLOAT | GLSL_IS_2D | GLSL_IS_MS,
	GLSL_SAMPLER2DMSARRAY	= GLSL_IS_SAMPLER | GLSL_IS_FLOAT | GLSL_IS_2D | GLSL_IS_MS | GLSL_IS_ARRAY,
	GLSL_ISAMPLER1D			= GLSL_IS_SAMPLER | GLSL_IS_INT | GLSL_IS_1D,
	GLSL_ISAMPLER2D			= GLSL_IS_SAMPLER | GLSL_IS_INT | GLSL_IS_2D,
	GLSL_ISAMPLER3D			= GLSL_IS_SAMPLER | GLSL_IS_INT | GLSL_IS_3D,
	GLSL_ISAMPLERCUBE		= GLSL_IS_SAMPLER | GLSL_IS_INT | GLSL_IS_CUBE,
	GLSL_ISAMPLER2DRECT		= GLSL_IS_SAMPLER | GLSL_IS_INT | GLSL_IS_2D | GLSL_IS_RECT,
	GLSL_ISAMPLER1DARRAY	= GLSL_IS_SAMPLER | GLSL_IS_INT | GLSL_IS_1D | GLSL_IS_ARRAY,
	GLSL_ISAMPLER2DARRAY	= GLSL_IS_SAMPLER | GLSL_IS_INT | GLSL_IS_2D | GLSL_IS_ARRAY,
	GLSL_ISAMPLERCUBEARRAY	= GLSL_IS_SAMPLER | GLSL_IS_INT | GLSL_IS_CUBE | GLSL_IS_ARRAY,
	GLSL_ISAMPLERBUFFER		= GLSL_IS_SAMPLER | GLSL_IS_INT | GLSL_IS_BUFFER,
	GLSL_ISAMPLER2DMS		= GLSL_IS_SAMPLER | GLSL_IS_INT | GLSL_IS_2D | GLSL_IS_MS,
	GLSL_ISAMPLER2DMSARRAY	= GLSL_IS_SAMPLER | GLSL_IS_INT | GLSL_IS_2D | GLSL_IS_MS | GLSL_IS_ARRAY,
	GLSL_USAMPLER1D			= GLSL_IS_SAMPLER | GLSL_IS_UINT | GLSL_IS_1D,
	GLSL_USAMPLER2D			= GLSL_IS_SAMPLER | GLSL_IS_UINT | GLSL_IS_2D,
	GLSL_USAMPLER3D			= GLSL_IS_SAMPLER | GLSL_IS_UINT | GLSL_IS_3D,
	GLSL_USAMPLERCUBE		= GLSL_IS_SAMPLER | GLSL_IS_UINT | GLSL_IS_CUBE,
	GLSL_USAMPLER2DRECT		= GLSL_IS_SAMPLER | GLSL_IS_UINT | GLSL_IS_2D | GLSL_IS_RECT,
	GLSL_USAMPLER1DARRAY	= GLSL_IS_SAMPLER | GLSL_IS_UINT | GLSL_IS_1D | GLSL_IS_ARRAY,
	GLSL_USAMPLER2DARRAY	= GLSL_IS_SAMPLER | GLSL_IS_UINT | GLSL_IS_2D | GLSL_IS_ARRAY,
	GLSL_USAMPLERCUBEARRAY	= GLSL_IS_SAMPLER | GLSL_IS_UINT | GLSL_IS_CUBE | GLSL_IS_ARRAY,
	GLSL_USAMPLERBUFFER		= GLSL_IS_SAMPLER | GLSL_IS_UINT | GLSL_IS_BUFFER,
	GLSL_USAMPLER2DMS		= GLSL_IS_SAMPLER | GLSL_IS_UINT | GLSL_IS_2D | GLSL_IS_MS,
	GLSL_USAMPLER2DMSARRAY	= GLSL_IS_SAMPLER | GLSL_IS_UINT | GLSL_IS_2D | GLSL_IS_MS | GLSL_IS_ARRAY,
	GLSL_SAMPLER1DSHADOW	= GLSL_IS_SAMPLER | GLSL_IS_FLOAT | GLSL_IS_SHADOW | GLSL_IS_1D,
	GLSL_SAMPLER2DSHADOW	= GLSL_IS_SAMPLER | GLSL_IS_FLOAT | GLSL_IS_SHADOW | GLSL_IS_2D,
	GLSL_SAMPLERCUBESHADOW	= GLSL_IS_SAMPLER | GLSL_IS_FLOAT | GLSL_IS_SHADOW | GLSL_IS_CUBE,
	GLSL_SAMPLER2DRECTSHADOW = GLSL_IS_SAMPLER | GLSL_IS_FLOAT | GLSL_IS_SHADOW | GLSL_IS_2D | GLSL_IS_RECT,
	GLSL_SAMPLER1DARRAYSHADOW = GLSL_IS_SAMPLER | GLSL_IS_FLOAT | GLSL_IS_SHADOW | GLSL_IS_1D | GLSL_IS_ARRAY,
	GLSL_SAMPLER2DARRAYSHADOW = GLSL_IS_SAMPLER | GLSL_IS_FLOAT | GLSL_IS_SHADOW | GLSL_IS_2D | GLSL_IS_ARRAY,
	GLSL_SAMPLERCUBEARRAYSHADOW = GLSL_IS_SAMPLER | GLSL_IS_FLOAT | GLSL_IS_SHADOW | GLSL_IS_CUBE | GLSL_IS_ARRAY,
	// Images
	GLSL_IMAGE1D		= GLSL_IS_IMAGE | GLSL_IS_FLOAT | GLSL_IS_1D,
	GLSL_IMAGE2D		= GLSL_IS_IMAGE | GLSL_IS_FLOAT | GLSL_IS_2D,
	GLSL_IMAGE3D		= GLSL_IS_IMAGE | GLSL_IS_FLOAT | GLSL_IS_3D,
	GLSL_IMAGECUBE		= GLSL_IS_IMAGE | GLSL_IS_FLOAT | GLSL_IS_CUBE,
	GLSL_IMAGE2DRECT	= GLSL_IS_IMAGE | GLSL_IS_FLOAT | GLSL_IS_2D | GLSL_IS_RECT,
	GLSL_IMAGE1DARRAY	= GLSL_IS_IMAGE | GLSL_IS_FLOAT | GLSL_IS_1D | GLSL_IS_ARRAY,
	GLSL_IMAGE2DARRAY	= GLSL_IS_IMAGE | GLSL_IS_FLOAT | GLSL_IS_2D | GLSL_IS_ARRAY,
	GLSL_IMAGECUBEARRAY	= GLSL_IS_IMAGE | GLSL_IS_FLOAT | GLSL_IS_CUBE | GLSL_IS_ARRAY,
	GLSL_IMAGEBUFFER	= GLSL_IS_IMAGE | GLSL_IS_FLOAT | GLSL_IS_BUFFER,
	GLSL_IMAGE2DMS		= GLSL_IS_IMAGE | GLSL_IS_FLOAT | GLSL_IS_2D | GLSL_IS_MS,
	GLSL_IMAGE2DMSARRAY	= GLSL_IS_IMAGE | GLSL_IS_FLOAT | GLSL_IS_2D | GLSL_IS_MS | GLSL_IS_ARRAY,
	GLSL_IIMAGE1D		= GLSL_IS_IMAGE | GLSL_IS_INT | GLSL_IS_1D,
	GLSL_IIMAGE2D		= GLSL_IS_IMAGE | GLSL_IS_INT | GLSL_IS_2D,
	GLSL_IIMAGE3D		= GLSL_IS_IMAGE | GLSL_IS_INT | GLSL_IS_3D,
	GLSL_IIMAGECUBE		= GLSL_IS_IMAGE | GLSL_IS_INT | GLSL_IS_CUBE,
	GLSL_IIMAGE2DRECT	= GLSL_IS_IMAGE | GLSL_IS_INT | GLSL_IS_2D | GLSL_IS_RECT,
	GLSL_IIMAGE1DARRAY	= GLSL_IS_IMAGE | GLSL_IS_INT | GLSL_IS_1D | GLSL_IS_ARRAY,
	GLSL_IIMAGE2DARRAY	= GLSL_IS_IMAGE | GLSL_IS_INT | GLSL_IS_2D | GLSL_IS_ARRAY,
	GLSL_IIMAGECUBEARRAY= GLSL_IS_IMAGE | GLSL_IS_INT | GLSL_IS_CUBE | GLSL_IS_ARRAY,
	GLSL_IIMAGEBUFFER	= GLSL_IS_IMAGE | GLSL_IS_INT | GLSL_IS_BUFFER,
	GLSL_IIMAGE2DMS		= GLSL_IS_IMAGE | GLSL_IS_INT | GLSL_IS_2D | GLSL_IS_MS,
	GLSL_IIMAGE2DMSARRAY= GLSL_IS_IMAGE | GLSL_IS_INT | GLSL_IS_2D | GLSL_IS_MS | GLSL_IS_ARRAY,
	GLSL_UIMAGE1D		= GLSL_IS_IMAGE | GLSL_IS_UINT | GLSL_IS_1D,
	GLSL_UIMAGE2D		= GLSL_IS_IMAGE | GLSL_IS_UINT | GLSL_IS_2D,
	GLSL_UIMAGE3D		= GLSL_IS_IMAGE | GLSL_IS_UINT | GLSL_IS_3D,
	GLSL_UIMAGECUBE		= GLSL_IS_IMAGE | GLSL_IS_UINT | GLSL_IS_CUBE,
	GLSL_UIMAGE2DRECT	= GLSL_IS_IMAGE | GLSL_IS_UINT | GLSL_IS_2D | GLSL_IS_RECT,
	GLSL_UIMAGE1DARRAY	= GLSL_IS_IMAGE | GLSL_IS_UINT | GLSL_IS_1D | GLSL_IS_ARRAY,
	GLSL_UIMAGE2DARRAY	= GLSL_IS_IMAGE | GLSL_IS_UINT | GLSL_IS_2D | GLSL_IS_ARRAY,
	GLSL_UIMAGECUBEARRAY= GLSL_IS_IMAGE | GLSL_IS_UINT | GLSL_IS_CUBE | GLSL_IS_ARRAY,
	GLSL_UIMAGEBUFFER	= GLSL_IS_IMAGE | GLSL_IS_UINT | GLSL_IS_BUFFER,
	GLSL_UIMAGE2DMS		= GLSL_IS_IMAGE | GLSL_IS_UINT | GLSL_IS_2D | GLSL_IS_MS,
	GLSL_UIMAGE2DMSARRAY= GLSL_IS_IMAGE | GLSL_IS_UINT | GLSL_IS_2D | GLSL_IS_MS | GLSL_IS_ARRAY,
	// Atomic Counter
	GLSL_ATOMIC_UINT	= GLSL_IS_ATOMIC | GLSL_IS_UINT,
};

enum class UniformLayoutType {
	STD140,
	SHARED, // TODO Not yet supported
	PACKED, // TODO Not yet supported
};

inline unsigned int glTypeSize(GLenum type) {
	switch (type) {
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
		return 1;
	case GL_SHORT:
	case GL_UNSIGNED_SHORT:
	case GL_HALF_FLOAT:
		return 2;
	case GL_INT:
	case GL_UNSIGNED_INT:
	case GL_FIXED:
	case GL_FLOAT:
		return 4;
	case GL_DOUBLE:
		return 8;
	}
	return 4;
}

inline bool glTypeIsInteger(GLenum type) {
	switch (type) {
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
	case GL_SHORT:
	case GL_UNSIGNED_SHORT:
	case GL_INT:
	case GL_UNSIGNED_INT:
		return true;
	case GL_FIXED:
	case GL_HALF_FLOAT:
	case GL_FLOAT:
	case GL_DOUBLE:
		return false;
	}
	return false;
}

inline unsigned int glGetUniformBufferAlignment(unsigned int currentOffset) {
	static unsigned int uniformBufferAlignment = 0;
	if (uniformBufferAlignment == 0) {
		int align;
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &align);
		uniformBufferAlignment = (unsigned int) align;
	}
	unsigned int disalign = currentOffset % uniformBufferAlignment;
	if (disalign != 0) {
		return currentOffset + (uniformBufferAlignment - disalign);
	} else {
		return currentOffset;
	}
}

inline unsigned int glFormatBitSize(GLenum format) {
	switch (format) {
		// 128
	case GL_RGBA32I:
	case GL_RGBA32UI:
	case GL_RGBA32F:
		return 128;

		// 96
	case GL_RGB32I:
	case GL_RGB32UI:
	case GL_RGB32F:
		return 96;

		// 64
	case GL_RGBA16_SNORM:
	case GL_RGBA16I:
	case GL_RGBA16UI:
	case GL_RGBA16F:
	case GL_RGBA16:
		return 64;

		// 48
	case GL_RGB16_SNORM:
	case GL_RGB16I:
	case GL_RGB16UI:
	case GL_RGB16F:
	case GL_RGB16:
		return 48;

		// 40
	case GL_DEPTH32F_STENCIL8:
		return 40;

		// 32
	case GL_RGB9_E5:
	case GL_SRGB8_ALPHA8:
	case GL_R11F_G11F_B10F:
	case GL_RG16_SNORM:
	case GL_RG16:
	case GL_RGBA8_SNORM:
	case GL_RGBA8I:
	case GL_RGBA8UI:
	case GL_RGBA8:
	case GL_RGBA:
	case GL_DEPTH24_STENCIL8:
	case GL_DEPTH_STENCIL:
	case GL_DEPTH_COMPONENT32:
	case GL_DEPTH_COMPONENT32F:
		return 32;

		// 24
	case GL_SRGB8:
	case GL_RGB8_SNORM:
	case GL_RGB8I:
	case GL_RGB8UI:
	case GL_RGB8:
	case GL_RGB:
	case GL_DEPTH_COMPONENT24:
	case GL_DEPTH_COMPONENT:
		return 24;

		// 16
	case GL_R16_SNORM:
	case GL_R16:
	case GL_RG8_SNORM:
	case GL_RG8:
	case GL_RG:
	case GL_DEPTH_COMPONENT16:
	case GL_STENCIL_INDEX16:
		return 16;

		// 12
	case GL_RGB10_A2UI:
	case GL_RGB10_A2:
		return 12;

		// 8
	case GL_R8_SNORM:
	case GL_R8:
	case GL_RED:
	case GL_STENCIL_INDEX8:
		return 8;

		// 4
	case GL_STENCIL_INDEX4:
		return 4;

		// 1
	case GL_STENCIL_INDEX1:
		return 1;
	}
}

inline unsigned int glFormatByteSize(GLenum format, unsigned int count) {
	unsigned int bitSize = glFormatBitSize(format) * count;
	unsigned int byteSize = bitSize / 8;
	if (bitSize % 8 != 0) {
		byteSize++;
	}
	return byteSize;
}

inline GLenum glGetDefaultInternalFormat(GLenum format) { // TODO add more
	switch (format) {
	case GL_RGBA:
		return GL_RGBA8;
	case GL_DEPTH_COMPONENT:
		return GL_DEPTH_COMPONENT24;
	case GL_STENCIL_INDEX:
		return GL_STENCIL_INDEX8;
	case GL_DEPTH_STENCIL:
		return GL_DEPTH24_STENCIL8;
	}
}

inline std::string glAttachmentString(GLenum attachPoint) {
	switch (attachPoint) {
	case GL_DEPTH_ATTACHMENT:
		return "Depth";
	case GL_STENCIL_ATTACHMENT:
		return "Stencil";
	case GL_DEPTH_STENCIL_ATTACHMENT:
		return "DepthStencil";
	default: //Assumes color
		return "Color" + std::to_string(attachPoint - GL_COLOR_ATTACHMENT0);
	}
}

inline std::string glFramebufferStatusString(GLenum status) {
	switch (status) {
	case GL_FRAMEBUFFER_UNDEFINED:
		return "Undefined";
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		return "Imcomplete Attachment";
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		return "Imcomplete Missing Attachment";
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		return "Imcomplete Draw Buffer";
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		return "Imcomplete Read Buffer";
	case GL_FRAMEBUFFER_UNSUPPORTED:
		return "Unsupported";
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		return "Imcomplete Multisample";
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		return "Imcomplete Layer Targets";
	}
}

inline unsigned int glslTypeBaseAlignment(GLSLType type);
inline unsigned int glslTypeBaseAlignment(GLSLType type, unsigned int arrSize) {
	const unsigned int vec4Align = glslTypeBaseAlignment(GLSL_VEC4);
	if (type & GLSL_IS_MAT) {
		// Rule 6
		if ((type & GLSL_IS_MAT2x2) == GLSL_IS_MAT2x2) {
			if (type & GLSL_IS_DOUBLE) {
				return glslTypeBaseAlignment(GLSL_DVEC2, 2 * arrSize);
			} else {
				return glslTypeBaseAlignment(GLSL_VEC2, 2 * arrSize);
			}
		} else if ((type & GLSL_IS_MAT2x3) == GLSL_IS_MAT2x3) {
			if (type & GLSL_IS_DOUBLE) {
				return glslTypeBaseAlignment(GLSL_DVEC3, 2 * arrSize);
			} else {
				return glslTypeBaseAlignment(GLSL_VEC3, 2 * arrSize);
			}
		} else if ((type & GLSL_IS_MAT2x4) == GLSL_IS_MAT2x4) {
			if (type & GLSL_IS_DOUBLE) {
				return glslTypeBaseAlignment(GLSL_DVEC4, 2 * arrSize);
			} else {
				return glslTypeBaseAlignment(GLSL_VEC4, 2 * arrSize);
			}
		} else if ((type & GLSL_IS_MAT3x2) == GLSL_IS_MAT3x2) {
			if (type & GLSL_IS_DOUBLE) {
				return glslTypeBaseAlignment(GLSL_DVEC2, 3 * arrSize);
			} else {
				return glslTypeBaseAlignment(GLSL_VEC2, 3 * arrSize);
			}
		} else if ((type & GLSL_IS_MAT3x3) == GLSL_IS_MAT3x3) {
			if (type & GLSL_IS_DOUBLE) {
				return glslTypeBaseAlignment(GLSL_DVEC3, 3 * arrSize);
			} else {
				return glslTypeBaseAlignment(GLSL_VEC3, 3 * arrSize);
			}
		} else if ((type & GLSL_IS_MAT3x4) == GLSL_IS_MAT3x4) {
			if (type & GLSL_IS_DOUBLE) {
				return glslTypeBaseAlignment(GLSL_DVEC4, 3 * arrSize);
			} else {
				return glslTypeBaseAlignment(GLSL_VEC4, 3 * arrSize);
			}
		} else if ((type & GLSL_IS_MAT4x2) == GLSL_IS_MAT4x2) {
			if (type & GLSL_IS_DOUBLE) {
				return glslTypeBaseAlignment(GLSL_DVEC2, 4 * arrSize);
			} else {
				return glslTypeBaseAlignment(GLSL_VEC2, 4 * arrSize);
			}
		} else if ((type & GLSL_IS_MAT4x3) == GLSL_IS_MAT4x3) {
			if (type & GLSL_IS_DOUBLE) {
				return glslTypeBaseAlignment(GLSL_DVEC3, 4 * arrSize);
			} else {
				return glslTypeBaseAlignment(GLSL_VEC3, 4 * arrSize);
			}
		} else if ((type & GLSL_IS_MAT4x4) == GLSL_IS_MAT4x4) {
			if (type & GLSL_IS_DOUBLE) {
				return glslTypeBaseAlignment(GLSL_DVEC4, 4 * arrSize);
			} else {
				return glslTypeBaseAlignment(GLSL_VEC4, 4 * arrSize);
			}
		}
	} else {
		// Rule 4
		unsigned int typeAlign = glslTypeBaseAlignment(type);
		if (typeAlign < vec4Align) {
			typeAlign = vec4Align;
		}
		return typeAlign * arrSize;
	}
}

inline unsigned int glslTypeBaseAlignment(GLSLType type) {
	if (type & GLSL_IS_SCALAR) { // Rule 1
		if (type & GLSL_IS_DOUBLE) {
			return 8;
		} else {
			return 4;
		}
	} else if ((type & GLSL_IS_VEC2) == GLSL_IS_VEC2) { // Rule 2
		if (type & GLSL_IS_DOUBLE) {
			return 16;
		} else {
			return 8;
		}
	} else if ((type & GLSL_IS_VEC3) == GLSL_IS_VEC3 || (type & GLSL_IS_VEC4) == GLSL_IS_VEC4) { // Rule 2 and 3
		if (type & GLSL_IS_DOUBLE) {
			return 32;
		} else {
			return 16;
		}
	}
	// Rule 5
	else if ((type & GLSL_IS_MAT2x2) == GLSL_IS_MAT2x2) {
		if (type & GLSL_IS_DOUBLE) {
			return glslTypeBaseAlignment(GLSL_DVEC2, 2);
		} else {
			return glslTypeBaseAlignment(GLSL_VEC2, 2);
		}
	} else if ((type & GLSL_IS_MAT2x3) == GLSL_IS_MAT2x3) {
		if (type & GLSL_IS_DOUBLE) {
			return glslTypeBaseAlignment(GLSL_DVEC3, 2);
		} else {
			return glslTypeBaseAlignment(GLSL_VEC3, 2);
		}
	} else if ((type & GLSL_IS_MAT2x4) == GLSL_IS_MAT2x4) {
		if (type & GLSL_IS_DOUBLE) {
			return glslTypeBaseAlignment(GLSL_DVEC4, 2);
		} else {
			return glslTypeBaseAlignment(GLSL_VEC4, 2);
		}
	} else if ((type & GLSL_IS_MAT3x2) == GLSL_IS_MAT3x2) {
		if (type & GLSL_IS_DOUBLE) {
			return glslTypeBaseAlignment(GLSL_DVEC2, 3);
		} else {
			return glslTypeBaseAlignment(GLSL_VEC2, 3);
		}
	} else if ((type & GLSL_IS_MAT3x3) == GLSL_IS_MAT3x3) {
		if (type & GLSL_IS_DOUBLE) {
			return glslTypeBaseAlignment(GLSL_DVEC3, 3);
		} else {
			return glslTypeBaseAlignment(GLSL_VEC3, 3);
		}
	} else if ((type & GLSL_IS_MAT3x4) == GLSL_IS_MAT3x4) {
		if (type & GLSL_IS_DOUBLE) {
			return glslTypeBaseAlignment(GLSL_DVEC4, 3);
		} else {
			return glslTypeBaseAlignment(GLSL_VEC4, 3);
		}
	} else if ((type & GLSL_IS_MAT4x2) == GLSL_IS_MAT4x2) {
		if (type & GLSL_IS_DOUBLE) {
			return glslTypeBaseAlignment(GLSL_DVEC2, 4);
		} else {
			return glslTypeBaseAlignment(GLSL_VEC2, 4);
		}
	} else if ((type & GLSL_IS_MAT4x3) == GLSL_IS_MAT4x3) {
		if (type & GLSL_IS_DOUBLE) {
			return glslTypeBaseAlignment(GLSL_DVEC3, 4);
		} else {
			return glslTypeBaseAlignment(GLSL_VEC3, 4);
		}
	} else if ((type & GLSL_IS_MAT4x4) == GLSL_IS_MAT4x4) {
		if (type & GLSL_IS_DOUBLE) {
			return glslTypeBaseAlignment(GLSL_DVEC4, 4);
		} else {
			return glslTypeBaseAlignment(GLSL_VEC4, 4);
		}
	}
}

inline GLSLType glslTypeFromString(std::string typeStr) {
	// TODO more safety checks for wrong type strings?

	uint type = GLSL_UNKNOWN;
	uint shapeStart = 0;
	if (typeStr.size() == 0) return GLSL_UNKNOWN;
	switch (typeStr[0]) {
	case 'a':
		return GLSL_ATOMIC_UINT;
	case 'b':
		type |= GLSL_IS_BOOL;
		switch (typeStr[1]) {
		case 'o':
			return GLSL_BOOL;
		case 'v':
			shapeStart = 1;
			type |= GLSL_IS_VEC;
			break;
		default:
			return GLSL_UNKNOWN;
		}
		break;
	case 'd':
		type |= GLSL_IS_DOUBLE;
		switch (typeStr[1]) {
		case 'o':
			return GLSL_DOUBLE;
		case 'v':
			shapeStart = 1;
			type |= GLSL_IS_VEC;
			break;
		case 'm':
			shapeStart = 1;
			type |= GLSL_IS_MAT;
			break;
		default:
			return GLSL_UNKNOWN;
		}
		break;
	case 'f':
		return GLSL_FLOAT;
	case 'i':
		switch (typeStr[1]) {
		case 'n':
			return GLSL_INT;
		case 'v':
			shapeStart = 1;
			type |= GLSL_IS_INT | GLSL_IS_VEC;
			break;
		case 's':
			shapeStart = 1;
			type |= GLSL_IS_INT | GLSL_IS_SAMPLER;
			break;
		case 'm':
			type |= GLSL_IS_FLOAT | GLSL_IS_IMAGE;
			break;
		case 'i':
			shapeStart = 1;
			type |= GLSL_IS_INT | GLSL_IS_IMAGE;
			break;
		default:
			return GLSL_UNKNOWN;
		}
		break;
	case 'm':
		type |= GLSL_IS_FLOAT | GLSL_IS_MAT;
		break;
	case 's':
		type |= GLSL_IS_FLOAT | GLSL_IS_SAMPLER;
		break;
	case 'u':
		type |= GLSL_IS_UINT;
		switch (typeStr[1]) {
		case 'i':
			switch (typeStr[2]) {
			case 'n':
				return GLSL_UINT;
			case 'm':
				shapeStart = 1;
				type |= GLSL_IS_IMAGE;
				break;
			default:
				return GLSL_UNKNOWN;
			}
			break;
		case 'v':
			shapeStart = 1;
			type |= GLSL_IS_VEC;
			break;
		case 's':
			shapeStart = 1;
			type |= GLSL_IS_SAMPLER;
			break;
		default:
			return GLSL_UNKNOWN;
		}
		break;
	case 'v':
		type |= GLSL_IS_FLOAT | GLSL_IS_VEC;
		break;
	default:
		return GLSL_UNKNOWN;
	}

	if (type & GLSL_IS_VEC) {
		switch (typeStr[shapeStart + 3]) {
		case '2':
			type |= GLSL_COMP1_SIZE2;
			break;
		case '3':
			type |= GLSL_COMP1_SIZE3;
			break;
		case '4':
			type |= GLSL_COMP1_SIZE4;
			break;
		default:
			return GLSL_UNKNOWN;
		}
	} else if (type & GLSL_IS_MAT) {
		switch (typeStr[shapeStart + 3]) {
		case '2':
			type |= GLSL_COMP1_SIZE2;
			break;
		case '3':
			type |= GLSL_COMP1_SIZE3;
			break;
		case '4':
			type |= GLSL_COMP1_SIZE4;
			break;
		default:
			return GLSL_UNKNOWN;
		}
		switch (typeStr[shapeStart + 5]) {
		case '2':
			type |= GLSL_COMP2_SIZE2;
			break;
		case '3':
			type |= GLSL_COMP2_SIZE3;
			break;
		case '4':
			type |= GLSL_COMP2_SIZE4;
			break;
		default:
			return GLSL_UNKNOWN;
		}
	} else if (type & GLSL_IS_SAMPLER || type & GLSL_IS_IMAGE) {
		uint nextTypeAttrStart = shapeStart + 7;
		switch (typeStr[nextTypeAttrStart]) {
		case '1':
			nextTypeAttrStart += 2;
			type |= GLSL_IS_1D;
			break;
		case '2':
			nextTypeAttrStart += 2;
			type |= GLSL_IS_2D;
			break;
		case '3':
			type |= GLSL_IS_3D;
			return (GLSLType) type;
		case 'C':
			nextTypeAttrStart += 4;
			type |= GLSL_IS_CUBE;
			break;
		case 'B':
			type |= GLSL_IS_BUFFER;
			return (GLSLType) type;
		default:
			return GLSL_UNKNOWN;
		}
		while (typeStr.size() > nextTypeAttrStart) {
			switch (typeStr[nextTypeAttrStart]) {
			case 'A':
				nextTypeAttrStart += 5;
				type |= GLSL_IS_ARRAY;
				break;
			case 'S':
				if (type & GLSL_IS_IMAGE) {
					return GLSL_UNKNOWN;
				}
				type |= GLSL_IS_SHADOW;
				return (GLSLType) type;
			case 'R':
				if (type & GLSL_IS_2D == 0) {
					return GLSL_UNKNOWN;
				}
				nextTypeAttrStart += 4;
				type |= GLSL_IS_RECT;
				break;
			case 'M':
				if (type & GLSL_IS_2D == 0) {
					return GLSL_UNKNOWN;
				}
				nextTypeAttrStart += 2;
				type |= GLSL_IS_MS;
				break;
			default:
				return GLSL_UNKNOWN;
			}
		}
	}
	
	return (GLSLType) type;
}

inline UniformLayoutType glslUniformLayoutTypeFromString(std::string typeStr) {
	switch (typeStr[0]) {
	case 'p':
		return UniformLayoutType::PACKED;
	case 's':
		if (typeStr[1] == 't') {
			return UniformLayoutType::STD140;
		} else {
			return UniformLayoutType::SHARED;
		}
		break;
	}
}

inline unsigned int glslTypeSize(GLSLType type) {
	switch (type) {
	case GLSL_BOOL:
	case GLSL_INT:
	case GLSL_UINT:
	case GLSL_FLOAT:
		return 4;
	case GLSL_DOUBLE:
	case GLSL_BVEC2:
	case GLSL_IVEC2:
	case GLSL_UVEC2:
	case GLSL_VEC2:
		return 8;
	case GLSL_BVEC3:
	case GLSL_IVEC3:
	case GLSL_UVEC3:
	case GLSL_VEC3:
		return 12;
	case GLSL_BVEC4:
	case GLSL_IVEC4:
	case GLSL_UVEC4:
	case GLSL_VEC4:
	case GLSL_DVEC2:
	case GLSL_MAT2x2:
		return 16;
	case GLSL_DVEC3:
	case GLSL_MAT2x3:
	case GLSL_MAT3x2:
		return 24;
	case GLSL_DVEC4:
	case GLSL_MAT2x4:
	case GLSL_MAT4x2:
	case GLSL_DMAT2x2:
		return 32;
	case GLSL_MAT3x3:
		return 36;
	case GLSL_MAT3x4:
	case GLSL_MAT4x3:
	case GLSL_DMAT2x3:
	case GLSL_DMAT3x2:
		return 48;
	case GLSL_MAT4x4:
	case GLSL_DMAT2x4:
	case GLSL_DMAT4x2:
		return 64;
	case GLSL_DMAT3x3:
		return 72;
	case GLSL_DMAT3x4:
	case GLSL_DMAT4x3:
		return 96;
	case GLSL_DMAT4x4:
		return 128;
	default: // Other types are ints in disguise
		return 4;
	}
}
