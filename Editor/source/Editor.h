#pragma once

#include "Engine/EditorLayer.h"

class Editor : public Engine::EditorLayer
{
public:
	Editor();

	virtual void Init() override
	{
		
	}

	virtual void Render()
	{
		ImGui::ShowDemoWindow();
	}
};

