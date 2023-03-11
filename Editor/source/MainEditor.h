#pragma once
#include <Engine.h>


namespace Engine
{
	enum class EPlaystate
	{
		Playing = 0,
		Paused,
		NotPlaying,
	};

	

	class MainEditor : public Engine::ILayer
	{
	public:
		MainEditor();

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
		void Settings(bool* pOpen);
		
		void EntityNode(Entity entity);
		
		void LoadScene(bool reset = false);
		void LoadScene(std::string& sceneName);
		void Save();
		void SaveAs();

	private:
		std::shared_ptr<Engine::Scene> m_EditorScene;
		std::shared_ptr<Engine::Scene> m_Scene;
			
		EditorSettings m_EditorSettings;
		
		std::string m_SceneFilePath = "";
		std::string m_CurrentSceneName = "Untitled";

		Entity m_SelectedEntity;

		std::filesystem::path m_SystemPath;
		std::filesystem::path m_AssetPath;

		std::shared_ptr<Engine::Texture2D> m_FileIcon;
		std::shared_ptr<Engine::Texture2D> m_FolderIcon;

		EPlaystate m_PlayState;

		bool bUnsaved = false; 
		bool bIsSaveNeeded = false;
		
		bool bShowGameWindow = true;
		bool bShowEntitesWindow = true;
		bool bShowDetailsWindow = true;
		bool bShowAssetsWindow = true;
		bool bShowSceneSettingsWindow = false;
		bool bShowSettingsWindow = false;
	};
}