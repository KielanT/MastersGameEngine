#include "Game.h"
#include "imgui.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Platform/SDLWinUtils.h"
#include "Engine/Data/Serializer.h"

namespace Engine
{
	Game::Game()
	{
		m_Scene = std::make_shared<Scene>();
		m_Scene->InitScene();
		Renderer::SetScene(m_Scene);
	}

	bool Game::Init()
	{
		return true;
	}

	void Game::Render()
	{
		m_Scene->RenderScene();
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration;
		bool open = true;
		ImGui::Begin("Temp Scene Loader", &open, window_flags);
		if (ImGui::Button("Open Scene"))
		{
			std::string path = FileDialog::OpenFile(Renderer::GetWindowProperties().Hwnd, "MGE Scene\0*.mge\0");
			if (!path.empty())
			{
				m_Scene->UnloadScene();

				SceneSerializer::DeserializeScene(path, m_Scene);
				m_Scene->LoadEntities();
			}
		}

		ImGui::End();
	}

	void Game::Update(float frameTime)
	{
		m_Scene->UpdateScene(frameTime);
	}

}