#pragma once

namespace Engine
{
	class  IRenderer
	{
	public:
		// Initialize the renderer
		virtual bool InitRenderer(WindowProperties& props) = 0;

		// Shutdown the renderer
		virtual void ShutdownRenderer() = 0;

		// Get the renderer type
		virtual const ERendererType GetRendererType() = 0;

		// Get the Window Properties
		virtual WindowProperties GetWindowProperties() = 0;

		virtual void Resize(eint32 height, eint32 width) = 0;
	};

	IRenderer* NewRenderer(const ERendererType type);
}