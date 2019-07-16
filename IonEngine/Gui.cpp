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



Gui::Gui(Window* window) : window(window) {
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


void Gui::update() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (Input::getKeyDown(KeyCode::GRAVE_ACCENT)) {
		debugDisplayed = !debugDisplayed;
	}
	if (debugDisplayed) {
		if (Input::getKeyDown(KeyCode::F1)) {
			statsDisplayed = !statsDisplayed;
		}
		if (Input::getKeyDown(KeyCode::F2)) {
			sceneDisplayed = !sceneDisplayed;
		}
		if (Input::getKeyDown(KeyCode::F3)) {
			shadersDisplayed = !shadersDisplayed;
		}
		if (Input::getKeyDown(KeyCode::F4)) {
			physicsDisplayed = !physicsDisplayed;
			Engine::physicsWorld->debug = physicsDisplayed;
		}
	}

	onUpdate();
	Mesh::triangleCount = 0;
}

void Gui::onUpdate() {
	if (debugDisplayed) {
		ImGui::BeginMainMenuBar();
		if (ImGui::MenuItem("Stats")) {
			statsDisplayed = !statsDisplayed;
		}
		if (ImGui::MenuItem("Scene")) {
			sceneDisplayed = !sceneDisplayed;
		}
		if (ImGui::MenuItem("Shaders")) {
			shadersDisplayed = !shadersDisplayed;
		}
		if (ImGui::MenuItem("Physics")) {
			physicsDisplayed = !physicsDisplayed;
			Engine::physicsWorld->debug = physicsDisplayed;
		}
		ImGui::EndMainMenuBar();

		if (statsDisplayed) {
			ImGui::Begin("Engine Stats");
			addFrameTime(Time::deltaTime);
			ImGui::PlotLines("Delta Times", frameTimes, 300, 0, '\0', 0, maxFrameTime);
			ImGui::Indent();
			ImGui::Text("(max: %.3fms)", maxFrameTime * 1000);
			ImGui::Unindent();
			ImGui::Text("Average Frame time: %.3fms", 1000.0f / ImGui::GetIO().Framerate);
			ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
			ImGui::Text("Triangles: %i", Mesh::triangleCount);
			ImGui::End();
		}

		if (sceneDisplayed) {
			ImGui::Begin("Scene");
			Engine::entityManager->gui();
			ImGui::End();
		}

		if (shadersDisplayed) {
			ImGui::Begin("ShaderPrograms");
			ShaderProgram::guiAll();
			ImGui::End();
		}
	}
}

void Gui::render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::initFrameTimes() {
	for (int i = 0; i < 300; i++) {
		frameTimes[i] = 0;
	}
}

void Gui::addFrameTime(float time) {
	maxFrameTime = 0;
	for (int i = 0; i < 299; i++) {
		frameTimes[i] = frameTimes[i + 1];
		if (frameTimes[i] > maxFrameTime) {
			maxFrameTime = frameTimes[i];
		}
	}
	frameTimes[299] = time;
	if (time > maxFrameTime) {
		maxFrameTime = time;
	}
}
