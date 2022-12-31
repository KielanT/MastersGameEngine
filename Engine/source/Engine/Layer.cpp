#include "epch.h"
#include "Layer.h"

namespace Engine
{
	static std::shared_ptr<ILayer> m_Layer = nullptr;

	bool Layer::Init(std::shared_ptr<ILayer> layer)
	{
		m_Layer = layer;
		if (layer == nullptr)
		{
			LOG_ERROR("Layer is Null");
			return false;
		}

		if (layer->Init())
			return false;

		return true;
	}
	void Layer::Render()
	{
		if (m_Layer != nullptr)
		{
			m_Layer->Render();
		}
	}
	void Layer::Update(float frameTime)
	{
		m_Layer->Update(frameTime);
	}
}