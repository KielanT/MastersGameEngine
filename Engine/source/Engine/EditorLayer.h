#pragma once

namespace Engine
{
	class EditorLayer
	{
	public:
		EditorLayer();
		~EditorLayer();

		EditorLayer(const EditorLayer&) = delete;
		EditorLayer(EditorLayer&&) = delete;
		EditorLayer& operator=(const EditorLayer&) = delete;
		EditorLayer& operator=(EditorLayer&&) = delete;
	
	public:
		void Update();
		void RenderGUI();
		
		
	private:
		
		void DockSpace();
		void GameWindow();
	};
}

