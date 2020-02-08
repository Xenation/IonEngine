#pragma once
#include <type_traits>
#include <typeinfo>
#include "Collections/CappedList.h"

namespace IonEngine {
	class Module;
	class EngineCore;

	struct AppDescription {
		const char* name;
		u16 versionMajor;
		u16 versionMinor;
		u16 versionPatch;
	};

	class ModuleManager {
	public:
		ModuleManager(EngineCore* engine) : engine(engine) {}
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
			modules.add(reinterpret_cast<Module*>(mod));
			return mod;
		}

		template<typename T, typename std::enable_if<std::is_base_of<Module, T>::value>::type* = nullptr>
		void removeModule() {
			for (Module*& mod : modules) {
				if (typeid(*mod) == typeid(T)) {
					modules.remove(&mod);
					delete mod;
					return;
				}
			}
		}

		template<typename T, typename std::enable_if<std::is_base_of<Module, T>::value>::type* = nullptr>
		T* getModule() {
			for (Module* mod : modules) {
				if (typeid(*mod) == typeid(T)) return reinterpret_cast<T*>(mod);
			}
			return nullptr;
		}

	private:
		EngineCore*const engine;
		CappedList<Module*, 32> modules;
	};

	class EngineCore {
	public:
		static constexpr u16 versionMajor = 0;
		static constexpr u16 versionMinor = 1;
		static constexpr u16 versionPatch = 0;

		AppDescription appDescription;
		ModuleManager moduleManager;

		EngineCore() : moduleManager(this), appDescription({"Unknown App", 0, 0, 0}) {}
		EngineCore(AppDescription appDesc) : moduleManager(this), appDescription(appDesc) {}
		EngineCore(const EngineCore&) = delete;
		void operator=(const EngineCore&) = delete;
	};
}
