#pragma once
#include <Engine.h>
#include "EditorCamera.h"
#include "EditorDraws.h"

namespace Engine
{
	// Play state enum
	// Use to run the correct code during play or not playing
	enum class EPlaystate
	{
		Playing = 0,
		Paused,
		NotPlaying,
	};

	
	// Main Editor layer
	class MainEditor : public Engine::ILayer
	{
	public:
		MainEditor();

		virtual bool Init() override;
		virtual void Render() override;
		virtual void Update(float frameTime) override;

	private:
		// Creates the dockspace for the editor
		void DockSpace();

		// Creates the main menu bar (file and windows)
		void MainMenuBar();

		// Creates the windows and everything that should
		// be drawn in them
		void GameWindow(bool* pOpen);
		void EntitiesWindow(bool* pOpen);
		void Details(bool* pOpen);
		void Assets(bool* pOpen);
		void SceneSettings(bool* pOpen);
		void Settings(bool* pOpen);
		
		// Creates Entity Nodes 
		// for the entities panel
		void EntityNode(Entity entity);
		
		// Scene load functions
		void LoadScene(bool reset = false);
		void LoadScene(std::string& sceneName);

		// Scene saving functions
		void Save();
		void SaveAs();

	private:
		// Scene Objects
		std::shared_ptr<Engine::Scene> m_EditorScene;
		std::shared_ptr<Engine::Scene> m_Scene;
		
		// Editor camera seperate from the game camera.
		std::shared_ptr<EditorCamera> m_EditorCamera;
		
		// The settings for the editor
		// Used for setting the start up scen
		EditorSettings m_EditorSettings;
		
		// File path used for loading and reloading the scene
		std::string m_SceneFilePath = "";

		// Scene name used for displaying the scene name
		std::string m_CurrentSceneName = "Untitled";

		// Used for displaying the details of the selected entity
		Entity m_SelectedEntity;

		
		std::filesystem::path m_SystemPath;

		// Asset path for displaying the assets in editor
		std::filesystem::path m_AssetPath;

		// Texture Resources for the icons 
		std::shared_ptr<Engine::Texture2D> m_FileIcon;
		std::shared_ptr<Engine::Texture2D> m_FolderIcon;

		// Playstes variable for changine and checking the play state
		EPlaystate m_PlayState;

		// booleans for saving
		bool bUnsaved = false; 
		bool bIsSaveNeeded = false;
		
		// booleans for displaying the windows
		bool bShowGameWindow = true;
		bool bShowEntitesWindow = true;
		bool bShowDetailsWindow = true;
		bool bShowAssetsWindow = true;
		bool bShowSceneSettingsWindow = false;
		bool bShowSettingsWindow = false;

		// Used for drawing the components
		EditorDraws m_Drawers;
	};
}