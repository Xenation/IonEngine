#include "Gui.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Engine.h"
#include "Window.h"
#include "EntityManager.h"
#include "Input.h"
#include "Time.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "PhysicsWorld.h"
#include "EditorWindow.h"
#include "SceneEditor.h"
#include "MaterialEditor.h"
#include "StatsEditor.h"
#include "ConsoleWindow.h"



Gui::Gui(Window* window) : window(window), editorWindows(16, 16) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void) io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window->glfwWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 400");
}

Gui::~Gui() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


unsigned int Gui::registerEditorWindow(EditorWindow* editor) {
	return editorWindows.add(editor);
}

void Gui::unregisterEditorWindow(unsigned int id) {
	editorWindows.remove(id);
}

void Gui::initialize() {
	sceneEditor = new SceneEditor(Engine::entityManager);
	sceneEditor->hidden = true;

	materialEditor = new MaterialEditor();
	materialEditor->hidden = true;

	statsEditor = new StatsEditor();
	statsEditor->hidden = true;

	consoleWindow = new ConsoleWindow();
	consoleWindow->hidden = true;
}

void Gui::update() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (Input::getKeyDown(KeyCode::GRAVE_ACCENT)) {
		debugDisplayed = !debugDisplayed;
	}
	if (debugDisplayed) {
		if (Input::getKeyDown(KeyCode::F1)) {
			statsEditor->hidden = !statsEditor->hidden;
		}
		if (Input::getKeyDown(KeyCode::F2)) {
			sceneEditor->hidden = !sceneEditor->hidden;
		}
		if (Input::getKeyDown(KeyCode::F3)) {
			materialEditor->hidden = !materialEditor->hidden;
		}
		if (Input::getKeyDown(KeyCode::F4)) {
			physicsDisplayed = !physicsDisplayed;
			Engine::physicsWorld->debug = physicsDisplayed;
		}
		if (Input::getKeyDown(KeyCode::F10)) {
			consoleWindow->hidden = !consoleWindow->hidden;
		}
	}

	onUpdate();
	Mesh::triangleCount = 0;
}

void Gui::onUpdate() {
	if (debugDisplayed) {
		ImGui::BeginMainMenuBar();
		if (ImGui::MenuItem("Stats", "F1")) {
			statsEditor->hidden = !statsEditor->hidden;
		}
		if (ImGui::MenuItem("Scene", "F2")) {
			sceneEditor->hidden = !sceneEditor->hidden;
		}
		if (ImGui::MenuItem("Materials", "F3")) {
			materialEditor->hidden = !materialEditor->hidden;
		}
		if (ImGui::MenuItem("Physics", "F4")) {
			physicsDisplayed = !physicsDisplayed;
			Engine::physicsWorld->debug = physicsDisplayed;
		}
		ImGui::EndMainMenuBar();

		unsigned int counted = 0;
		for (unsigned int i = 0; i < editorWindows.capacity && counted < editorWindows.count; i++) {
			if (editorWindows[i] == nullptr) continue;
			editorWindows[i]->render();
			counted++;
		}
	}
}

void Gui::render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
