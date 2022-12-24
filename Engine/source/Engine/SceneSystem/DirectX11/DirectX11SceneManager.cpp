#include "epch.h"
#include "DirectX11SceneManager.h"
#include "Engine/Renderer/DirectX11/DirectX11Renderer.h"
#include "Engine/SceneSystem/Scenes/TestScene.h"
#include "Engine/Lab/GraphicsHelpers.h"

#include "imgui.h"
//#include "imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_sdl.h"



namespace Engine
{
	DirectX11SceneManager::DirectX11SceneManager(std::shared_ptr<IRenderer> renderer, WindowProperties& props)
	{
		m_Renderer = renderer; // Sets the renderer
		m_Props = props; // Sets the properties
		
		m_SceneIndex = 0; // Sets the scene index 

		// Creates the scene
		CreateScenesFromFile();
		CreateSceneFromObject(); // TEMP

	}

	DirectX11SceneManager::~DirectX11SceneManager()
	{
		
	}

	void DirectX11SceneManager::Release()
	{
		/*for (int i = 0; i < m_Scenes.size(); ++i)
		{
			m_Scenes[i]->ReleaseResources();
		}*/
	}

	void DirectX11SceneManager::CreateScenesFromFile()
	{

	}

	void DirectX11SceneManager::CreateSceneFromObject()
	{
		//std::shared_ptr<TestScene> scene = std::make_shared<TestScene>(this, m_Renderer, 0); // Creates a new temp scene
		//auto pos = m_Scenes.begin() + scene->GetSceneIndex(); // Used to add the scene at the correct position
		//m_Scenes.insert(pos, scene); // Adds scene to the array

		m_Scene = std::make_shared<Scene>(m_Renderer);

	}

	bool DirectX11SceneManager::LoadFirstScene()
	{
		//if (m_Scenes[m_SceneIndex]->InitGeometry() && m_Scenes[m_SceneIndex]->InitScene()) // Loads the first scenes 
		//{
		//	Layer.SetScene(m_Scenes[m_SceneIndex]);
		//	return true;
		//}
		//else
		//{
		//	LOG_ERROR("Error Loading Scene");
		//	return false;
		//}

		m_Scene->InitScene();
		//Layer.SetScene(m_Scene);
		
		return true;
	}

	void DirectX11SceneManager::LoadScene(int index)
	{
		//m_SceneIndex = index; // Sets the scene index then Initializes the correct scene
		//m_Scenes[m_SceneIndex]->InitGeometry();
		//m_Scenes[m_SceneIndex]->InitScene();

		m_Scene->InitScene();

	}

	void DirectX11SceneManager::RemoveCurrentScene()
	{
		//m_Scenes[m_SceneIndex]->ReleaseResources(); // Releases the current scenes resources

		m_Scene->RemoveScene();
	}

	void DirectX11SceneManager::RemoveSceneAtIndex(int index)
	{
		//m_Scenes[index]->ReleaseResources(); // Releases the scenes resources at index
		m_Scene->RemoveScene();
	}

	void DirectX11SceneManager::SceneLoop(float frameTime)
	{
		RenderScene(); // Renders the correct scene
		//m_Scenes[m_SceneIndex]->UpdateScene(frameTime); // Updates the correct scene
		m_Scene->UpdateScene(frameTime);
	}

	void DirectX11SceneManager::RenderScene()
	{
		ImGui_ImplDX11_NewFrame();
		//ImGui_ImplWin32_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		
		//// Common settings ////
		if (m_Renderer->GetRendererType() == ERendererType::DirectX11) // Checks the correct renderer
		{

			std::shared_ptr<DirectX11Renderer> d11Renderer = std::static_pointer_cast<DirectX11Renderer>(m_Renderer);// Casts the renderer to the correct renderer
			//Layer.SetSceneTexture(d11Renderer->GetSceneTexture());

			// Sets the correct scene settings
			/*d11Renderer->PerFrameConstants.ambientColour = m_Scenes[m_SceneIndex]->GetAmbientColour();
			d11Renderer->PerFrameConstants.specularPower = m_Scenes[m_SceneIndex]->GetSpecularPower();
			d11Renderer->PerFrameConstants.cameraPosition = m_Scenes[m_SceneIndex]->GetCamera()->Position();*/

			d11Renderer->PerFrameConstants.ambientColour = m_Scene->GetSceneSettings().ambientColour;
			d11Renderer->PerFrameConstants.specularPower = m_Scene->GetSceneSettings().specularPower;
			d11Renderer->PerFrameConstants.cameraPosition = m_Scene->GetCamera()->Position();

			//// Main scene rendering ////

			// Set the back buffer as the target for rendering and select the main depth buffer.
			// When finished the back buffer is sent to the "front buffer" - which is the monitor.
			CComPtr<ID3D11RenderTargetView> backBuffer = d11Renderer->GetBackBuffer();
			d11Renderer->GetDeviceContext()->OMSetRenderTargets(1, &backBuffer.p, d11Renderer->GetDepthStencil());

			CComPtr<ID3D11RenderTargetView> texture = d11Renderer->GetSceneRenderTarget();
			d11Renderer->GetDeviceContext()->OMSetRenderTargets(1, &texture.p, d11Renderer->GetDepthStencil());
		

			// Clear the back buffer to a fixed colour and the depth buffer to the far distance
			//glm::vec4 backgroundColour = m_Scenes[m_SceneIndex]->GetBackgroundColour();
			glm::vec4 backgroundColour = m_Scene->GetSceneSettings().backgroundColour;
			d11Renderer->GetDeviceContext()->ClearRenderTargetView(d11Renderer->GetBackBuffer(), &backgroundColour.r);
			d11Renderer->GetDeviceContext()->ClearRenderTargetView(d11Renderer->GetSceneRenderTarget(), &backgroundColour.r);
			d11Renderer->GetDeviceContext()->ClearDepthStencilView(d11Renderer->GetDepthStencil(), D3D11_CLEAR_DEPTH, 1.0f, 0);


			// Render the scene from the main camera
			RenderSceneFromCamera();

			//Layer.RenderGUI();
			
			ImGui::Render();
			d11Renderer->GetDeviceContext()->OMSetRenderTargets(1, &backBuffer.p, nullptr);

			//// Scene completion ////
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			//Layer.Update();
			
			// When drawing to the off-screen back buffer is complete, we "present" the image to the front buffer (the screen)
			// Set first parameter to 1 to lock to vsync (typically 60fps)
			//d11Renderer->GetSwapChain()->Present(m_Scenes[m_SceneIndex]->GetVSync() ? 1 : 0, 0);
			d11Renderer->GetSwapChain()->Present(m_Scene->GetSceneSettings().vsyncOn ? 1 : 0, 0);
			
		}

		ImGuiIO& io = ImGui::GetIO();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void DirectX11SceneManager::RenderSceneFromCamera()
	{
		if (m_Renderer->GetRendererType() == ERendererType::DirectX11)
		{
			std::shared_ptr<DirectX11Renderer> d11Renderer = std::static_pointer_cast<DirectX11Renderer>(m_Renderer);
			// Set camera matrices in the constant buffer and send over to GPU
			/*d11Renderer->PerFrameConstants.viewMatrix = m_Scenes[m_SceneIndex]->GetCamera()->ViewMatrix();
			d11Renderer->PerFrameConstants.EngineionMatrix = m_Scenes[m_SceneIndex]->GetCamera()->EngineionMatrix();
			d11Renderer->PerFrameConstants.viewEngineionMatrix = m_Scenes[m_SceneIndex]->GetCamera()->ViewEngineionMatrix();*/
			d11Renderer->PerFrameConstants.viewMatrix = m_Scene->GetCamera()->ViewMatrix();
			d11Renderer->PerFrameConstants.EngineionMatrix = m_Scene->GetCamera()->EngineionMatrix();
			d11Renderer->PerFrameConstants.viewEngineionMatrix = m_Scene->GetCamera()->ViewEngineionMatrix();

			UpdateConstantBuffer(d11Renderer->GetDeviceContext(), d11Renderer->PerFrameConstantBuffer, d11Renderer->PerFrameConstants);

			// Indicate that the constant buffer we just updated is for use in the vertex shader (VS) and pixel shader (PS)
			d11Renderer->GetDeviceContext()->VSSetConstantBuffers(0, 1, &d11Renderer->PerFrameConstantBuffer.p); // First parameter must match constant buffer number in the shader 
			d11Renderer->GetDeviceContext()->PSSetConstantBuffers(0, 1, &d11Renderer->PerFrameConstantBuffer.p);

			m_Scene->RenderScene();

		}
	}
}