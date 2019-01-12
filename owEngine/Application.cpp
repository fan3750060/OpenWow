#include "stdafx.h"

// Genreal
#include "Application.h"

// Additional
#include "EngineTime.h"

#include "..\\owRenderDX11\\owRenderDX11.h"
#include "..\\owRenderOpenGL\\owRenderOpenGL.h"

#define RENDER_WINDOW_CLASS_NAME "RenderWindowClass"

float g_GameDeltaTime = 0.0f;
float g_ApplicationTime = 0.0f;
int64_t g_FrameCounter = 0L;

// Global Window Procedure callback function
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static RenderWindow* gs_WindowHandle = nullptr;
static HWND gs_hWindow = 0;
static Application* gs_pApplicationInstance = nullptr;

Application::Application()
	: m_bIsInitialized(false)
	, m_bIsRunning(false)
{
	m_hInstance = ::GetModuleHandle(NULL);

	HINSTANCE hDll;
	hDll = LoadLibrary("SHELL32.dll");

	// Register a window class for creating our render windows with.
	WNDCLASSEX renderWindowClass;
	renderWindowClass.cbSize = sizeof(WNDCLASSEX);
	renderWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	renderWindowClass.lpfnWndProc = &WndProc;
	renderWindowClass.cbClsExtra = 0;
	renderWindowClass.cbWndExtra = 0;
	renderWindowClass.hInstance = m_hInstance;
	renderWindowClass.hIcon = LoadIcon(hDll, MAKEINTRESOURCE(2));
	renderWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	renderWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	renderWindowClass.lpszMenuName = NULL;
	renderWindowClass.lpszClassName = RENDER_WINDOW_CLASS_NAME;
	renderWindowClass.hIconSm = LoadIcon(hDll, MAKEINTRESOURCE(2));

	if (!RegisterClassEx(&renderWindowClass))
	{
		fail2("Failed to register the render window class.");
	}

#ifdef  IS_DX11
	m_pRenderDevice = CreateRenderDeviceDX11(_BaseManager);
#else
	m_pRenderDevice = CreateRenderDeviceOGL(_BaseManager);
#endif

	gs_pApplicationInstance = this;
}

Application& Application::Get()
{
	assert(gs_pApplicationInstance != nullptr);
	return *gs_pApplicationInstance;
}

Application::~Application()
{
	if (!UnregisterClass(RENDER_WINDOW_CLASS_NAME, m_hInstance))
	{
		fail1("Failed to unregister render window class");
	}

	gs_pApplicationInstance = nullptr;
}

HINSTANCE Application::GetModuleHandle() const
{
	return m_hInstance;
}

RenderWindow* Application::CreateRenderWindow(cstring windowName, int windowWidth, int windowHeight, bool vSync)
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	RECT windowRect = { 0, 0, windowWidth, windowHeight };

	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	windowWidth = windowRect.right - windowRect.left;
	windowHeight = windowRect.bottom - windowRect.top;

	int windowX = (screenWidth - windowWidth) / 2;
	int windowY = (screenHeight - windowHeight) / 2;

	HWND hWindow = CreateWindowEx
	(
		NULL,
		RENDER_WINDOW_CLASS_NAME,
		windowName.c_str(),
		WS_OVERLAPPEDWINDOW,
		windowX, windowY, windowWidth, windowHeight,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);

	if (!hWindow)
	{
		fail1("Failed to create render window.");
	}

#ifdef  IS_DX11
	m_Windows = CreateRenderWindowDX11(hWindow, GetRenderDevice(), windowName, windowWidth, windowHeight, vSync);
#else
	m_Windows = CreateRenderWindowOGL(hWindow, GetRenderDevice(), windowName, windowWidth, windowHeight, vSync);
#endif

	gs_hWindow = hWindow;
	gs_WindowHandle = m_Windows;

	if (m_bIsRunning)
	{
		EventArgs eventArgs(*this);
		m_Windows->OnInitialize(eventArgs);
	}

	UpdateWindow(hWindow);

	return m_Windows;
}


IRenderDevice* Application::GetRenderDevice()
{
	assert1(m_pRenderDevice);
	return m_pRenderDevice;
}

RenderWindow* Application::GetRenderWindow()
{
	return m_Windows;
}

// Convert the message ID into a MouseButton ID
static MouseButtonEventArgs::MouseButton DecodeMouseButton(UINT messageID)
{
	MouseButtonEventArgs::MouseButton mouseButton = MouseButtonEventArgs::None;
	switch (messageID)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	{
		mouseButton = MouseButtonEventArgs::Left;
	}
	break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	{
		mouseButton = MouseButtonEventArgs::Right;
	}
	break;
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	{
		mouseButton = MouseButtonEventArgs::Middel;
	}
	break;
	}

	return mouseButton;
}

// Convert the message ID into a ButtonState.
static MouseButtonEventArgs::ButtonState DecodeButtonState(UINT messageID)
{
	MouseButtonEventArgs::ButtonState buttonState;
	switch (messageID)
	{
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_XBUTTONUP:
	{
		buttonState = MouseButtonEventArgs::Released;
	}
	break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_XBUTTONDOWN:
	{
		buttonState = MouseButtonEventArgs::Pressed;
	}
	break;
	}

	return buttonState;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (gs_WindowHandle != NULL)
	{
		switch (message)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT paintStruct;
			HDC hDC = BeginPaint(hwnd, &paintStruct);
			EndPaint(hwnd, &paintStruct);
		}
		break;
		case WM_KEYDOWN:
		{
			MSG charMsg;

			// Get the unicode character (UTF-16)
			unsigned int c = 0;
			// For printable characters, the next message will be WM_CHAR.
			// This message contains the character code we need to send the KeyPressed event.
			// Inspired by the SDL 1.2 implementation.
			if (PeekMessage(&charMsg, hwnd, 0, 0, PM_NOREMOVE) && charMsg.message == WM_CHAR)
			{
				GetMessage(&charMsg, hwnd, 0, 0);
				c = static_cast<unsigned int>(charMsg.wParam);
			}

			bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
			bool control = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
			bool alt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

			KeyCode key = (KeyCode)wParam;
			unsigned int scanCode = (lParam & 0x00FF0000) >> 16;
			KeyEventArgs keyEventArgs(*gs_WindowHandle, key, c, KeyEventArgs::Pressed, control, shift, alt);
			gs_WindowHandle->OnKeyPressed(keyEventArgs);
		}
		break;
		case WM_KEYUP:
		{
			bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
			bool control = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
			bool alt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

			KeyCode key = (KeyCode)wParam;
			unsigned int c = 0;
			unsigned int scanCode = (lParam & 0x00FF0000) >> 16;

			// Determine which key was released by converting the key code and the scan code
			// to a printable character (if possible).
			// Inspired by the SDL 1.2 implementation.
			unsigned char keyboardState[256];
			GetKeyboardState(keyboardState);
			wchar_t translatedCharacters[4];
			if (int result = ToUnicodeEx((UINT)wParam, scanCode, keyboardState, translatedCharacters, 4, 0, NULL) > 0)
			{
				c = translatedCharacters[0];
			}

			KeyEventArgs keyEventArgs(*gs_WindowHandle, key, c, KeyEventArgs::Released, control, shift, alt);
			gs_WindowHandle->OnKeyReleased(keyEventArgs);
		}
		break;
		case WM_KILLFOCUS:
		{
			// Window lost keyboard focus.
			EventArgs eventArgs(*gs_WindowHandle);
			gs_WindowHandle->OnKeyboardBlur(eventArgs);
		}
		break;
		case WM_SETFOCUS:
		{
			EventArgs eventArgs(*gs_WindowHandle);
			gs_WindowHandle->OnKeyboardFocus(eventArgs);
		}
		break;
		case WM_MOUSEMOVE:
		{
			bool lButton = (wParam & MK_LBUTTON) != 0;
			bool rButton = (wParam & MK_RBUTTON) != 0;
			bool mButton = (wParam & MK_MBUTTON) != 0;
			bool shift = (wParam & MK_SHIFT) != 0;
			bool control = (wParam & MK_CONTROL) != 0;

			int x = ((int)(short)LOWORD(lParam));
			int y = ((int)(short)HIWORD(lParam));

			MouseMotionEventArgs mouseMotionEventArgs(*gs_WindowHandle, lButton, mButton, rButton, control, shift, x, y);
			gs_WindowHandle->OnMouseMoved(mouseMotionEventArgs);
		}
		break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		{
			bool lButton = (wParam & MK_LBUTTON) != 0;
			bool rButton = (wParam & MK_RBUTTON) != 0;
			bool mButton = (wParam & MK_MBUTTON) != 0;
			bool shift = (wParam & MK_SHIFT) != 0;
			bool control = (wParam & MK_CONTROL) != 0;

			int x = ((int)(short)LOWORD(lParam));
			int y = ((int)(short)HIWORD(lParam));

			MouseButtonEventArgs mouseButtonEventArgs(*gs_WindowHandle, DecodeMouseButton(message), MouseButtonEventArgs::Pressed, lButton, mButton, rButton, control, shift, x, y);
			gs_WindowHandle->OnMouseButtonPressed(mouseButtonEventArgs);
		}
		break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		{
			bool lButton = (wParam & MK_LBUTTON) != 0;
			bool rButton = (wParam & MK_RBUTTON) != 0;
			bool mButton = (wParam & MK_MBUTTON) != 0;
			bool shift = (wParam & MK_SHIFT) != 0;
			bool control = (wParam & MK_CONTROL) != 0;

			int x = ((int)(short)LOWORD(lParam));
			int y = ((int)(short)HIWORD(lParam));

			MouseButtonEventArgs mouseButtonEventArgs(*gs_WindowHandle, DecodeMouseButton(message), MouseButtonEventArgs::Released, lButton, mButton, rButton, control, shift, x, y);
			gs_WindowHandle->OnMouseButtonReleased(mouseButtonEventArgs);
		}
		break;
		case WM_MOUSEWHEEL:
		{
			// The distance the mouse wheel is rotated.
			// A positive value indicates the wheel was rotated to the right.
			// A negative value indicates the wheel was rotated to the left.
			float zDelta = ((int)(short)HIWORD(wParam)) / (float)WHEEL_DELTA;
			short keyStates = (short)LOWORD(wParam);

			bool lButton = (keyStates & MK_LBUTTON) != 0;
			bool rButton = (keyStates & MK_RBUTTON) != 0;
			bool mButton = (keyStates & MK_MBUTTON) != 0;
			bool shift = (keyStates & MK_SHIFT) != 0;
			bool control = (keyStates & MK_CONTROL) != 0;

			int x = ((int)(short)LOWORD(lParam));
			int y = ((int)(short)HIWORD(lParam));

			// Convert the screen coordinates to client coordinates.
			POINT clientToScreenPoint;
			clientToScreenPoint.x = x;
			clientToScreenPoint.y = y;
			ScreenToClient(hwnd, &clientToScreenPoint);

			MouseWheelEventArgs mouseWheelEventArgs(*gs_WindowHandle, zDelta, lButton, mButton, rButton, control, shift, (int)clientToScreenPoint.x, (int)clientToScreenPoint.y);
			gs_WindowHandle->OnMouseWheel(mouseWheelEventArgs);
		}
		break;
		// NOTE: Not really sure if these next set of messages are working correctly.
		// Not really sure HOW to get them to work correctly.
		// TODO: Try to fix these if I need them ;)
		case WM_CAPTURECHANGED:
		{
			EventArgs mouseBlurEventArgs(*gs_WindowHandle);
			gs_WindowHandle->OnMouseBlur(mouseBlurEventArgs);
		}
		break;
		case WM_MOUSEACTIVATE:
		{
			EventArgs mouseFocusEventArgs(*gs_WindowHandle);
			gs_WindowHandle->OnMouseFocus(mouseFocusEventArgs);
		}
		break;
		case WM_MOUSELEAVE:
		{
			EventArgs mouseLeaveEventArgs(*gs_WindowHandle);
			gs_WindowHandle->OnMouseLeave(mouseLeaveEventArgs);
		}
		break;
		case WM_SIZE:
		{
			int width = ((int)(short)LOWORD(lParam));
			int height = ((int)(short)HIWORD(lParam));

			ResizeEventArgs resizeEventArgs(*gs_WindowHandle, width, height);
			gs_WindowHandle->OnResize(resizeEventArgs);
		}
		break;
		case WM_CLOSE:
		{
			WindowCloseEventArgs windowCloseEventArgs(*gs_WindowHandle);
			gs_WindowHandle->OnClose(windowCloseEventArgs);

			if (windowCloseEventArgs.ConfirmClose)
			{
				//DestroyWindow( hwnd );
				// Just hide the window.
				// Windows will be destroyed when the application quits.
				ShowWindow(hwnd, SW_HIDE);
			}
		}
		break;
		case WM_DESTROY:
		{
			// TODO delete gs_WindowHandle;
		}
		break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
	}
	else
	{
		switch (message)
		{
		case WM_CREATE:
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}

	}

	return 0;
}

int Application::Run()
{
	static Timer elapsedTime;

	OnInitialize(EventArgs(*this));

	m_bIsRunning = true;

	MSG msg;
	while (m_bIsRunning)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				EventArgs eventArgs(*this);
				OnExit(eventArgs);
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		{
			g_GameDeltaTime = elapsedTime.GetElapsedTime();
			g_ApplicationTime += g_GameDeltaTime;
			++g_FrameCounter;

			UpdateEventArgs updateArgs(*this, g_GameDeltaTime, g_ApplicationTime);
			OnUpdate(updateArgs);

			RenderEventArgs renderArgs(*this, g_GameDeltaTime * 1000.0f, g_ApplicationTime * 1000.0f, g_FrameCounter);
			OnRender(renderArgs);
		}
	}

	OnTerminate(EventArgs(*this));
	OnTerminated(EventArgs(*this));

	return static_cast<int>(msg.wParam);
}

void Application::Stop()
{
	PostQuitMessage(0);
}

//---------------------------------------------------------------
//-- EVENTS
//---------------------------------------------------------------

void Application::OnInitialize(EventArgs& e)
{
	if (m_bIsInitialized) return;

	Initialize(e);

	m_bIsInitialized = true;
}

void Application::OnUpdate(UpdateEventArgs& e)
{
	Update(e);
}

void Application::OnRender(RenderEventArgs& e)
{
	m_Windows->OnPreRender(e);
	m_Windows->OnRender(e);
	m_Windows->OnPostRender(e);
}

void Application::OnTerminate(EventArgs& e)
{
	Terminate(e);

	//delete m_Windows;
}

void Application::OnTerminated(EventArgs& e)
{
	m_bIsInitialized = false;

	Terminated(e);
}

void Application::OnExit(EventArgs& e)
{
	Exit(e);

	// Destroy any windows that are still hanging around.
	DestroyWindow(gs_hWindow);

	// Setting this to false will cause the main application's
	// message pump to stop.
	m_bIsRunning = false;
}

void Application::OnUserEvent(UserEventArgs& e)
{
	UserEvent(e);
}