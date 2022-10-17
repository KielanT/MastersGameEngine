#pragma once

namespace Engine
{
	class EditorLayer
	{
	public:
		EditorLayer() = default;
		~EditorLayer();

		EditorLayer(const EditorLayer&) = delete;
		EditorLayer(EditorLayer&&) = delete;
		EditorLayer& operator=(const EditorLayer&) = delete;
		EditorLayer& operator=(EditorLayer&&) = delete;
	
	public:
		void RenderGUI();
		
	private:
		void MainWindow();
	};
}

