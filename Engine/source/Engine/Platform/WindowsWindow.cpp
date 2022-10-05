#include "epch.h"
#include "WindowsWindow.h"

namespace Engine
{
	IWindow* IWindow::Create(WindowProperties& props)
	{
		return new WindowsWindow(props);
	}
	
	WindowsWindow::WindowsWindow(WindowProperties& props)
	{
		Init(props);
	}
	
	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	BOOL WindowsWindow::Init(WindowProperties& props)
	{
		m_Window = S_OK;
		m_Props = props;
		m_Window = CreateDesktopWindow();
		
		return TRUE;
	}

	void WindowsWindow::Update()
	{
		if (SUCCEEDED(m_Window))
		{
			m_Window = Run();
		}
	}

	void WindowsWindow::Shutdown()
	{
		DestroyWindow(m_hWnd);
	}

	LRESULT WindowsWindow::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_PAINT: // A necessary message to ensure the window content is displayed
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;

		case WM_DESTROY: // Another necessary message to deal with the window being closed
			PostQuitMessage(0);
			break;


			// The WM_KEYXXXX messages report keyboard input to our window.
			// This application has added some simple functions (not DirectX) to process these messages (all in Input.cpp/h)
			// so you don't need to change this code. Instead simply use KeyHit, KeyHeld etc.
		/*case WM_KEYDOWN:
			KeyDownEvent(static_cast<KeyCode>(wParam));
			break;

		case WM_KEYUP:
			KeyUpEvent(static_cast<KeyCode>(wParam));
			break;
*/

			// The following WM_XXXX messages report mouse movement and button presses
			// Use KeyHit to get mouse buttons, GetMouseX, GetMouseY for its position
		/*case WM_MOUSEMOVE:
		{
			MouseMoveEvent(LOWORD(lParam), HIWORD(lParam));
			break;
		}
		case WM_LBUTTONDOWN:
		{
			KeyDownEvent(Mouse_LButton);
			break;
		}
		case WM_LBUTTONUP:
		{
			KeyUpEvent(Mouse_LButton);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			KeyDownEvent(Mouse_RButton);
			break;
		}
		case WM_RBUTTONUP:
		{
			KeyUpEvent(Mouse_RButton);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			KeyDownEvent(Mouse_MButton);
			break;
		}
		case WM_MBUTTONUP:
		{
			KeyUpEvent(Mouse_MButton);
			break;
		}*/


		// Any messages we don't handle are passed back to Windows default handling
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

	

	HRESULT WindowsWindow::CreateDesktopWindow()
	{
		SHSTOCKICONINFO stockIcon;
		stockIcon.cbSize = sizeof(stockIcon);

		if (SHGetStockIconInfo(SIID_APPLICATION, SHGSI_ICON, &stockIcon) != S_OK) 
		{
			return E_FAIL;
		}

		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WindowsWindow::WindowProc; 
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0; SIID_APPLICATION;
		wcex.hInstance = m_hInstance;
		wcex.hIcon = stockIcon.hIcon; 
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW); 
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = L"WinClass";
		wcex.hIconSm = stockIcon.hIcon;
		if (!RegisterClassEx(&wcex)) 
		{
			return E_FAIL;
		}


		DWORD windowStyle = WS_OVERLAPPEDWINDOW; 
		RECT rc = { 0, 0, m_Props.Width, m_Props.Height };
		AdjustWindowRect(&rc, windowStyle, FALSE);


		std::wstring wTitle = std::wstring(m_Props.Title.begin(), m_Props.Title.end());
		const wchar_t* wcharTitle = wTitle.c_str();
		m_hWnd = CreateWindow(L"WinClass", wcharTitle, windowStyle,
			CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, m_hInstance, nullptr);
		if (!m_hWnd)
		{
			return E_FAIL;
		}

		m_Props.Hwnd = m_hWnd;

		ShowWindow(m_hWnd, 1);
		UpdateWindow(m_hWnd);

		return TRUE;
	}

	HRESULT WindowsWindow::Run()
	{
		HRESULT hr = S_OK;

		// Initlisie input
		if (hr != S_OK)
		{
			return 0;
		}


		MSG msg = {};
		while (msg.message != WM_QUIT) // As long as window is open
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				// Deal with messages
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			else // When no windows messages left to process then render & update our scene
			{
				// Update the scene by the amount of time since the last frame
				//float frameTime = m_Timer.DeltaTime();
				//m_SceneManager->SceneLoop(frameTime);
			}
		}

		
	}

	
}