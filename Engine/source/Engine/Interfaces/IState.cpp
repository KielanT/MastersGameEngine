#include "epch.h"
#include "IState.h"
#include "Engine/Renderer/DirectX11/DirectX11States.h"


namespace Engine
{
    std::shared_ptr<IState> SetStates(ERendererType type)
    {
        if (type == ERendererType::DirectX11) // Returns the DirectX 11 States
        {
            return std::make_shared<DirectX11States>();
        }
        else
        {
            return nullptr;
        }
    }
}
