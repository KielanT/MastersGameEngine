#pragma once

#include <Engine.h>
#include <Engine/Scripting/ScriptClass.h>



#define IMGUI_LEFT_LABEL(func, label, ...) (ImGui::TextUnformatted(label), ImGui::SameLine(), func("##" label, __VA_ARGS__))

namespace Engine
{
	class EditorDraws
	{
	public:
		void DrawComponents(Entity& entity, std::filesystem::path assetPath);

	private:
		void DrawIDComponent(IDComponent& comp);

		void DrawTransformComponent(TransformComponent& comp);

		void DrawMeshRendererComponent(MeshRendererComponent& comp, Entity& entity);

		void DrawTextureComponent(TextureComponent& comp, Entity& entity);

		void DrawCameraComponent(CameraComponent& comp, Entity& entity);

		void DrawRigidDynamic(RigidDynamicComponent& comp, Entity& entity);

		void DrawCollisionComponent(CollisionComponents& comp, Entity& entity);

		void DrawScriptComponent(ScriptComponent& comp, Entity& entity);

		void DrawSkyboxComponent(SkyboxComponent& comp, Entity& entity);


		int ComboBox(const std::string& label, const char* items[], int size, int& selected);
		void TextureBoxes(std::string Label, std::string& path, CComPtr<ID3D11ShaderResourceView>& resourseView);

		void DrawField(const std::string& name, const ScriptField& field, std::shared_ptr<ScriptClass> scriptClass, ScriptComponent& comp);

		//*************
		// Helpers
		//*************
		void CreateScript(const std::string& className);

	public:

		bool bIsUnsaved = false;

	private:
		int m_Flags; // ImGuiTreeNodeFlags

		float m_InputNumWidth = 50.0f;

		std::filesystem::path m_AssetPath;
	};
}

