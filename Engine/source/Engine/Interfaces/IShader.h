#pragma once


namespace Engine
{
	// Interface that returns the shaders depending on the renderer in use (currently only using DirectX 11)
	class  IShader
	{
	public:
		virtual ~IShader(){}

		// Initialize the shaders 
		virtual bool InitShaders() = 0;

		// Release the shaders
		virtual void ReleaseShaders() = 0;
	};

	// Sets the shader depending on the renderer
	std::shared_ptr<IShader> SetShader(ERendererType renderertype);
}