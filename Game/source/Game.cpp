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
	}

	bool Game::Init()
	{
		std::filesystem::path SystemPath = std::filesystem::current_path();
		std::filesystem::path AssetPath = SystemPath.parent_path().string() + "\\Assets";
		std::string path = AssetPath.string() + "\\Scenes" + "\\" + "PBRScene" + ".mge";

		SceneSerializer::DeserializeScene(path, m_Scene);
		m_Scene->InitScene();
		Renderer::SetScene(m_Scene);
		m_Scene->LoadEntities(AssetPath.string());
		m_Scene->BeginScene();

		return true;
	}

	void Game::Render()
	{
		m_Scene->RenderScene();
	}

	void Game::Update(float frameTime)
	{
		m_Scene->UpdateScene(frameTime);
		m_Scene->SimulateScene(frameTime);
	}

}