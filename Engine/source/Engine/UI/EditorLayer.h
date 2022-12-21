#pragma once
#include "Engine/Interfaces/IRenderer.h"
#include "Engine/Interfaces/IScene.h"

#include "Engine/SceneSystem/Scenes/Entity.h"
#include "Engine/Interfaces/ITexture.h"

#define IMGUI_LEFT_LABEL(func, label, ...) (ImGui::TextUnformatted(label), ImGui::SameLine(), func("##" label, __VA_ARGS__))

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
	

		void Update();
		void RenderGUI();
		
		void SetSceneTexture(CComPtr<ID3D11ShaderResourceView> sceneTexture) { m_SceneTexture = sceneTexture; }
		void SetScene(std::shared_ptr<IScene> scene) { m_Scene = scene; }
		
	private:
		
		void DockSpace();
		void MainWindow();
		void GameWindow();
		void EntitiesWindow();
		void Details();
		void Assets();

		void EntityNode(Entity entity);
		void DrawComponents();

		void DrawIDComponent(IDComponent& comp);
		void DrawTransformComponent(TransformComponent& comp);
		void DrawMeshRendererComponent(MeshRendererComponent& comp);
		void DrawTextureComponent(TextureComponent& comp);

		int RendererComboBox(const std::string& label, const char* items[], int size, int& selected);
		void TextureBoxes(std::string Label, std::string& path, CComPtr<ID3D11ShaderResourceView>& resourseView);

	private:
		CComPtr<ID3D11ShaderResourceView> m_SceneTexture;
		std::shared_ptr<IScene> m_Scene = nullptr;

		Entity m_SelectedEntity;

	private:
		bool IsPBR = false;

		std::shared_ptr<Texture2D> m_FileIcon;
		std::shared_ptr<Texture2D> m_FolderIcon;

	private:
		std::vector<std::string> m_CurrentResources; // Create a struct/object that stores the resources

	};
}
