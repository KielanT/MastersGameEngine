#pragma once

namespace Engine
{
	class ILayer
	{
	public:
		virtual bool Init() = 0;
		virtual void Render() = 0;
		virtual void Update(float frameTime) = 0;
	};
}

