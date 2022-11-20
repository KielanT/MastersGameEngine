#pragma once
#include "Interfaces/IRenderer.h"
#include "Interfaces/IScene.h"

#include "SceneSystem/Scenes/Entity.h"

namespace Engine
{
	class EditorLayer
	{
	public:
		//EditorLayer(CComPtr<ID3D11ShaderResourceView> sceneTexture, IRenderer* renderer, IScene* scene);
		//~EditorLayer();

		/*EditorLayer(const EditorLayer&) = delete;
		EditorLayer(EditorLayer&&) = delete;
		EditorLayer& operator=(const EditorLayer&) = delete;
		EditorLayer& operator=(EditorLayer&&) = delete;*/
	
	public:
		void Update();
		void RenderGUI();
		
		void SetSceneTexture(CComPtr<ID3D11ShaderResourceView> sceneTexture) { m_SceneTexture = sceneTexture; }
		void SetScene(IScene* scene) { m_Scene = scene; }
		
	private:
		
		void DockSpace();
		void MainWindow();
		void GameWindow();
		void EntitiesWindow();
		void Details();
		void Assets();

		void EntityNode(Entity entity);

	private:
		CComPtr<ID3D11ShaderResourceView> m_SceneTexture;
		IScene* m_Scene = nullptr;

		Entity m_SelectedEntity;

	};
}

