#include "ConsoleWindow.h"

#include <iostream>
#include <sstream>
#include <imgui.h>
#include "Debug.h"



ConsoleWindow::ConsoleWindow() : EditorWindow("Console", true), consoleOut(Debug::getOutStream()) {
	
}

ConsoleWindow::~ConsoleWindow() {
	
}


void ConsoleWindow::drawGui() {
	ImGui::BeginChild("Scroll", ImVec2(0, 0), false);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

	std::string consoleStr = consoleOut->str(); // TODO very basic and heavy
	ImGui::TextUnformatted(consoleStr.c_str());
	
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
		ImGui::SetScrollHereY(1.0f);
	}
	ImGui::PopStyleVar();
	ImGui::EndChild();
}
