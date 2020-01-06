#pragma once
#include <type_traits>
#include <typeinfo>
#include "Collections/SimpleList.h"

namespace IonEngine {
	class Module;
	class EngineCore;

	class ModuleManager {
	public:
		ModuleManager(EngineCore* engine) : engine(engine), modules(8, 8) {}
		~ModuleManager();
		ModuleManager(const ModuleManager&) = delete;
		void operator=(const ModuleManager&) = delete;

		template<typename T, typename std::enable_if<std::is_base_of<Module, T>::value>::type* = nullptr>
		T* addModule() {
			T* mod = new T(engine);
			return addModule(mod);
		}

		template<typename T, typename std::enable_if<std::is_base_of<Module, T>::value>::type* = nullptr>
		T* addModule(T* mod) {
			if (getModule<T>() != nullptr) {
				// TODO Log Error
				return nullptr;
			}
			modules.add((Module*) mod);
			return mod;
		}

		template<typename T, typename std::enable_if<std::is_base_of<Module, T>::value>::type* = nullptr>
		void removeModule() {
			for (u32 i = 0; i < modules.count; i++) {
				if (typeid(*modules[i]) == typeid(T)) {
					modules.removeAt(i);
					delete modules[i];
					return;
				}
			}
		}

		template<typename T, typename std::enable_if<std::is_base_of<Module, T>::value>::type* = nullptr>
		T* getModule() {
			for (u32 i = 0; i < modules.count; i++) {
				if (typeid(*modules[i]) == typeid(T)) return (T*) modules[i];
			}
			return nullptr;
		}

	private:
		EngineCore*const engine;
		SimpleList<Module*> modules;
	};

	class EngineCore {
	public:
		ModuleManager moduleManager;

		EngineCore() : moduleManager(this) {}
		EngineCore(const EngineCore&) = delete;
		void operator=(const EngineCore&) = delete;
	};
}
