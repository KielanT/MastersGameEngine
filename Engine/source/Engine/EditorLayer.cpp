#include "epch.h"
#include "EditorLayer.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace Engine
{
	EditorLayer::EditorLayer()
	{

	}

	EditorLayer::~EditorLayer()
	{
	}
	
	void EditorLayer::Update()
	{
		ImGuiIO& io = ImGui::GetIO();
		
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}
	
	void EditorLayer::RenderGUI()
	{
		DockSpace();
		GameWindow();
		ImGui::ShowDemoWindow();
	}
	
	void EditorLayer::GameWindow()
	{
		ImGui::Begin("Game");

		ImGui::End();
	}

	void EditorLayer::DockSpace()
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	}

}