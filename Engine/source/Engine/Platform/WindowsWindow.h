#pragma once

#include "Engine/Interfaces/IWindow.h"
#include "Engine/Utility/ITimer.h"

namespace Engine
{
	class WindowsWindow : public IWindow
	{
	public:
		WindowsWindow(WindowProperties& props);
		virtual ~WindowsWindow();

		WindowsWindow(const WindowsWindow&) = delete;
		WindowsWindow(WindowsWindow&&) = delete;
		WindowsWindow& operator=(const WindowsWindow&) = delete;
		WindowsWindow& operator=(WindowsWindow&&) = delete;

	public:
		virtual void Update(std::shared_ptr<ISceneManager> m_SceneManager) override;
		virtual void Shutdown() override;

		virtual WindowProperties GetWindowProperties() const override { return m_Props; };
		
		const HWND GetWindowHandle() { return m_hWnd; }
		
		
	private:
		static LRESULT CALLBACK WindowProc(
			HWND hWnd,
			UINT msg,
			WPARAM wParam,
			LPARAM lParam
			);
		

		BOOL Init(WindowProperties& props);

		HRESULT CreateDesktopWindow(WindowProperties& props);
		
		HRESULT Run(std::shared_ptr<ISceneManager> m_SceneManager);
		
		
	private:
		WindowProperties m_Props;

		HWND m_hWnd;
		HRESULT m_Window;
		ITimer m_Timer;
		
	};
	static HINSTANCE m_hInstance;
}
