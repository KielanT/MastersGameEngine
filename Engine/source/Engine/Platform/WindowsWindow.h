#pragma once

#include "Engine/Interfaces/IWindow.h"

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
		virtual void Update() override;
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

		HRESULT CreateDesktopWindow();
		
		HRESULT Run();
		
		
	private:
		WindowProperties m_Props;

		HWND m_hWnd;
		HRESULT m_Window;

		
	};
	static HINSTANCE m_hInstance;
}
