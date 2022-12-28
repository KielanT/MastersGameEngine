 #include "epch.h"
#include <filesystem>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Engine/UI/EditorLayer.h"
#include "Engine/Lab/GraphicsHelpers.h"
#include "Engine/Platform/SDLWinUtils.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/DirectX11/DX11Renderer.h"
#include "Engine/SceneSystem/Scenes/SceneSerializer.h"

namespace Engine
{
	void EditorLayer::RenderGUI()
	{
		DockSpace();
		//MainWindow();
		GameWindow();
		EntitiesWindow();
		Details();
		Assets();
		
		//ImGui::ShowDemoWindow();

		
	}
	
	void EditorLayer::DockSpace()
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	}

	void EditorLayer::MainWindow()
	{
		ImGui::Begin("Main");

		ImGui::End();
	}
	
	void EditorLayer::GameWindow()
	{
		ImGui::Begin("Game");
		//ImVec2 size = ImGui::GetWindowSize();
		
		if(m_SceneTexture)
			ImGui::Image(m_SceneTexture, ImVec2(1600 / 2, 900 / 2));

		ImGui::End();
	}


	void EditorLayer::EntitiesWindow()
	{
		ImGui::Begin("Entities");
		
		bool btn = ImGui::Button("Add");
		if (btn || (ImGui::IsMouseDown(1) && ImGui::IsWindowHovered()))
		{
			ImGui::OpenPopup("EntityPopup");
		}

		if (ImGui::BeginPopup("EntityPopup"))
		{
			//std::shared_ptr<TestScene> scene = std::static_pointer_cast<TestScene>(m_Scene);
			if (ImGui::MenuItem("Create Empty Entity"))
				m_SelectedEntity = m_Scene->CreateEntity("Empty Entity");
			if (ImGui::MenuItem("Create Mesh Entity"))
				m_SelectedEntity = m_Scene->CreateMeshEntity("Mesh Entity");

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


		if (ImGui::Button("Save"))
		{
			std::string testPath = "W:/Uni/Masters/CO4305/MastersGameEngine/test.txt";
			SceneSerializer::SerializeScene(testPath, m_Scene);
		}
		if (ImGui::Button("Load"))
		{
			std::string testPath = "W:/Uni/Masters/CO4305/MastersGameEngine/test.txt";
			SceneSerializer::DeserializeScene(testPath, m_Scene);
		}

		ImGui::End();
	}

	void EditorLayer::Details()
	{
		ImGui::Begin("Details");

		if (m_Scene != nullptr)
		{
			if (m_SelectedEntity)
			{
				DrawComponents();


				ImGui::Separator();

				if (ImGui::Button("Add Component"))
				{
					ImGui::OpenPopup("AddComp");
				}

				if (ImGui::BeginPopup("AddComp"))
				{
					if (ImGui::MenuItem("MeshRenderer"))
						m_SelectedEntity.AddComponent<MeshRendererComponent>();
					if (ImGui::MenuItem("TextureComponent"))
						m_SelectedEntity.AddComponent<TextureComponent>();
					if (ImGui::MenuItem("CameraComponent"))
						m_SelectedEntity.AddComponent<CameraComponent>();
					if (ImGui::MenuItem("PhysicsComponent"))
						m_SelectedEntity.AddComponent<PhysicsComponents>();
					if (ImGui::MenuItem("CollisionComponent"))
						m_SelectedEntity.AddComponent<CollisionComponents>();
					if (ImGui::MenuItem("ScriptComponent"))
						m_SelectedEntity.AddComponent<ScriptComponent>();

					ImGui::EndPopup();
				}

				/*if (ImGui::MenuItem("CameraComponent"))
					m_SelectedEntity.AddComponent<CameraComponent>();*/
			}

		}

		ImGui::End();
	}

	void EditorLayer::Assets()
	{
		ImGui::Begin("Assets");
		static std::filesystem::path currentPath = std::filesystem::current_path();

		if (ImGui::Button("Back"))
		{
			currentPath = currentPath.parent_path();
		}

		static bool test = true;
		if (test)
		{
			m_FileIcon = Texture2D::Create("W:/Uni/Masters/CO4305/MastersGameEngine/Engine/media/icons/icons8-file-150.png");
			m_FolderIcon = Texture2D::Create("W:/Uni/Masters/CO4305/MastersGameEngine/Engine/media/icons/icons8-folder-150.png");
			test = false;
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

	void EditorLayer::EntityNode(Entity entity)
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
			}
			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}

	void EditorLayer::DrawComponents()
	{
		if (m_SelectedEntity)
		{
			if (m_SelectedEntity.HasComponent<IDComponent>())
				DrawIDComponent(m_SelectedEntity.GetComponent<IDComponent>()); 	ImGui::Separator();



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

	void EditorLayer::DrawIDComponent(IDComponent& comp)
	{
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, comp.Tag.c_str(), sizeof(buffer));
		if (IMGUI_LEFT_LABEL(ImGui::InputText, "Name: ", buffer, sizeof(buffer)))
		{
			comp.Tag = std::string(buffer);
		}
	}

	void EditorLayer::DrawTransformComponent(TransformComponent& comp)
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

	void EditorLayer::DrawMeshRendererComponent(MeshRendererComponent& comp)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
		if (ImGui::TreeNodeEx("Mesh Renderer", flags))
		{
			std::shared_ptr<DX11Renderer> dx11Render = std::static_pointer_cast<DX11Renderer>(Renderer::GetRendererAPI());

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
				
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::SameLine();

			if (ImGui::Button("Add##Model"))
			{
				const char* filter = "Select Type\0*.*\0PNG\0*.PNG\0JPG\0*.JPG\0JPEG\0*.JPEG\0DDS\0*.DDS\0";
				std::string file = OpenFile(Renderer::GetWindowProperties().Hwnd, filter);

				if (!file.empty())
				{
					std::string last = comp.Path;
					comp.Path = file;

					
					std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(comp.Path);
					comp.Model = std::make_shared<Model>(mesh);
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
			IsPBR = true;
		else
			IsPBR = false;
	}

	void EditorLayer::DrawTextureComponent(TextureComponent& comp)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
		
		if (ImGui::TreeNodeEx("Texture", flags))
		{
			if(!IsPBR)
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

	void EditorLayer::DrawCameraComponent(CameraComponent& comp)
	{
		ImGui::Text("Camera Component : NOT IMPLEMENTED");
	}

	void EditorLayer::DrawPhysicsComponent(PhysicsComponents& comp)
	{
		ImGui::Text("Physics Component : NOT IMPLEMENTED");
	}

	void EditorLayer::DrawCollisionComponent(CollisionComponents& comp)
	{
		ImGui::Text("Collision Component : NOT IMPLEMENTED");
	}

	void EditorLayer::DrawScriptComponent(ScriptComponent& comp)
	{
		ImGui::Text("Script Component : NOT IMPLEMENTED");
	}

	int EditorLayer::RendererComboBox(const std::string& label, const char* items[], int size, int& selected)
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

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		return selected;
		
	}

	void EditorLayer::TextureBoxes(std::string Label, std::string& path, CComPtr<ID3D11ShaderResourceView>& resourseView)
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
			std::string file = OpenFile(dx11Render->GetWindowProperties().Hwnd, filter);

			if (!file.empty())
			{
				std::string last = path;
				path = file;
				CComPtr<ID3D11Resource> Resource;
				CComPtr<ID3D11ShaderResourceView> ResourceView;

				if (dx11Render->LoadTexture(path, &Resource, &ResourceView))
				{
					resourseView = ResourceView;
				}
				else
				{
					path = last;
					//TODO: Error Pop up
				}
			}
		}
		
	}


}