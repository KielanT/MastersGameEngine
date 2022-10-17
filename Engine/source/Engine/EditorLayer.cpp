#include "epch.h"
#include "EditorLayer.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace Engine
{
	EditorLayer::~EditorLayer()
	{
	}
	void EditorLayer::RenderGUI()
	{
		ImGui::ShowDemoWindow();
		MainWindow();
	}
	void EditorLayer::MainWindow()
	{
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui::Begin("Test");

		ImGui::End();
	}
}