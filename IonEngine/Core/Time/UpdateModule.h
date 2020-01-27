#pragma once
#include "Core/Module.h"
#include "Core/Collections/CappedSparseSet.h"

namespace IonEngine {
	class EngineCore;
	class UpdateTimeline;

	class UpdateModule : Module {
	public:
		UpdateModule(EngineCore* core);
		~UpdateModule();
		UpdateModule(const UpdateModule&) = delete;
		void operator=(const UpdateModule&) = delete;

		void loop();
		void exit();

	private:
		bool shouldExit = false;
		CappedSparseSet<UpdateTimeline*, 32> timelines; // TODO replace by priority ordered list
	};
}
