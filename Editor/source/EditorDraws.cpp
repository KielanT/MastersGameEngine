#include "EditorDraws.h"
#include "imgui.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Platform/SDLWinUtils.h"
#include "Engine/Renderer/DirectX11/DX11Renderer.h"

namespace Engine
{
	void EditorDraws::DrawComponents(Entity& entity)
	{
		if (entity)
		{
			if (entity.HasComponent<IDComponent>())
			{
				DrawIDComponent(entity.GetComponent<IDComponent>());
				ImGui::Separator();
			}

			if (entity.HasComponent<TransformComponent>())
			{
				DrawTransformComponent(entity.GetComponent<TransformComponent>());
				ImGui::Separator();
			}
			if (entity.HasComponent<MeshRendererComponent>())
			{
				DrawMeshRendererComponent(entity.GetComponent<MeshRendererComponent>());
				ImGui::Separator();
			}
			if (entity.HasComponent<TextureComponent>())
			{
				DrawTextureComponent(entity.GetComponent<TextureComponent>());
				ImGui::Separator();
			}
			if (entity.HasComponent<CameraComponent>())
			{
				DrawCameraComponent(entity.GetComponent<CameraComponent>());
				ImGui::Separator();
			}
			if (entity.HasComponent<RigidDynamicComponent>())
			{
				DrawRigidDynamic(entity.GetComponent<RigidDynamicComponent>());
				ImGui::Separator();
			}
			if (entity.HasComponent<RigidStaticComponent>())
			{
				DrawStaticDynamic(entity.GetComponent<RigidStaticComponent>());
				ImGui::Separator();
			}
			if (entity.HasComponent<CollisionComponents>())
			{
				DrawCollisionComponent(entity.GetComponent<CollisionComponents>());
				ImGui::Separator();
			}
			if (entity.HasComponent<ScriptComponent>())
			{
				//DrawScriptComponent(m_SelectedEntity.GetComponent<ScriptComponent>());
				ImGui::Separator();
			}
		}
	}

	void EditorDraws::DrawIDComponent(IDComponent& comp)
	{
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, comp.Tag.c_str(), sizeof(buffer));
		if (IMGUI_LEFT_LABEL(ImGui::InputText, "Name: ", buffer, sizeof(buffer)))
		{
			comp.Tag = std::string(buffer);
		}
	}

	void EditorDraws::DrawTransformComponent(TransformComponent& comp)
	{
		
		m_Flags = ImGuiTreeNodeFlags_DefaultOpen;
		
		if (ImGui::TreeNodeEx("Transform", m_Flags))
		{
			ImGui::Text("Position: "); ImGui::SameLine();
			ImGui::PushItemWidth(m_InputNumWidth);
			ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##px", &comp.Position.x); ImGui::SameLine();
			ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##pz", &comp.Position.y); ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##py", &comp.Position.z);
			ImGui::PopItemWidth();


			ImGui::Text("Rotation: "); ImGui::SameLine();
			ImGui::PushItemWidth(m_InputNumWidth);
			ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##rx", &comp.Rotation.x); ImGui::SameLine();
			ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##rz", &comp.Rotation.y); ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##ry", &comp.Rotation.z);
			ImGui::PopItemWidth();


			ImGui::Text("   Scale: "); ImGui::SameLine();
			ImGui::PushItemWidth(m_InputNumWidth);
			ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##sx", &comp.Scale.x); ImGui::SameLine();
			ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##sz", &comp.Scale.y); ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##sy", &comp.Scale.z);
			ImGui::PopItemWidth();

			ImGui::TreePop();
		}
	}

	void EditorDraws::DrawMeshRendererComponent(MeshRendererComponent& comp)
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
					bIsUnsaved = true;
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
					bIsUnsaved = true;
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

		//if (comp.PixelShader == EPixelShader::PBRPixelShader || comp.VertexShader == EVertexShader::PBRVertexShader)
		//	bIsPBR = true;
		//else
		//	bIsPBR = false;
	}

	void EditorDraws::DrawTextureComponent(TextureComponent& comp)
	{
		if (ImGui::TreeNodeEx("Texture", m_Flags))
		{
			//if (!bIsPBR)
				TextureBoxes("Texture", comp.Path, comp.ResourceView);
			//else
			//{
			//	TextureBoxes("Albedo", comp.Path, comp.ResourceView);
			//	TextureBoxes("Roughness", comp.RoughPath, comp.RoughView);
			//	TextureBoxes("Normal", comp.NormalPath, comp.NormalView);
			//	TextureBoxes("Height", comp.HeightPath, comp.HeightView);
			//	TextureBoxes("Metalness", comp.MetalnessPath, comp.MetalnessView);
			//}
			ImGui::TreePop();
		}
	}

	void EditorDraws::DrawCameraComponent(CameraComponent& comp)
	{
		ImGui::Text("Camera Component : NOT IMPLEMENTED");
	}

	void EditorDraws::DrawRigidDynamic(RigidDynamicComponent& comp)
	{

		if (ImGui::TreeNodeEx("Rigid Dynamic", m_Flags))
		{
			ImGui::Text("Rigid Dynamic Component : BEING IMPLEMENTED");

			ImGui::TreePop();
		}
	}

	void EditorDraws::DrawStaticDynamic(RigidStaticComponent& comp)
	{
		if (ImGui::TreeNodeEx("Static Dynamic", m_Flags))
		{
			ImGui::Text("Static Dynamic Component : BEING IMPLEMENTED");

			ImGui::TreePop();
		}
	}

	void EditorDraws::DrawCollisionComponent(CollisionComponents& comp)
	{
		ImGui::Text("Collision Component : NOT IMPLEMENTED");
	}

	void EditorDraws::DrawScriptComponent(ScriptComponent& comp)
	{
		ImGui::Text("Script Component : NOT IMPLEMENTED");
	}

	int EditorDraws::RendererComboBox(const std::string& label, const char* items[], int size, int& selected)
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

	void EditorDraws::TextureBoxes(std::string Label, std::string& path, CComPtr<ID3D11ShaderResourceView>& resourseView)
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

