#include "epch.h"
#include "ISceneManager.h"
#include "Engine/SceneSystem/DirectX11/DirectX11SceneManager.h"

namespace Engine
{

	ISceneManager* NewSceneManager(IRenderer* renderer)
	{
        if (renderer->GetRendererType() == ERendererType::DirectX11) // Returns the DirectX 11 scene manager
        {
			WindowProperties props = renderer->GetWindowProperties();
			return new DirectX11SceneManager(renderer, props);
           //return new DirectX11SceneManager(renderer, renderer->GetWindowProperties());
           // return new DirectX11SceneManager(renderer, WindowProperties());
        }
        else
        {
            return nullptr;
        }
	}

}