#pragma once

#include <Engine.h>
#include <Engine/Scripting/ScriptClass.h>

#include "EditorDraws.h"


#define IMGUI_LEFT_LABEL(func, label, ...) (ImGui::TextUnformatted(label), ImGui::SameLine(), func("##" label, __VA_ARGS__))

namespace Engine
{
	class EditorDraws
	{
	public:
		// Main Draw function
		void DrawComponents(Entity& entity, std::filesystem::path assetPath);

	private:
		// Components to draw
		void DrawIDComponent(IDComponent& comp);

		void DrawTransformComponent(TransformComponent& comp);

		void DrawMeshRendererComponent(MeshRendererComponent& comp, Entity& entity);

		void DrawTextureComponent(TextureComponent& comp, Entity& entity);

		void DrawCameraComponent(CameraComponent& comp, Entity& entity);

		void DrawRigidDynamic(RigidDynamicComponent& comp, Entity& entity);

		void DrawCollisionComponent(CollisionComponents& comp, Entity& entity);

		void DrawScriptComponent(ScriptComponent& comp, Entity& entity);

		void DrawSkyboxComponent(SkyboxComponent& comp, Entity& entity);

		// Helper draw functions
		int ComboBox(const std::string& label, const char* items[], int size, int& selected);
		void TextureBoxes(std::string Label, std::string& path, CComPtr<ID3D11ShaderResourceView>& resourseView);

		void DrawField(const std::string& name, const ScriptField& field, std::shared_ptr<ScriptClass> scriptClass, ScriptComponent& comp);


	public:

		bool bIsUnsaved = false;

	private:
		// Default variables
		int m_Flags; // ImGuiTreeNodeFlags
		float m_InputNumWidth = 50.0f;
		std::filesystem::path m_AssetPath;
	};
}

