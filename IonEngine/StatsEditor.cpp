#include "StatsEditor.h"

#include <imgui.h>
#include "Time.h"
#include "Mesh.h"



StatsEditor::StatsEditor() : EditorWindow("Engine Stats", true) {}

StatsEditor::~StatsEditor() {}


void StatsEditor::drawGui() {
	addFrameTime(Time::deltaTime);
	ImGui::PlotLines("Delta Times", frameTimes, 300, 0, '\0', 0, maxFrameTime);
	ImGui::Indent();
	ImGui::Text("(max: %.3fms)", maxFrameTime * 1000);
	ImGui::Unindent();
	ImGui::Text("Average Frame time: %.3fms", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::Text("Triangles: %i", Mesh::triangleCount);
}

void StatsEditor::initFrameTimes() {
	for (int i = 0; i < 300; i++) {
		frameTimes[i] = 0;
	}
}

void StatsEditor::addFrameTime(float time) {
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

