#include "epch.h"
#include "SceneWriter.h"

namespace Engine
{



	void SceneWriter::Write()
	{
		YAML::Emitter out;
		out << "Hello World!";
	}

}