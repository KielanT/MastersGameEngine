 #include "epch.h"
#include "EditorLayer.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "SceneSystem/Scenes/TestScene.h"

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
		ImGui::ShowDemoWindow();

		
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
		if (btn)
		{
			TestScene* scene = static_cast<TestScene*>(m_Scene);
			m_SelectedEntity = scene->CreateEntity("Empty Entity"); 

			
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
			IMGUI_LEFT_LABEL(ImGui::InputFloat, "X:", &comp.Position.x);  ImGui::SameLine();
			IMGUI_LEFT_LABEL(ImGui::InputFloat, "Y:", &comp.Position.y);  ImGui::SameLine();
			IMGUI_LEFT_LABEL(ImGui::InputFloat, "Z:", &comp.Position.z); 
			ImGui::PopItemWidth();


			ImGui::Text("Rotation: "); ImGui::SameLine();
			ImGui::PushItemWidth(width);
			IMGUI_LEFT_LABEL(ImGui::InputFloat, "X:", &comp.Rotation.x);  ImGui::SameLine();
			IMGUI_LEFT_LABEL(ImGui::InputFloat, "Y:", &comp.Rotation.y);  ImGui::SameLine();
			IMGUI_LEFT_LABEL(ImGui::InputFloat, "Z:", &comp.Rotation.z);
			ImGui::PopItemWidth();


			ImGui::Text("   Scale: "); ImGui::SameLine();
			ImGui::PushItemWidth(width);
			IMGUI_LEFT_LABEL(ImGui::InputFloat, "X:", &comp.Scale.x);  ImGui::SameLine();
			IMGUI_LEFT_LABEL(ImGui::InputFloat, "Y:", &comp.Scale.y);  ImGui::SameLine();
			IMGUI_LEFT_LABEL(ImGui::InputFloat, "Z:", &comp.Scale.z);
			ImGui::PopItemWidth();

			ImGui::TreePop();
		}
			
	}


	
	

}