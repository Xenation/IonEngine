#include "UpdateTimeline.h"

#include "Core/Debug.h" // TODO remove
#include "Core/EngineCore.h"
#include "Render/RenderModule.h"
#include "Display/WindowModule.h"
#include "Core/Time/Time.h"
#include "UpdateModule.h"

using namespace IonEngine;



/* ==== UPDATE TIMELINE ==== */

UpdateTimeline::UpdateTimeline(float ups) : lastUpdateTime(Time::getTime()), updateInterval(1.0f / ups) {

}

UpdateTimeline::~UpdateTimeline() {

}



/* ==== GAME LOGIC TIMELINE ==== */

void GameLogicTimeline::update(float dt) {
	//Debug::logInfo("LOGIC");
}



/* ==== RENDER TIMELINE ==== */

void RenderTimeline::update(float dt) {
	/*core->windowModule->poolEvents();
	core->renderModule->render(dt);
	if (core->renderModule->shouldClose()) {
		core->updateModule->exit();
	}*/
}