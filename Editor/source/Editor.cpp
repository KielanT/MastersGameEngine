#include "Editor.h"
#include "imgui.h"


#include "Engine/Renderer/Renderer.h"
#include "Engine/Platform/SDLWinUtils.h"
#include "Engine/Renderer/DirectX11/DX11Renderer.h"
#include "Engine/Data/Serializer.h"

namespace Engine
{
	Editor::Editor()
	{
		MainPath = std::filesystem::current_path().parent_path().append("Editor");
		std::filesystem::path Resource = MainPath.string() + "\\Resources"; 

		m_FileIcon = Texture2D::Create(Resource.string() + "/icons/icons8-file-150.png");
		m_FolderIcon = Texture2D::Create(Resource.string() + "/icons/icons8-folder-150.png");
		
		m_Scene = std::make_shared<Scene>();
		m_Scene->InitScene();
		Renderer::SetScene(m_Scene);
		m_CurrentSceneName = m_Scene->GetSceneSettings().title;
		Engine::SceneSettings settings = m_Scene->GetSceneSettings();
		settings.assetFilePath = MainPath.string() + "\\Assets";
		m_Scene->SetSceneSettings(settings);

		std::string path = settings.assetFilePath.string();
		SceneOrderSerilizer::DeserializeSceneOrder(path, m_SceneOrder);

		if (m_SceneOrder.assetFilePath.empty())
			m_SceneOrder.assetFilePath = settings.assetFilePath.string() + "\\Scenes";
	}

	bool Editor::Init()
	{

		
		return true;
	}

	void Editor::Render()
	{
		m_Scene->RenderScene();
		DockSpace();

		if(bShowEntitesWindow)
			EntitiesWindow(&bShowEntitesWindow);

		if(bShowGameWindow)
			GameWindow(&bShowGameWindow);

		if(bShowDetailsWindow)
			Details(&bShowDetailsWindow);

		if(bShowAssetsWindow)
			Assets(&bShowAssetsWindow);

		if (bShowSceneSettingsWindow)
			SceneSettings(&bShowSceneSettingsWindow);

		if(bShowPreferencesWindow)
			Preferences(&bShowPreferencesWindow);

		//ImGui::ShowDemoWindow();
	}

	void Editor::Update(float frameTime)
	{
		m_Scene->UpdateScene(frameTime);

		if (bGamePlay)
		{
			m_Scene->SimulateScene(frameTime);
		}
	}

	void Editor::DockSpace()
	{
		//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

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

	void Editor::MainMenuBar()
	{

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene"))
				{
					if (bUnsaved)
					{
						// Save before changing
						bIsSaveNeeded = true;
					}
					else
					{
						m_Scene->UnloadScene();
						m_SelectedEntity = {};
						m_CurrentSceneName = m_Scene->GetSceneSettings().title;
						m_SceneFilePath = "";

						Engine::SceneSettings settings = m_Scene->GetSceneSettings();
						settings.assetFilePath = MainPath.string() + "\\Assets";
						m_Scene->SetSceneSettings(settings);

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
					std::string path = FileDialog::OpenFile(Renderer::GetWindowProperties().Hwnd, "MGE Scene\0*.mge\0");
					if (!path.empty())
					{
						m_Scene->UnloadScene();
						m_SelectedEntity = {};

						SceneSerializer::DeserializeScene(path, m_Scene);
						m_SceneFilePath = path;
						m_CurrentSceneName = m_Scene->GetSceneSettings().title;
						if (m_Scene != nullptr)
						{
							m_Scene->LoadEntities();
						}
					}
				}
				ImGui::EndMenu();
			}
			
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Preferences"))
				{
					bShowPreferencesWindow = true;
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
			ImGui::Text("Save the Current Scene");
			if (ImGui::Button("Save", ImVec2(120, 0)))
			{
				Save();
				m_Scene->UnloadScene();
				m_SelectedEntity = {};
				m_CurrentSceneName = m_Scene->GetSceneSettings().title;
				m_SceneFilePath = "";
				bUnsaved = true;

				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			
			bIsSaveNeeded = false;

			ImGui::EndPopup();
		}
	}

	void Editor::GameWindow(bool* pOpen)
	{
		ImGui::Begin("Game", pOpen);

		if(ImGui::Button("Play"))
		{
			bGamePlay = !bGamePlay;
		}

		ImGui::Separator();

		if (Renderer::GetSceneTexture() != nullptr)
			ImGui::Image(Renderer::GetSceneTexture(), ImVec2(1600 / 2, 900 / 2));


		ImGui::End();
	}

	void Editor::EntitiesWindow(bool* pOpen)
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

	void Editor::Details(bool* pOpen)
	{
		ImGui::Begin("Details", pOpen);

		if (m_Scene != nullptr)
		{
			if (m_SelectedEntity)
			{
				DrawComponents();

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
					if (ImGui::MenuItem("PhysicsComponent"))
					{
						m_SelectedEntity.AddComponent<PhysicsComponents>();
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

				/*if (ImGui::MenuItem("CameraComponent"))
					m_SelectedEntity.AddComponent<CameraComponent>();*/
			}

		}

		ImGui::End();
	}

	void Editor::Assets(bool* pOpen)
	{
		ImGui::Begin("Assets", pOpen);
		static std::filesystem::path currentPath = m_Scene->GetSceneSettings().assetFilePath;
		ImGui::Text("Directory: "); ImGui::SameLine(); ImGui::Text(currentPath.string().c_str());

		ImGui::Separator();

		if (ImGui::Button("Back") && currentPath != m_Scene->GetSceneSettings().assetFilePath)
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

	void Editor::SceneSettings(bool* pOpen)
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
		if (ImGui::TreeNodeEx("Scene Order", flags))
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
				std::string path = m_Scene->GetSceneSettings().assetFilePath.string();
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
				std::string path = m_Scene->GetSceneSettings().assetFilePath.string();
				SceneOrderSerilizer::SerializeSceneOrder(path, m_SceneOrder);
			}


			ImGui::TreePop();
		}

		
		ImGui::End();
	}

	void Editor::Preferences(bool* pOpen)
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse;

		ImGui::SetWindowFocus("Preferences");
		ImGui::Begin("Preferences", pOpen, window_flags);
		
		
		char aBuffer[256];
		memset(aBuffer, 0, sizeof(aBuffer));
		strncpy_s(aBuffer, m_Scene->GetSceneSettings().assetFilePath.string().c_str(), sizeof(aBuffer));
		if (IMGUI_LEFT_LABEL(ImGui::InputText, "Asset Path: ", aBuffer, sizeof(aBuffer)))
		{
			Engine::SceneSettings settings = m_Scene->GetSceneSettings();
			settings.assetFilePath = std::string(aBuffer);
			m_Scene->SetSceneSettings(settings);
		}

		char sBuffer[256];
		memset(sBuffer, 0, sizeof(sBuffer));
		strncpy_s(sBuffer, m_SceneOrder.assetFilePath.c_str(), sizeof(sBuffer));
		if (IMGUI_LEFT_LABEL(ImGui::InputText, "Scene File Locations: ", sBuffer, sizeof(sBuffer)))
		{
			m_SceneOrder.assetFilePath = std::string(sBuffer);
		}

		ImGui::End();
	}

	void Editor::EntityNode(Entity entity)
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
				m_Scene->DeleteEntity(entity);
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
	
	void Editor::DrawComponents()
	{
		if (m_SelectedEntity)
		{
			if (m_SelectedEntity.HasComponent<IDComponent>())
			{
				DrawIDComponent(m_SelectedEntity.GetComponent<IDComponent>());
				ImGui::Separator();
			}

			if (m_SelectedEntity.HasComponent<TransformComponent>())
			{
				DrawTransformComponent(m_SelectedEntity.GetComponent<TransformComponent>());
				ImGui::Separator();
			}
			if (m_SelectedEntity.HasComponent<MeshRendererComponent>())
			{
				DrawMeshRendererComponent(m_SelectedEntity.GetComponent<MeshRendererComponent>());
				ImGui::Separator();
			}
			if (m_SelectedEntity.HasComponent<TextureComponent>())
			{
				DrawTextureComponent(m_SelectedEntity.GetComponent<TextureComponent>());
				ImGui::Separator();
			}
			if (m_SelectedEntity.HasComponent<CameraComponent>())
			{
				DrawCameraComponent(m_SelectedEntity.GetComponent<CameraComponent>());
				ImGui::Separator();
			}
			if (m_SelectedEntity.HasComponent<PhysicsComponents>())
			{
				DrawPhysicsComponent(m_SelectedEntity.GetComponent<PhysicsComponents>());
				ImGui::Separator();
			}
			if (m_SelectedEntity.HasComponent<CollisionComponents>())
			{
				DrawCollisionComponent(m_SelectedEntity.GetComponent<CollisionComponents>());
				ImGui::Separator();
			}
			if (m_SelectedEntity.HasComponent<ScriptComponent>())
			{
				DrawScriptComponent(m_SelectedEntity.GetComponent<ScriptComponent>());
				ImGui::Separator();
			}
		}
	}

	void Editor::DrawIDComponent(IDComponent& comp)
	{
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, comp.Tag.c_str(), sizeof(buffer));
		if (IMGUI_LEFT_LABEL(ImGui::InputText, "Name: ", buffer, sizeof(buffer)))
		{
			comp.Tag = std::string(buffer);
		}
	}

	void Editor::DrawTransformComponent(TransformComponent& comp)
	{
		float width = 50.0f;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
		if (ImGui::TreeNodeEx("Transform", flags))
		{
			ImGui::Text("Position: "); ImGui::SameLine();
			ImGui::PushItemWidth(width);
			ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##px", &comp.Position.x); ImGui::SameLine();
			ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##pz", &comp.Position.y); ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##py", &comp.Position.z);
			ImGui::PopItemWidth();


			ImGui::Text("Rotation: "); ImGui::SameLine();
			ImGui::PushItemWidth(width);
			ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##rx", &comp.Rotation.x); ImGui::SameLine();
			ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##rz", &comp.Rotation.y); ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##ry", &comp.Rotation.z);
			ImGui::PopItemWidth();


			ImGui::Text("   Scale: "); ImGui::SameLine();
			ImGui::PushItemWidth(width);
			ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##sx", &comp.Scale.x); ImGui::SameLine();
			ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##sz", &comp.Scale.y); ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##sy", &comp.Scale.z);
			ImGui::PopItemWidth();

			ImGui::TreePop();
		}
	}

	void Editor::DrawMeshRendererComponent(MeshRendererComponent& comp)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
		if (ImGui::TreeNodeEx("Mesh Renderer", flags))
		{
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy_s(buffer, comp.Path.c_str(), sizeof(buffer));
			ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
			IMGUI_LEFT_LABEL(ImGui::InputText, "File Path: ", buffer, sizeof(buffer), flags);

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AssetPayload"))
				{
					const wchar_t* pathp = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath = pathp;

					std::string last = comp.Path;
					comp.Path = texturePath.generic_string();

					std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(comp.Path);
					comp.Model = std::make_shared<Model>(mesh);
					bUnsaved = true;

				}
				ImGui::EndDragDropTarget();
			}

			ImGui::SameLine();

			if (ImGui::Button("Add##Model"))
			{
				const char* filter = "Select Type\0*.*\0PNG\0*.PNG\0JPG\0*.JPG\0JPEG\0*.JPEG\0DDS\0*.DDS\0";
				std::string file = FileDialog::OpenFile(Renderer::GetWindowProperties().Hwnd, filter);

				if (!file.empty())
				{
					std::string last = comp.Path;
					comp.Path = file;


					std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(comp.Path);
					comp.Model = std::make_shared<Model>(mesh);
					bUnsaved = true;
				}
			}

			static int ps = 0;
			const char* pixelItems[static_cast<int>(EPixelShader::EPixelShaderSize)];
			pixelItems[0] = "PixelLightingPixelShader";
			pixelItems[1] = "LightModelPixelShader";
			pixelItems[2] = "PBRPixelShader";
			comp.PixelShader = static_cast<EPixelShader>(RendererComboBox("Pixel Shader: ", pixelItems, static_cast<int>(EPixelShader::EPixelShaderSize), ps));


			static int vs = 0;
			const char* vertexItems[static_cast<int>(EVertexShader::EVertexShaderSize)];
			vertexItems[0] = "PixelLightingVertexShader";
			vertexItems[1] = "BasicTransformVertexShader";
			vertexItems[2] = "SkinningVertexShader";
			vertexItems[3] = "PBRVertexShader";
			comp.VertexShader = static_cast<EVertexShader>(RendererComboBox("Vertex Shader: ", vertexItems, static_cast<int>(EVertexShader::EVertexShaderSize), vs));

			static int bs = 0;
			const char* blendItems[static_cast<int>(EBlendState::EBlendStateSize)];
			blendItems[0] = "NoBlendingState";
			blendItems[1] = "AdditiveBlending";
			comp.BlendState = static_cast<EBlendState>(RendererComboBox("Blend State: ", blendItems, static_cast<int>(EBlendState::EBlendStateSize), bs));


			static int dss = 0;
			const char* dssItems[static_cast<int>(EDepthStencilState::EDepthStencilStateSize)];
			dssItems[0] = "UseDepthBufferState";
			dssItems[1] = "DepthReadOnlyState";
			dssItems[2] = "NoDepthBufferState";
			comp.DepthStencil = static_cast<EDepthStencilState>(RendererComboBox("Depth Stencil State: ", dssItems, static_cast<int>(EDepthStencilState::EDepthStencilStateSize), dss));

			static int rs = 0;
			const char* rasterizerItems[static_cast<int>(ERasterizerState::ERasterizerStateSize)];
			rasterizerItems[0] = "CullBackState";
			rasterizerItems[1] = "CullFrontState";
			rasterizerItems[2] = "CullNoneState";
			comp.RasterizerState = static_cast<ERasterizerState>(RendererComboBox("Rasterizer State: ", rasterizerItems, static_cast<int>(ERasterizerState::ERasterizerStateSize), rs));

			static int ss = 0;
			const char* samplerItems[static_cast<int>(ESamplerState::ESamplerStateSize)];
			samplerItems[0] = "Anisotropic4xSampler";
			samplerItems[1] = "TrilinearSampler";
			samplerItems[2] = "PointSampler";
			comp.SamplerState = static_cast<ESamplerState>(RendererComboBox("Sampler State: ", samplerItems, static_cast<int>(ESamplerState::ESamplerStateSize), ss));


			ImGui::TreePop();
		}

		if (comp.PixelShader == EPixelShader::PBRPixelShader || comp.VertexShader == EVertexShader::PBRVertexShader)
			bIsPBR = true;
		else
			bIsPBR = false;
	}

	void Editor::DrawTextureComponent(TextureComponent& comp)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::TreeNodeEx("Texture", flags))
		{
			if (!bIsPBR)
				TextureBoxes("Texture", comp.Path, comp.ResourceView);
			else
			{
				TextureBoxes("Albedo", comp.Path, comp.ResourceView);
				TextureBoxes("Roughness", comp.RoughPath, comp.RoughView);
				TextureBoxes("Normal", comp.NormalPath, comp.NormalView);
				TextureBoxes("Height", comp.HeightPath, comp.HeightView);
				TextureBoxes("Metalness", comp.MetalnessPath, comp.MetalnessView);
			}
			ImGui::TreePop();
		}

	}

	void Editor::DrawCameraComponent(CameraComponent& comp)
	{
		ImGui::Text("Camera Component : NOT IMPLEMENTED");
	}

	void Editor::DrawPhysicsComponent(PhysicsComponents& comp)
	{

		ImGui::Text("Physics Component : NOT IMPLEMENTED");
	}

	void Editor::DrawCollisionComponent(CollisionComponents& comp)
	{
		ImGui::Text("Collision Component : NOT IMPLEMENTED");
	}

	void Editor::DrawScriptComponent(ScriptComponent& comp)
	{
		ImGui::Text("Script Component : NOT IMPLEMENTED");
	}

	int Editor::RendererComboBox(const std::string& label, const char* items[], int size, int& selected)
	{
		const char* combo_preview_value = items[selected];
		ImGui::PushItemWidth(310);

		ImGui::Text(label.c_str()); ImGui::SameLine();

		std::string l = "##" + label;
		if (ImGui::BeginCombo(l.c_str(), combo_preview_value))
		{
			for (int n = 0; n < size; n++)
			{
				const bool is_selected = (selected == n);
				if (ImGui::Selectable(items[n], is_selected))
					selected = n;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		return selected;
	}

	void Editor::TextureBoxes(std::string Label, std::string& path, CComPtr<ID3D11ShaderResourceView>& resourseView)
	{
		std::shared_ptr<DX11Renderer> dx11Render = std::static_pointer_cast<DX11Renderer>(Renderer::GetRendererAPI());

		std::string label = Label;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, path.c_str(), sizeof(buffer));

		std::string textLabel = "##" + Label + "text";
		ImGui::Text(Label.c_str()); ImGui::SameLine();
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
		ImGui::InputText(textLabel.c_str(), buffer, sizeof(buffer), flags);
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AssetPayload"))
			{
				const wchar_t* pathp = (const wchar_t*)payload->Data;
				std::filesystem::path texturePath = pathp;

				std::string last = path;
				path = texturePath.generic_string();

				CComPtr<ID3D11Resource> Resource;
				CComPtr<ID3D11ShaderResourceView> ResourceView;

				if (dx11Render->LoadTexture(path, &Resource, &ResourceView))
				{
					resourseView = ResourceView;
					bUnsaved = true;
				}
				else
				{
					path = last;
					//TODO: Error Pop up
				}
			}
			ImGui::EndDragDropTarget();
		}


		ImGui::SameLine();
		std::string btnLabel = "Add##" + Label + "texture";
		if (ImGui::Button(btnLabel.c_str()))
		{
			const char* filter = "Select Type\0*.*\0PNG\0*.PNG\0JPG\0*.JPG\0JPEG\0*.JPEG\0DDS\0*.DDS\0";
			std::string file = FileDialog::OpenFile(dx11Render->GetWindowProperties().Hwnd, filter);

			if (!file.empty())
			{
				std::string last = path;
				path = file;
				CComPtr<ID3D11Resource> Resource;
				CComPtr<ID3D11ShaderResourceView> ResourceView;

				if (dx11Render->LoadTexture(path, &Resource, &ResourceView))
				{
					resourseView = ResourceView;
					bUnsaved = true;
				}
				else
				{
					path = last;
					//TODO: Error Pop up
				}
			}
		}

	}
	
	void Editor::Save()
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

	void Editor::SaveAs()
	{
		// Save As
		std::string path = FileDialog::SaveFile(Renderer::GetWindowProperties().Hwnd, "MGE Scene (*.mge)\0*.mge\0");

		if (!path.empty())
		{
			SceneSerializer::SerializeScene(path, m_Scene);
			bUnsaved = false;
			m_SceneFilePath = path;
			m_CurrentSceneName = m_Scene->GetSceneSettings().title;

		}
	}
}