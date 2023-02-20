#pragma once

#include <Engine.h>



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

		void DrawMeshRendererComponent(MeshRendererComponent& comp);

		void DrawTextureComponent(TextureComponent& comp);

		void DrawCameraComponent(CameraComponent& comp);

		void DrawRigidDynamic(RigidDynamicComponent& comp);

		void DrawCollisionComponent(CollisionComponents& comp, Entity& entity);

		void DrawScriptComponent(ScriptComponent& comp);


		int ComboBox(const std::string& label, const char* items[], int size, int& selected);
		void TextureBoxes(std::string Label, TextureComponent& comp, CComPtr<ID3D11ShaderResourceView>& resourseView);

		//void ChangePhysicsCollision();

	public:

		bool bIsUnsaved = false;

	private:
		int m_Flags; // ImGuiTreeNodeFlags

		float m_InputNumWidth = 50.0f;

		std::filesystem::path m_AssetPath;
	};
}

