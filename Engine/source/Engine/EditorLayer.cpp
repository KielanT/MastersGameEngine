#include "epch.h"
#include "EditorLayer.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace Engine
{
	EditorLayer::EditorLayer(CComPtr<ID3D11ShaderResourceView> sceneTexture)
	{
		m_SceneTexture = sceneTexture;
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
		//MainWindow();
		GameWindow();
		EntitiesWindow();
		Details();
		Assets();
		//ImGui::ShowDemoWindow();
	}
	
	void EditorLayer::DockSpace()
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	}

	void EditorLayer::MainWindow()
	{
		ImGui::Begin("Main");

		ImGui::End();
	}
	
	void EditorLayer::GameWindow()
	{
		ImGui::Begin("Game");
		//ImVec2 size = ImGui::GetWindowSize();
		ImGui::Image(m_SceneTexture, ImVec2(1280 / 2, 720 / 2));

		ImGui::End();
	}

	void EditorLayer::EntitiesWindow()
	{
		ImGui::Begin("Entities");

		ImGui::End();
	}

	void EditorLayer::Details()
	{
		ImGui::Begin("Details");

		ImGui::End();
	}

	void EditorLayer::Assets()
	{
		ImGui::Begin("Assets");

		ImGui::End();
	}

	

	

}