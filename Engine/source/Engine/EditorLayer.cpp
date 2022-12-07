 #include "epch.h"
#include "EditorLayer.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "SceneSystem/Scenes/TestScene.h"
#include "Engine/Lab/GraphicsHelpers.h"
#include "Engine/Platform/SDLWinUtils.h"

namespace Engine
{
	
	void EditorLayer::Update()
	{
		ImGuiIO& io = ImGui::GetIO();
		
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}
	
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
			std::shared_ptr<TestScene> scene = std::static_pointer_cast<TestScene>(m_Scene);
			if (ImGui::MenuItem("Create Empty Entity"))
				m_SelectedEntity = scene->CreateEntity("Empty Entity");
			if (ImGui::MenuItem("Create Mesh Entity"))
				m_SelectedEntity = scene->CreateMeshEntity("Mesh Entity");

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

	void EditorLayer::Details()
	{
		ImGui::Begin("Details");



		if (m_Scene != nullptr)
		{
			if (m_SelectedEntity)
			{
				DrawComponents();
			}
		}

		ImGui::End();
	}

	void EditorLayer::Assets()
	{
		ImGui::Begin("Assets");

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
			{
				DrawIDComponent(m_SelectedEntity.GetComponent<IDComponent>());
			}
			if (m_SelectedEntity.HasComponent<TransformComponent>())
			{
				DrawTransformComponent(m_SelectedEntity.GetComponent<TransformComponent>());
			}
			if (m_SelectedEntity.HasComponent<MeshRendererComponent>())
			{
				DrawMeshRendererComponent(m_SelectedEntity.GetComponent<MeshRendererComponent>());
			}
			if (m_SelectedEntity.HasComponent<TextureComponent>())
			{
				DrawTextureComponent(m_SelectedEntity.GetComponent<TextureComponent>());
			}
		}
	}

	void EditorLayer::DrawIDComponent(IDComponent& comp)
	{
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, comp.Tag.c_str(), sizeof(buffer));
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
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, comp.Path.c_str(), sizeof(buffer));
			ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
			IMGUI_LEFT_LABEL(ImGui::InputText, "File Path: ", buffer, sizeof(buffer), flags);

			ImGui::SameLine();

			if (ImGui::Button("Add##Model"))
			{
				const char* filter = "Select Type\0*.*\0PNG\0*.PNG\0JPG\0*.JPG\0JPEG\0*.JPEG\0DDS\0*.DDS\0";
				std::string file = OpenFile(m_Scene->GetRenderer()->GetWindowProperties().Hwnd, filter);

				if (!file.empty())
				{
					std::string last = comp.Path;
					comp.Path = file;

					std::shared_ptr<DirectX11Renderer> renderer = std::static_pointer_cast<DirectX11Renderer>(m_Scene->GetRenderer());
					std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(renderer, comp.Path);
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
			//if(!IsPBR)
			//	TextureBoxes("Texture", comp.Path, comp.ResourceView);
			//else
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
		std::string label = Label;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, path.c_str(), sizeof(buffer));

		std::string textLabel = "##" + Label + "text";
		ImGui::Text(Label.c_str()); ImGui::SameLine();
		ImGui::InputText(textLabel.c_str(), buffer, sizeof(buffer), flags);

		ImGui::SameLine();
		std::string btnLabel = "Add##" + Label + "texture";
		if (ImGui::Button(btnLabel.c_str()))
		{
			const char* filter = "Select Type\0*.*\0PNG\0*.PNG\0JPG\0*.JPG\0JPEG\0*.JPEG\0DDS\0*.DDS\0";
			std::string file = OpenFile(m_Scene->GetRenderer()->GetWindowProperties().Hwnd, filter);

			if (!file.empty())
			{
				std::string last = path;
				path = file;
				CComPtr<ID3D11Resource> Resource;
				CComPtr<ID3D11ShaderResourceView> ResourceView;

				std::shared_ptr<DirectX11Renderer> renderer = std::static_pointer_cast<DirectX11Renderer>(m_Scene->GetRenderer());
				if (LoadTexture(renderer, path, &Resource, &ResourceView))
				{
					resourseView = ResourceView;
				}
				else
				{
					path = last;
					// Error Pop up
				}
			}
		}
	}


}