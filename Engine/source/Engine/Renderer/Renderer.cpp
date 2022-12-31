#include "epch.h"
#include "Renderer.h"

#include "imgui.h"
#include "backends/imgui_impl_SDL.h"
#include "backends/imgui_impl_dx11.h"
#include "Engine/SceneSystem/Scenes/Entity.h"

namespace Engine
{
	static std::shared_ptr<RendererAPI> m_RendererAPI = nullptr;

	void Renderer::Init(WindowProperties& props)
	{

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Create Renderer based on the API selected
		m_RendererAPI = Create(ERendererAPI::DirectX11);
		m_RendererAPI->Init(props);

		LOG_DEBUG("Renderer Initilised");
	}

	void Renderer::SetScene(std::shared_ptr<Scene> scene)
	{
		m_RendererAPI->SetScene(scene);
	}

	void Renderer::RenderLoop()
	{
		if (ImGui::GetCurrentContext() != nullptr)
		{
			m_RendererAPI->GUINewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();
		}
;
		m_RendererAPI->RenderLoop();

		if (ImGui::GetCurrentContext() != nullptr)
			ImGui::Render();

		m_RendererAPI->GUIRenderDrawData();
		m_RendererAPI->Present();

		if (ImGui::GetCurrentContext() != nullptr)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}
		}
	}

	void Renderer::RendererEntity(Entity entity)
	{		
		m_RendererAPI->Renderer(entity);
	}


	WindowProperties Renderer::GetWindowProperties()
	{
		return m_RendererAPI->GetWindowProperties();
	}


	void Renderer::ShutdownRenderer()
	{
		m_RendererAPI->ShutdownRenderer();
		LOG_DEBUG("Renderer shutdown");
	}

	CComPtr<ID3D11ShaderResourceView> Renderer::GetSceneTexture()
	{
		return m_RendererAPI->GetSceneTexture();
	}

	std::shared_ptr<RendererAPI> Renderer::GetRendererAPI()
	{
		return m_RendererAPI;
	}

	

}