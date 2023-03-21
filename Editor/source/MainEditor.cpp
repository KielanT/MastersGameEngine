#include "MainEditor.h"
#include "imgui.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Platform/SDLWinUtils.h"
#include "Engine/Data/Serializer.h"


#include "EditorDraws.h"



namespace Engine
{
	MainEditor::MainEditor()
	{
		m_SystemPath = std::filesystem::current_path();
		std::filesystem::path Resource = std::filesystem::current_path().string() + "\\Resources";
		m_AssetPath = m_SystemPath.parent_path().string() + "\\Assets";
		
		m_FileIcon = Texture2D::Create(Resource.string() + "/icons/icons8-file-150.png");
		m_FolderIcon = Texture2D::Create(Resource.string() + "/icons/icons8-folder-150.png");

		m_PlayState = EPlaystate::NotPlaying;

		m_EditorScene = std::make_shared<Scene>();
		m_Scene = m_EditorScene;

	}

	bool MainEditor::Init()
	{
		m_EditorCamera = std::make_shared<EditorCamera>();
		m_EditorCamera->SetPosition({ 0, 0, 50.0f });
		m_EditorCamera->SetLens(0.25f * glm::pi<float>(), 1600.0f / 900.0f, 1.0f, 1000.0f);

		m_Scene = std::make_shared<Scene>();
		m_Scene->InitScene();
		m_Scene->SetCamera(m_EditorCamera);
		Renderer::SetScene(m_Scene);

		SettingsSerilizer settings;
		std::string syspath = m_SystemPath.string();
		settings.DeserializeEditorSettings(syspath, m_EditorSettings);
		if (!m_EditorSettings.m_StartUpScene.empty())
		{
			LoadScene(m_EditorSettings.m_StartUpScene);
		}

		return true;
	}

	void MainEditor::Render()
	{
		m_Scene->RenderScene();

		DockSpace();
		
		bool showWindow = true;
		
		if(bShowGameWindow)
			EntitiesWindow(&bShowGameWindow);
		
		if(bShowGameWindow)
			GameWindow(&bShowGameWindow);
		
		if(bShowDetailsWindow)
			Details(&bShowDetailsWindow);
		
		if(bShowAssetsWindow)
			Assets(&bShowAssetsWindow);
		
		if(bShowSceneSettingsWindow)
			SceneSettings(&bShowSceneSettingsWindow);
		
		if (bShowSettingsWindow)
			Settings(&bShowSettingsWindow);

		

	}

	void MainEditor::Update(float frameTime)
	{
		m_Scene->UpdateScene(frameTime);

		if (m_PlayState == EPlaystate::Playing)
		{
			m_Scene->SimulateScene(frameTime);
		}
		else
		{
			m_Scene->EditorUpdatePhysicsScene(frameTime);
		}
	}

	void MainEditor::DockSpace()
	{
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		std::string windowName = m_CurrentSceneName + "###";
		ImGui::Begin(windowName.c_str(), (bool*)0, window_flags);
		ImGui::PopStyleVar();
		MainMenuBar();

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		ImGui::End();
	}

	void MainEditor::MainMenuBar()
	{
		if (ImGui::BeginMenuBar())
		{

			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene"))
				{
					if (bUnsaved)
					{
						bIsSaveNeeded = true;
					}
					else
					{
						m_Scene->UnloadScene();
						m_SelectedEntity = {};

						m_CurrentSceneName = "";
						m_SceneFilePath = "";

						std::shared_ptr<Engine::Scene> scene = std::make_shared<Scene>();
						m_EditorScene = scene;
						m_Scene = m_EditorScene;
						m_Scene->InitScene();
						m_Scene->SetCamera(m_EditorCamera);
						Renderer::SetScene(m_Scene);
						bUnsaved = true;

					}
				}

				if (ImGui::MenuItem("Save Scene"))
				{
					Save();
				}
				if (ImGui::MenuItem("Save As Scene"))
				{
					SaveAs();
				}

				if (ImGui::MenuItem("Load Scene"))
				{
					if (bUnsaved)
					{
						bIsSaveNeeded = true;
					}
					else
					{
						LoadScene();
					}
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::MenuItem("Game", NULL, bShowGameWindow))
					bShowGameWindow = !bShowGameWindow;

				if (ImGui::MenuItem("Entities", NULL, bShowEntitesWindow))
					bShowEntitesWindow = !bShowEntitesWindow;

				if (ImGui::MenuItem("Details", NULL, bShowDetailsWindow))
					bShowDetailsWindow = !bShowDetailsWindow;

				if (ImGui::MenuItem("Assets", NULL, bShowAssetsWindow))
					bShowAssetsWindow = !bShowAssetsWindow;

				if (ImGui::MenuItem("Scene Settings", NULL, bShowSceneSettingsWindow))
					bShowSceneSettingsWindow = !bShowSceneSettingsWindow;

				if (ImGui::MenuItem("Settings", NULL, bShowSettingsWindow))
					bShowSettingsWindow = !bShowSettingsWindow;

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (bIsSaveNeeded)
		{
			ImGui::OpenPopup("Save");
		}

		if (ImGui::BeginPopupModal("Save", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Save the Current Scene First");

			if (ImGui::Button("Save", ImVec2(120, 0)))
			{
				Save();
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }

			bIsSaveNeeded = false;
			ImGui::EndPopup();
		}

	}

	void MainEditor::GameWindow(bool* pOpen)
	{
		ImGui::Begin("Game", pOpen);
		
		if (m_PlayState == EPlaystate::NotPlaying || m_PlayState == EPlaystate::Paused)
		{
			if (ImGui::Button("Play"))
			{
				m_Scene->SetActiveCamera();
				m_PlayState = EPlaystate::Playing;
				Save();
			}
		}
		else if (m_PlayState == EPlaystate::Playing)
		{
			if (ImGui::Button("Pause"))
			{
				m_PlayState = EPlaystate::Paused;
			}
		}

		if (m_PlayState == EPlaystate::Playing || m_PlayState == EPlaystate::Paused)
		{
			ImGui::SameLine();
			if (ImGui::Button("Stop"))
			{
				m_PlayState = EPlaystate::NotPlaying;
				LoadScene(true);
			
			}
		}

		if (Renderer::GetSceneTexture() != nullptr)
			ImGui::Image(Renderer::GetSceneTexture(), ImVec2(1600 / 2, 900 / 2));

		ImGui::End();
	}

	void MainEditor::EntitiesWindow(bool* pOpen)
	{
		ImGuiWindowFlags window_flags = 0;
		if (bUnsaved)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
		ImGui::Begin("Entities", pOpen, window_flags);
		

		bool btn = ImGui::Button("Add Entity");
		if (btn || (ImGui::IsMouseDown(1) && ImGui::IsWindowHovered()))
		{
			ImGui::OpenPopup("EntityPopup");
		}

		if (ImGui::BeginPopup("EntityPopup"))
		{
			if (ImGui::MenuItem("Create Camera Entity"))
			{
				m_SelectedEntity = m_Scene->CreateCameraEntity("Camera Entity");
				bUnsaved = true;
			}
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_SelectedEntity = m_Scene->CreateEntity("Empty Entity");
				bUnsaved = true;
			}
			if (ImGui::MenuItem("Create Mesh Entity"))
			{
				m_SelectedEntity = m_Scene->CreateMeshEntity("Mesh Entity");
				bUnsaved = true;
			}
			if (ImGui::MenuItem("Create Skybox Entity"))
			{
				m_SelectedEntity = m_Scene->CreateSkyboxEntity("Skybox Entity");
				bUnsaved = true;
			}

			ImGui::EndPopup();
		}

		if (m_Scene != nullptr)
		{
			m_Scene->GetEntityRegistry().each([&](auto entityID)
				{
					Entity entity{ entityID, m_Scene };
					EntityNode(entity);
				});
		}

		ImGui::End();
	}

	void MainEditor::Details(bool* pOpen)
	{
		ImGuiWindowFlags window_flags = 0;

		ImGui::Begin("Details", pOpen, window_flags);

		if (m_Scene != nullptr && m_SelectedEntity)
		{
			EditorDraws drawer;
			drawer.DrawComponents(m_SelectedEntity, m_AssetPath);

			if (drawer.bIsUnsaved)
			{
				if (bUnsaved == false)
				{
					bUnsaved = drawer.bIsUnsaved;
				}
			}

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("AddComp");
			}

			if (ImGui::BeginPopup("AddComp"))
			{
				if (ImGui::MenuItem("MeshRenderer"))
				{
					m_SelectedEntity.AddComponent<MeshRendererComponent>();
					bUnsaved = true;
				}
				if (ImGui::MenuItem("TextureComponent"))
				{
					m_SelectedEntity.AddComponent<TextureComponent>();
					bUnsaved = true;
				}
				if (ImGui::MenuItem("CameraComponent"))
				{
					m_SelectedEntity.AddComponent<CameraComponent>();
					bUnsaved = true;
				}
				if (ImGui::MenuItem("RigidDynamicComponent"))
				{
					m_SelectedEntity.AddComponent<RigidDynamicComponent>();
					bUnsaved = true;
				}
				if (ImGui::MenuItem("CollisionComponent"))
				{
					m_SelectedEntity.AddComponent<CollisionComponents>();
					bUnsaved = true;
				}
				if (ImGui::MenuItem("ScriptComponent"))
				{
					m_SelectedEntity.AddComponent<ScriptComponent>();
					bUnsaved = true;
				}

				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}

	void MainEditor::Assets(bool* pOpen)
	{
		ImGuiWindowFlags window_flags = 0;

		ImGui::Begin("Assets", pOpen, window_flags);
		static std::filesystem::path currentPath = m_AssetPath;

		ImGui::Text("Directory: "); ImGui::SameLine(); ImGui::Text(currentPath.string().c_str());
		ImGui::Separator();

		if (ImGui::Button("Back") && currentPath != m_AssetPath)
		{
			currentPath = currentPath.parent_path();
		}

		static float padding = 16.0f;
		static float thumbnailSize = 100.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto const& dir : std::filesystem::directory_iterator{ currentPath })
		{
			ImGui::PushID(dir.path().filename().string().c_str());
			std::shared_ptr<Texture2D> icon = dir.is_directory() ? m_FolderIcon : m_FileIcon;

			ImGui::ImageButton(icon->GetTexture(), { thumbnailSize , thumbnailSize });

			if (!dir.is_directory() && ImGui::BeginDragDropSource())
			{
				//auto relativePath = std::filesystem::relative(dir.path(), currentPath);
				const wchar_t* itemPath = dir.path().c_str();
				ImGui::SetDragDropPayload("AssetPayload", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (dir.is_directory())
					currentPath /= dir.path().filename();

			}

			ImGui::TextWrapped(dir.path().filename().string().c_str());

			ImGui::NextColumn();
			ImGui::PopID();
		}

		ImGui::End();
	}

	void MainEditor::SceneSettings(bool* pOpen)
	{
		ImGui::Begin("Scene Settings", pOpen);

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
		if (ImGui::TreeNodeEx("Current Scene Settings", flags))
		{
			ImGui::Text("Name: "); ImGui::SameLine(); ImGui::Text(m_Scene->GetSceneSettings().title.c_str());

			//ImGui::Separator();

			ImGui::PushItemWidth(50);
			ImGui::Text("Specular Power"); ImGui::SameLine(); ImGui::InputFloat("##Specular", &m_Scene->m_SceneSettings.specularPower);
			ImGui::PopItemWidth();

			//ImGui::Separator();

			ImGui::Text("VSync"); ImGui::SameLine(); ImGui::Checkbox("##vsync", &m_Scene->m_SceneSettings.vsyncOn);

			//ImGui::Separator();

			ImGui::Text("Ambient Colour:"); ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##abx", &m_Scene->m_SceneSettings.ambientColour.x); ImGui::SameLine();
			ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##abz", &m_Scene->m_SceneSettings.ambientColour.y); ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##aby", &m_Scene->m_SceneSettings.ambientColour.z);
			ImGui::PopItemWidth();

			//ImGui::Separator();

			ImGui::Text("Background Colour:"); ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##bgx", &m_Scene->m_SceneSettings.backgroundColour.x); ImGui::SameLine();
			ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##bgz", &m_Scene->m_SceneSettings.backgroundColour.y); ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##bgy", &m_Scene->m_SceneSettings.backgroundColour.z);  ImGui::SameLine();
			ImGui::Text("A"); ImGui::SameLine(); ImGui::InputFloat("##bga", &m_Scene->m_SceneSettings.backgroundColour.w);
			ImGui::PopItemWidth();
			ImGui::TreePop();

			if (ImGui::Button("Reset To default"))
			{
				Engine::SceneSettings defaultSettings = Engine::SceneSettings();
				m_Scene->m_SceneSettings.backgroundColour = defaultSettings.backgroundColour;
				m_Scene->m_SceneSettings.specularPower = defaultSettings.specularPower;
				m_Scene->m_SceneSettings.vsyncOn = defaultSettings.vsyncOn;
				m_Scene->m_SceneSettings.ambientColour = defaultSettings.ambientColour;
			}
		}

		ImGui::Separator();
		/*if (ImGui::TreeNodeEx("Scene Order", flags))
		{
			static int index = m_SceneOrder.sceneOrderVar.size();
			static bool HasScene = false;
			static SceneOrderVar var;
			if (ImGui::Button("Add"))
			{
				ImGui::OpenPopup("AddScene");
				var.title = "";

			}

			if (ImGui::BeginPopupModal("AddScene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Make Sure The map Name Is Correct!");

				char buff[256];
				memset(buff, 0, sizeof(buff));
				strncpy_s(buff, var.title.c_str(), sizeof(buff));
				if (IMGUI_LEFT_LABEL(ImGui::InputText, "Title: ", buff, sizeof(buff)))
				{
					var.title = std::string(buff);
				}

				if (ImGui::Button("Add Scene"))
				{
					var.index = index;
					m_SceneOrder.sceneOrderVar.push_back(var);
					index++;
					HasScene = true;
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();

				if (ImGui::Button("Cancel"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			ImGui::SameLine();

			if (HasScene && ImGui::Button("Save Order"))
			{
				std::string path = m_EditorScene->GetSceneSettings().assetFilePath.string();
				SceneOrderSerilizer::SerializeSceneOrder(path, m_SceneOrder);
			}

			if (index <= 0)
				HasScene = false;
			else
				HasScene = true;

			for (auto var : m_SceneOrder.sceneOrderVar)
			{
				ImGui::Text(var.title.c_str());
				ImGui::SameLine();
				ImGui::Text("%d", var.index);

			}

			if (HasScene && ImGui::Button("Remove"))
			{
				m_SceneOrder.sceneOrderVar.pop_back();
				index--;
				std::string path = m_EditorScene->GetSceneSettings().assetFilePath.string();
				SceneOrderSerilizer::SerializeSceneOrder(path, m_SceneOrder);
			}


			ImGui::TreePop();
		}*/


		ImGui::End();
	}

	void MainEditor::Settings(bool* pOpen)
	{
		ImGuiWindowFlags window_flags = 0;

		ImGui::Begin("Perferences", pOpen, window_flags);
		
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, m_EditorSettings.m_StartUpScene.c_str(), sizeof(buffer));
		if (IMGUI_LEFT_LABEL(ImGui::InputText, "Start Up Scene: ", buffer, sizeof(buffer)))
		{
			m_EditorSettings.m_StartUpScene = std::string(buffer);
			SettingsSerilizer settings;
			std::string syspath = m_SystemPath.string();
			settings.SerializeEditorSettings(syspath, m_EditorSettings);
		}

		ImGui::End();
	}

	void MainEditor::EntityNode(Entity entity)
	{
		auto& id = entity.GetComponent<IDComponent>().ID;
		auto& tag = entity.GetComponent<IDComponent>().Tag;


		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				m_EditorScene->DeleteEntity(entity);
				m_SelectedEntity = {}; // Clears Selected entity
				bUnsaved = true;
			}
			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}

	void MainEditor::LoadScene(bool reset)
	{
		std::string path;

		if (!reset)
		{
			path = FileDialog::OpenFile(Renderer::GetWindowProperties().Hwnd, "MGE Scene\0*.mge\0", m_AssetPath.string() + "\\Scenes", "Open Scene");
		}
		else
		{
			path = m_SceneFilePath;
		}

		

		if (!path.empty())
		{
			m_SelectedEntity = {};
			m_Scene->UnloadScene();

			std::shared_ptr<Engine::Scene> scene = std::make_shared<Scene>();
			SceneSerializer::DeserializeScene(path, scene);
			m_SceneFilePath = path;
			m_CurrentSceneName = scene->GetSceneSettings().title;

			if (scene != nullptr)
			{
				m_EditorScene = scene;
				m_Scene = m_EditorScene;
				m_Scene->InitScene();
				m_Scene->SetCamera(m_EditorCamera);
				Renderer::SetScene(m_Scene);
				m_Scene->LoadEntities(m_AssetPath.string());
			}
		}
	}

	void MainEditor::LoadScene(std::string& sceneName)
	{
		std::string path = m_AssetPath.string() + "\\Scenes" + "\\" + sceneName + ".mge";
		if (!path.empty())
		{
			m_SelectedEntity = {};
			m_Scene->UnloadScene();

			std::shared_ptr<Engine::Scene> scene = std::make_shared<Scene>();
			SceneSerializer::DeserializeScene(path, scene);
			m_SceneFilePath = path;
			m_CurrentSceneName = scene->GetSceneSettings().title;

			if (scene != nullptr)
			{
				m_EditorScene = scene;
				m_Scene = m_EditorScene;
				m_Scene->InitScene();
				m_Scene->SetCamera(m_EditorCamera);
				Renderer::SetScene(m_Scene);
				m_Scene->LoadEntities(m_AssetPath.string());
			}
		}
	}

	void MainEditor::Save()
	{
		if (!m_SceneFilePath.empty())
		{
			SceneSerializer::SerializeScene(m_SceneFilePath, m_Scene);
			bUnsaved = false;
			m_CurrentSceneName = m_Scene->GetSceneSettings().title;
		}
		else
		{
			SaveAs();
		}
	}

	void MainEditor::SaveAs()
	{
		// Save As
		std::string path = FileDialog::SaveFile(Renderer::GetWindowProperties().Hwnd, "MGE Scene (*.mge)\0*.mge\0", m_AssetPath.string() + "\\Scenes", "Save Scene");

		if (!path.empty())
		{
			SceneSerializer::SerializeScene(path, m_Scene);
			bUnsaved = false;
			m_SceneFilePath = path;
			m_CurrentSceneName = m_Scene->GetSceneSettings().title;

		}
	}
}