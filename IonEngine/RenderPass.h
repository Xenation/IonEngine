#pragma once
#include <string>
#include "XTypes.h"
#include "HollowSet.h"

class SpecializedShaderProgram;
class Framebuffer;

class RenderPass {
public:
	std::string name;
	HollowSet<SpecializedShaderProgram*> programs;

	RenderPass(const char* name);
	~RenderPass();

	virtual void prepare();
	virtual void render();
	virtual void onResize(uint width, uint height);
};

class RenderPassOpaque : public RenderPass {
public:
	RenderPassOpaque(const char* name);
	~RenderPassOpaque();

	virtual void prepare() override;
};

class RenderPassTransparent : public RenderPass {
public:
	RenderPassTransparent(const char* name);
	~RenderPassTransparent();

	virtual void prepare() override;
};

class RenderPassPostprocess : public RenderPass {
public:
	RenderPassPostprocess(const char* name, Framebuffer* renderBuffer);
	~RenderPassPostprocess();

	virtual void render() override;
	virtual void onResize(uint width, uint height) override;

private:
	Framebuffer* renderBuffer;
	Framebuffer* temporary1;
	Framebuffer* temporary2;
};
