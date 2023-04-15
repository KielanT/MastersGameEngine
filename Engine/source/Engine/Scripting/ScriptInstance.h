#pragma once
#include "ScriptClass.h"

class _MonoClass;

namespace Engine
{
	class ScriptInstance
	{
	public:
		ScriptInstance(std::shared_ptr<ScriptClass> ScriptClass);

		void OnBegin();

	private:
		std::shared_ptr<ScriptClass> m_ScriptClass;
	};

}
