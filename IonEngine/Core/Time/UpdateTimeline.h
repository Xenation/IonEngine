#pragma once

namespace IonEngine {
	class RenderModule;
	class EngineCore;

	class UpdateTimeline {
	public:
		double lastUpdateTime;
		float updateInterval;

		UpdateTimeline(float ups);
		~UpdateTimeline();
		UpdateTimeline(const UpdateTimeline&) = delete;
		void operator=(const UpdateTimeline&) = delete;

		virtual void update(float dt) = 0;
	};

	class GameLogicTimeline : public UpdateTimeline {
	public:
		GameLogicTimeline(float ups) : UpdateTimeline(ups) {}

		void update(float dt) override;
	};

	class RenderTimeline : public UpdateTimeline {
	public:
		RenderTimeline(float ups, EngineCore* core) : UpdateTimeline(ups), core(core) {}

		void update(float dt) override;

	private:
		EngineCore* core;
	};
}
