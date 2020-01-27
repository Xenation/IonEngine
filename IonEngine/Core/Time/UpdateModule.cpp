#include "UpdateModule.h"

#include <thread>
#include <chrono>
#include "Time.h"
#include "Core/MathUtils.h"
#include "Core/EngineCore.h"
#include "UpdateTimeline.h"

using namespace IonEngine;



UpdateModule::UpdateModule(EngineCore* core) : Module(core) {
	//timelines.add(new GameLogicTimeline(60.0f));
	timelines.add(new RenderTimeline(144.0f, core));
}

UpdateModule::~UpdateModule() {
	for (UpdateTimeline* timeline : timelines) {
		delete timeline;
	}
}


void UpdateModule::loop() {
	while (!shouldExit) {
		double currentTime = Time::getTime();

		// Find the timeline that should get updated the soonest
		UpdateTimeline* soonestTimeline = nullptr;
		double soonestUpdateTime = f64c::positiveInfinity;
		for (UpdateTimeline* timeline : timelines) {
			double nextUpdateTime = timeline->lastUpdateTime + timeline->updateInterval;
			if (nextUpdateTime < soonestUpdateTime) {
				soonestUpdateTime = nextUpdateTime;
				soonestTimeline = timeline;
			}
		}

		// Execute or wait
		if (soonestUpdateTime > currentTime) { // No timeline is late, wait
			std::this_thread::sleep_for(std::chrono::duration<double>(soonestUpdateTime - currentTime)); // TODO instead of waiting just run the soonest timeline
		} else { // Execute the most late timeline
			float dt = static_cast<float>(currentTime - soonestTimeline->lastUpdateTime);
			soonestTimeline->update(dt);
			soonestTimeline->lastUpdateTime = currentTime;
		}
	}
}

void UpdateModule::exit() {
	shouldExit = true;
}
