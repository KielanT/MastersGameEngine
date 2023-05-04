#pragma once

#include "Engine/Scene/Scene.h"
#include "Engine/Data/Serializer.h"
#include "Engine/Data/Settings.h"


// Static Class to for the serilisation 

namespace Engine
{
	class SceneSerializer
	{
	public:
		
		static void SerializeScene(std::string& path, std::shared_ptr<Scene> scene);   // Save the scene to a file
		static void DeserializeScene(std::string& path, std::shared_ptr<Scene> scene); // Load the scene from file
	};

	class SettingsSerilizer
	{
	public:
		static void SerializeEditorSettings(std::string& path, EditorSettings& settings);  
		static void DeserializeEditorSettings(std::string path, EditorSettings& settings); 
	};
}

