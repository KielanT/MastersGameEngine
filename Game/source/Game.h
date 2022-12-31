#pragma once

#include <Engine.h>

namespace Engine 
{
	class Game : public ILayer
	{
	public:
		Game();

		virtual bool Init() override;
		virtual void Render() override;
		virtual void Update(float frameTime) override;

	private:
		std::shared_ptr<Engine::Scene> m_Scene;
	};
}

