#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace Engine
{

	class EditorLayer
	{
	public:

		virtual void Init() = 0;

		virtual void Render() = 0;

	};
}
