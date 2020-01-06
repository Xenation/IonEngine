﻿#pragma once

namespace IonEngine {
	class EngineCore;

	class Module {
	public:
		Module(EngineCore* engine) : engine(engine) {}
		Module(const Module&) = delete;
		void operator=(const Module&) = delete;

	protected:
		EngineCore*const engine;
	};
}
