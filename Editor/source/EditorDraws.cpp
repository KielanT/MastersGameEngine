#include "EditorDraws.h"
#include "imgui.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Platform/SDLWinUtils.h"
#include "Engine/Renderer/DirectX11/DX11Renderer.h"
#include <Engine/Physics/Physics.h>
#include <Engine/Scripting/Scripting.h>

namespace Engine
{
	void EditorDraws::DrawComponents(Entity& entity, std::filesystem::path assetPath)
	{
		m_AssetPath = assetPath;
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
			if (entity.HasComponent<CollisionComponents>())
			{
				DrawCollisionComponent(entity.GetComponent<CollisionComponents>(), entity);
				ImGui::Separator();
			}
			if (entity.HasComponent<ScriptComponent>())
			{
				DrawScriptComponent(entity.GetComponent<ScriptComponent>());
				ImGui::Separator();
			}
			if (entity.HasComponent<SkyboxComponent>())
			{
				DrawSkyboxComponent(entity.GetComponent<SkyboxComponent>());
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

					std::string fileName = texturePath.filename().string();

					std::filesystem::path newF = texturePath;
					std::string s;
					while (newF != m_AssetPath)
					{

						if (newF.parent_path() == m_AssetPath)
							break;


						if (s.empty())
							s += newF.parent_path().filename().string() + "/";
						else
							s.insert(0, newF.parent_path().filename().string() + "/");

						newF = newF.parent_path();
					}

					s += fileName;
					comp.Path = s;
			
					std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(texturePath.string());
					comp.Model = std::make_shared<Model>(mesh);
					bIsUnsaved = true;
				}
				ImGui::EndDragDropTarget();
			}



			int bs = static_cast<int>(comp.BlendState);
			const char* blendItems[static_cast<int>(EBlendState::EBlendStateSize)];
			blendItems[0] = "NoBlendingState";
			blendItems[1] = "AdditiveBlending";
			comp.BlendState = static_cast<EBlendState>(ComboBox("Blend State: ", blendItems, static_cast<int>(EBlendState::EBlendStateSize), bs));

			int dss = static_cast<int>(comp.DepthStencil);
			const char* dssItems[static_cast<int>(EDepthStencilState::EDepthStencilStateSize)];
			dssItems[0] = "UseDepthBufferState";
			dssItems[1] = "DepthReadOnlyState";
			dssItems[2] = "NoDepthBufferState";
			comp.DepthStencil = static_cast<EDepthStencilState>(ComboBox("Depth Stencil State: ", dssItems, static_cast<int>(EDepthStencilState::EDepthStencilStateSize), dss));

			int rs = static_cast<int>(comp.RasterizerState);
			const char* rasterizerItems[static_cast<int>(ERasterizerState::ERasterizerStateSize)];
			rasterizerItems[0] = "CullBackState";
			rasterizerItems[1] = "CullFrontState";
			rasterizerItems[2] = "CullNoneState";
			comp.RasterizerState = static_cast<ERasterizerState>(ComboBox("Rasterizer State: ", rasterizerItems, static_cast<int>(ERasterizerState::ERasterizerStateSize), rs));

			int ss = static_cast<int>(comp.SamplerState);
			const char* samplerItems[static_cast<int>(ESamplerState::ESamplerStateSize)];
			samplerItems[0] = "Anisotropic4xSampler";
			samplerItems[1] = "TrilinearSampler";
			samplerItems[2] = "PointSampler";
			samplerItems[3] = "BilinearClamp";
			comp.SamplerState = static_cast<ESamplerState>(ComboBox("Sampler State: ", samplerItems, static_cast<int>(ESamplerState::ESamplerStateSize), ss));
			

			ImGui::TreePop();
		}

	}

	void EditorDraws::DrawTextureComponent(TextureComponent& comp)
	{
		if (ImGui::TreeNodeEx("Texture", m_Flags))
		{

			TextureBoxes("Albedo", comp.Path, comp.ResourceView);
			TextureBoxes("Roughness", comp.RoughPath, comp.RoughView);
			TextureBoxes("Normal", comp.NormalPath, comp.NormalView);
			TextureBoxes("Height", comp.HeightPath, comp.HeightView);
			TextureBoxes("Metalness", comp.MetalnessPath, comp.MetalnessView);
			
			ImGui::TreePop();
		}
	}

	void EditorDraws::DrawCameraComponent(CameraComponent& comp)
	{
		
		if (ImGui::TreeNodeEx("Camera Component", m_Flags)) 
		{
			ImGui::Text("Camera Component : IN PROGRESS");
			//IMGUI_LEFT_LABEL(ImGui::Checkbox, "Active", &comp.IsActive);

			ImGui::TreePop();
		}
		

	}

	void EditorDraws::DrawRigidDynamic(RigidDynamicComponent& comp)
	{

		if (ImGui::TreeNodeEx("Rigid Dynamic", m_Flags))
		{
			IMGUI_LEFT_LABEL(ImGui::Checkbox, "Gravity", &comp.Gravity);
			ImGui::SameLine();

			ImGui::PushItemWidth(m_InputNumWidth);
			ImGui::Text("Mass"); ImGui::SameLine(); ImGui::InputFloat("##mass", &comp.Mass);
			ImGui::PopItemWidth();

			//ImGui::Text("Mass Space Inertia Tensor: "); ImGui::SameLine();
			//ImGui::PushItemWidth(m_InputNumWidth);
			//ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##msitx", &comp.MassSpaceInertiaTensor.x); ImGui::SameLine();
			//ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##msitz", &comp.MassSpaceInertiaTensor.y); ImGui::SameLine();
			//ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##msity", &comp.MassSpaceInertiaTensor.z);
			//ImGui::PopItemWidth();
			//
			//ImGui::Text("Linear Velocity: "); ImGui::SameLine();
			//ImGui::PushItemWidth(m_InputNumWidth);
			//ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##lvx", &comp.LinearVelocity.x); ImGui::SameLine();
			//ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##lvz", &comp.LinearVelocity.y); ImGui::SameLine();
			//ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##lvy", &comp.LinearVelocity.z);
			//ImGui::PopItemWidth();
			//
			//ImGui::Text("Angular Velocity: "); ImGui::SameLine();
			//ImGui::PushItemWidth(m_InputNumWidth);
			//ImGui::Text("X"); ImGui::SameLine(); ImGui::InputFloat("##avx", &comp.AngularVelocity.x); ImGui::SameLine();
			//ImGui::Text("Y"); ImGui::SameLine(); ImGui::InputFloat("##avz", &comp.AngularVelocity.y); ImGui::SameLine();
			//ImGui::Text("Z"); ImGui::SameLine(); ImGui::InputFloat("##avy", &comp.AngularVelocity.z);
			//ImGui::PopItemWidth();

			//ImGui::PushItemWidth(m_InputNumWidth);
			//ImGui::Text("Angular Damping"); ImGui::SameLine(); ImGui::InputFloat("##adx", &comp.AngularDamping);
			//ImGui::PopItemWidth();

			ImGui::TreePop();
		}
	}


	void EditorDraws::DrawCollisionComponent(CollisionComponents& comp, Entity& entity)
	{
		if (ImGui::TreeNodeEx("Collision Component", m_Flags))
		{
			int ct = static_cast<int>(comp.CollisionType);
			const char* ColTypeItems[static_cast<int>(ECollisionTypes::ECollisionTypesSize)];
			ColTypeItems[0] = "Box Collision";
			ColTypeItems[1] = "Sphere Collision";
			
			int previousSelected = ct;
			comp.CollisionType = static_cast<ECollisionTypes>(ComboBox("Collision Type: ", ColTypeItems, static_cast<int>(ECollisionTypes::ECollisionTypesSize), ct));
			
			if (previousSelected != static_cast<int>(comp.CollisionType))
			{
				Physics::CreateCollision(entity);
			}

			ImGui::TreePop();
		}
	
	}

	void EditorDraws::DrawScriptComponent(ScriptComponent& comp)
	{
		if (ImGui::TreeNodeEx("Script Component", m_Flags)) 
		{
			ImGui::Text("Script Component : BEING IMPLEMENTED");

			// TODO: Create a script or select from script list
			std::vector<std::string> ListOfClassNames = Scripting::GetInstance()->GetAllClassNames();

			if (!ListOfClassNames.empty())
			{

				std::string preview_value = ListOfClassNames[comp.selected];



				comp.ClassName = ListOfClassNames[comp.selected];
				
				std::shared_ptr<ScriptInstance> instance = Scripting::GetInstance()->GetScriptInstance(comp.OwnerEntityId);
				if (instance == nullptr)
				{
					Scripting::GetInstance()->CreateScriptInstance(comp);
				}

				if (ImGui::BeginCombo("##Script", preview_value.c_str()))
				{
					for (int n = 0; n < ListOfClassNames.size(); ++n)
					{
						const bool is_selected = (comp.selected == n);
						if (ImGui::Selectable(ListOfClassNames[n].c_str(), is_selected))
						{
							comp.selected = n;
							comp.ClassName = ListOfClassNames[comp.selected];
							comp.FieldMap.clear();
							instance->ChangeScriptClass(Scripting::GetInstance()->GetScriptClassByName(comp.ClassName));
						}

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
				ImGui::Text("No Script Found");
				ImGui::PopStyleColor();
			}

			// TODO be able to change properties and field's from selected script
			std::shared_ptr<ScriptInstance> instance = Scripting::GetInstance()->GetScriptInstance(comp.OwnerEntityId);
			if (instance != nullptr)
			{
				const auto& fields = instance->GetScriptClass()->FieldMap;
				for (const auto& [name, field] : fields)
				{
					DrawField(name, field, instance->GetScriptClass(), comp);
				}
			}

			ImGui::TreePop();
		}
	}

	void EditorDraws::DrawSkyboxComponent(SkyboxComponent& comp)
	{
		if (ImGui::TreeNodeEx("Skybox", m_Flags))
		{
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy_s(buffer, comp.MeshPath.c_str(), sizeof(buffer));
			ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
			IMGUI_LEFT_LABEL(ImGui::InputText, "File Path: ", buffer, sizeof(buffer), flags);

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AssetPayload"))
				{
					const wchar_t* pathp = (const wchar_t*)payload->Data;
					std::filesystem::path path = pathp;

					std::string last = comp.TexPath;

					std::string fileName = path.filename().string();

					std::filesystem::path newF = path;
					std::string s;
					while (newF != m_AssetPath)
					{

						if (newF.parent_path() == m_AssetPath)
							break;


						if (s.empty())
							s += newF.parent_path().filename().string() + "/";
						else
							s.insert(0, newF.parent_path().filename().string() + "/");

						newF = newF.parent_path();
					}

					s += fileName;
					comp.MeshPath = s;

					std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(path.string());
					comp.Model = std::make_shared<Model>(mesh);
					
					bIsUnsaved = true;
				}
				ImGui::EndDragDropTarget();
			}


			TextureBoxes("Skybox map", comp.TexPath, comp.TexMapView);
			ImGui::TreePop();
		}
	}

	int EditorDraws::ComboBox(const std::string& label, const char* items[], int size, int& selected)
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

				std::string fileName = texturePath.filename().string();

				std::filesystem::path newF = texturePath;
				std::string s;
				while (newF != m_AssetPath)
				{

					if (newF.parent_path() == m_AssetPath)
						break;


					if (s.empty())
						s += newF.parent_path().filename().string() + "/";
					else
						s.insert(0, newF.parent_path().filename().string() + "/");

					newF = newF.parent_path();
				}

				s += fileName;
				path = s;

				CComPtr<ID3D11Resource> Resource;
				CComPtr<ID3D11ShaderResourceView> ResourceView;

				if (dx11Render->LoadTexture(texturePath.string(), &Resource, &ResourceView))
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
	}

	void EditorDraws::DrawField(const std::string& name, const ScriptField& field, std::shared_ptr<ScriptClass> scriptClass, ScriptComponent& comp)
	{
		// Used for creating a unique imgui inputs
		std::string FieldName = name + "##" + std::to_string(comp.OwnerEntityId);
		void* dataPtr = nullptr;


		if (field.FieldDataType == ScriptFieldDataTypes::Float)
		{
			float data = scriptClass->GetFieldValue<float>(name);
			bool t = false;
			if (ImGui::DragFloat(FieldName.c_str(), &data))
			{
				scriptClass->SetFieldValue(name, data);
				dataPtr = new float(data);
			}

		
		}

		if (field.FieldDataType == ScriptFieldDataTypes::Int32)
		{
			int data = scriptClass->GetFieldValue<int>(name);
			if (ImGui::DragInt(FieldName.c_str(), &data))
			{
				scriptClass->SetFieldValue(name, data);
				dataPtr = new int(data);
			}
		}

		if (field.FieldDataType == ScriptFieldDataTypes::String)
		{
			//MonoError error;
			std::string dataStr = scriptClass->GetFieldValue(name);
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy_s(buffer, dataStr.c_str(), sizeof(buffer));
			
			if (ImGui::InputText(FieldName.c_str(), buffer, sizeof(buffer)))
			{
				dataStr = buffer;
				scriptClass->SetFieldValue(name, dataStr);
				dataPtr = new std::string(dataStr);
			}
		
		}

		
		if (dataPtr != nullptr)
		{
			std::pair<ScriptFieldDataTypes, void*> pairData = std::make_pair(field.FieldDataType, dataPtr);
			comp.FieldMap[name] = pairData;
		}
	}

}

