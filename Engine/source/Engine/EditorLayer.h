#pragma once
#include "Interfaces/IRenderer.h"


namespace Engine
{
	class EditorLayer
	{
	public:
		EditorLayer(CComPtr<ID3D11ShaderResourceView> sceneTexture, IRenderer* renderer);
		~EditorLayer();

		EditorLayer(const EditorLayer&) = delete;
		EditorLayer(EditorLayer&&) = delete;
		EditorLayer& operator=(const EditorLayer&) = delete;
		EditorLayer& operator=(EditorLayer&&) = delete;
	
	public:
		void Update();
		void RenderGUI();
		
		
	private:
		
		void DockSpace();
		void MainWindow();
		void GameWindow();
		void EntitiesWindow();
		void Details();
		void Assets();

	private:
		CComPtr<ID3D11ShaderResourceView> m_SceneTexture;
		IRenderer* m_Renderer;
	};
}

