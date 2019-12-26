#pragma once
#include <string>
#include "XMath.h"
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
	class AtomicCounterBuffer;
	class Light;
	class ComputeShader;
	class Mesh;
	class Skybox;

	class RenderPass {
	public:
		std::string name;
		HollowSet<SpecializedShaderProgram*> programs;

		RenderPass(const char* name, Pipeline* pipeline);
		~RenderPass();

		virtual void onShadersInitialized() {}
		virtual void prepareFrame() {}
		virtual void prepare() {}
		virtual void render(Camera* camera, const SimpleSet<u32>& visibleRenderers);
		virtual void finish() {}
		virtual void onResize(u32 width, u32 height) {}

	protected:
		Pipeline* pipeline;
	};

	class RenderPassLightAssign : public RenderPass {
	public:
		RenderPassLightAssign(Pipeline* pipeline);
		~RenderPassLightAssign();

		virtual void onShadersInitialized() override;
		virtual void render(Camera* camera, const SimpleSet<u32>& visibleRenderers) override;

	private:
		ComputeShader* assignShader;

		ShaderStorageBuffer* clustersBuffer;
		ShaderStorageBuffer* pointLightsBuffer;
		ShaderStorageBuffer* spotLightsBuffer;
		AtomicCounterBuffer* atomicIndexBuffer;
	};

	class RenderPassShadows : public RenderPass {
	public:
		RenderPassShadows(Pipeline* pipeline, HollowSet<Renderer*>& renderers);
		~RenderPassShadows();

		virtual void onShadersInitialized() override;
		virtual void render(Camera* camera, const SimpleSet<u32>& visibleRenderers) override;

	private:
		HollowSet<Renderer*>& renderers;
	};

	class RenderPassOpaque : public RenderPass {
	public:
		RenderPassOpaque(const char* name, Pipeline* pipeline, u32 width, u32 height, u32 samples);
		~RenderPassOpaque();

		virtual void onShadersInitialized() override;
		virtual void prepareFrame() override;
		virtual void prepare() override;
		virtual void finish() override;
		virtual void onResize(u32 width, u32 height) override;

	private:
		Material* deferredMaterial;
		SpecializedShaderProgram* deferredSpecShader;
		Framebuffer* renderBuffer;

		void updateLightsData();
	};

	class RenderPassSkybox : public RenderPass {
	public:
		RenderPassSkybox(Pipeline* pipeline);
		~RenderPassSkybox();

		virtual void onShadersInitialized() override;
		virtual void render(Camera* camera, const SimpleSet<u32>& visibleRenderers) override;

		SpecializedShaderProgram* getProceduralSkyShader() { return proceduralSkySpecShader; }

	private:
		Mesh* skyMesh;
		SpecializedShaderProgram* proceduralSkySpecShader;
		Skybox* defaultSkybox;
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

		virtual void render(Camera* camera, const SimpleSet<u32>& visibleRenderers) override;
		virtual void onResize(u32 width, u32 height) override;

	private:
		Framebuffer* renderBuffer;
		Framebuffer* temporary1;
		Framebuffer* temporary2;
	};
}
