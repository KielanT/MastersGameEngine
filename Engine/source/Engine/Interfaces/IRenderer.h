#pragma once

namespace Engine
{
	class IRenderer
	{
	public:
		virtual bool Init(WindowProperties& props) = 0;
		
		virtual void Shutdown() = 0;
		
		virtual const ERendererType GetRendererType() = 0;
		
		virtual WindowProperties GetWindowProperties() = 0;

	};

	IRenderer* NewRenderer(const ERendererType type);
}