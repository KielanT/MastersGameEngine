#pragma once

namespace Engine
{
	// Layer for adding the editor or game to the engine
	class ILayer
	{
	public:
		virtual bool Init() = 0;
		virtual void Render() = 0;
		virtual void Update(float frameTime) = 0;
	};
}

