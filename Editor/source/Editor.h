#pragma once
#include <memory>
#include <filesystem>

#include <Engine.h>

#define IMGUI_LEFT_LABEL(func, label, ...) (ImGui::TextUnformatted(label), ImGui::SameLine(), func("##" label, __VA_ARGS__))

namespace Engine
{

	class Editor : public ILayer
	{
	public:
		Editor();

		virtual bool Init() override;
		virtual void Render() override;
		virtual void Update(float frameTime) override;

	private:
		void DockSpace();
		void MainMenuBar();
		void GameWindow(bool* pOpen);
		void EntitiesWindow(bool* pOpen);
		void Details(bool* pOpen);
		void Assets(bool* pOpen);
		void SceneSettings(bool* pOpen);
		void Preferences(bool* pOpen);

		void EntityNode(Entity entity);
		void DrawComponents();

		void DrawIDComponent(IDComponent& comp);
		void DrawTransformComponent(TransformComponent& comp);
		void DrawMeshRendererComponent(MeshRendererComponent& comp);
		void DrawTextureComponent(TextureComponent& comp);
		void DrawCameraComponent(CameraComponent& comp);
		void DrawPhysicsComponent(PhysicsComponents& comp);
		void DrawCollisionComponent(CollisionComponents& comp);
		void DrawScriptComponent(ScriptComponent& comp);

		int RendererComboBox(const std::string& label, const char* items[], int size, int& selected);
		void TextureBoxes(std::string Label, std::string& path, CComPtr<ID3D11ShaderResourceView>& resourseView);
		void Save();
		void SaveAs();

	private:
		bool bShowGameWindow = true;
		bool bShowEntitesWindow = true;
		bool bShowDetailsWindow = true;
		bool bShowAssetsWindow = true;
		bool bShowSceneSettingsWindow = true;
		
		bool bShowPreferencesWindow = false;
		
		bool bGamePlay = false;
	private:
		Entity m_SelectedEntity;
		bool bUnsaved = false;
		bool bIsPBR = false;
		bool bIsSaveNeeded = false;
		std::string m_SceneFilePath = "";
		std::string m_CurrentSceneName = "";
		std::filesystem::path MainPath;

		std::shared_ptr<Engine::Texture2D> m_FileIcon;
		std::shared_ptr<Engine::Texture2D> m_FolderIcon;
		std::shared_ptr<Engine::Scene> m_Scene;
		SceneOrder m_SceneOrder;

		
	};
}
