#pragma once
#include <string>
#include "XTypes.h"
#include "HollowSet.h"
#include "SimpleSet.h"

namespace IonEngine {
	class SpecializedShaderProgram;
	class Framebuffer;
	class Pipeline;
	class Material;
	class SpecializedShaderProgram;
	class UniformBuffer;
	class Renderer;
	class ShadowCaster;
	class Camera;
	class ShaderStorageBuffer;

	class RenderPass {
	public:
		std::string name;
		HollowSet<SpecializedShaderProgram*> programs;

		RenderPass(const char* name, Pipeline* pipeline);
		~RenderPass();

		virtual void onShadersInitialized() {}
		virtual void prepareFrame() {}
		virtual void prepare() {}
		virtual void render(Camera* camera, const SimpleSet<unsigned int>& visibleRenderers);
		virtual void finish() {}
		virtual void onResize(uint width, uint height) {}

	protected:
		Pipeline* pipeline;
	};

	class RenderPassLightAssign : public RenderPass {
	public:
		RenderPassLightAssign(Pipeline* pipeline);
		~RenderPassLightAssign();

		virtual void onShadersInitialized() override;
		virtual void render(Camera* camera, const SimpleSet<unsigned int>& visibleRenderers) override;

	private:
		Framebuffer* lightAssignBuffer;
		SpecializedShaderProgram* lightAssignSpecShader;
		Material* lightAssignMaterial;
		unsigned int ltwMatrixLocation;
		unsigned int lightTypeLocation;
		unsigned int lightIdLocation;
		unsigned int fillLocation;
		ShaderStorageBuffer* clustersBuffer;

		void renderLightMeshes();
	};

	class RenderPassShadows : public RenderPass {
	public:
		RenderPassShadows(Pipeline* pipeline, HollowSet<Renderer*>& renderers);
		~RenderPassShadows();

		virtual void onShadersInitialized() override;
		virtual void render(Camera* camera, const SimpleSet<unsigned int>& visibleRenderers) override;

	private:
		HollowSet<Renderer*>& renderers;
	};

	class RenderPassOpaque : public RenderPass {
	public:
		RenderPassOpaque(const char* name, Pipeline* pipeline, unsigned int width, unsigned int height, unsigned int samples);
		~RenderPassOpaque();

		virtual void onShadersInitialized() override;
		virtual void prepareFrame() override;
		virtual void prepare() override;
		virtual void finish() override;
		virtual void onResize(unsigned int width, unsigned int height) override;

	private:
		Material* deferredMaterial;
		SpecializedShaderProgram* deferredSpecShader;
		Framebuffer* renderBuffer;

		void updateLightsData();
	};

	class RenderPassTransparent : public RenderPass {
	public:
		RenderPassTransparent(const char* name, Pipeline* pipeline);
		~RenderPassTransparent();

		virtual void prepare() override;
	};

	class RenderPassPostprocess : public RenderPass {
	public:
		RenderPassPostprocess(const char* name, Pipeline* pipeline, Framebuffer* renderBuffer);
		~RenderPassPostprocess();

		virtual void render(Camera* camera, const SimpleSet<unsigned int>& visibleRenderers) override;
		virtual void onResize(uint width, uint height) override;

	private:
		Framebuffer* renderBuffer;
		Framebuffer* temporary1;
		Framebuffer* temporary2;
	};
}
