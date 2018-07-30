#include "stdafx.h"

#include "Timer.h"

// Genreal
#include "Application.h"

// Additional
#include "Camera.h"
#include "ProgressWindow.h"

#include "DX11/RenderDeviceDX11.h"
#include "DX11/RenderWindowDX11.h"

#include "EngineTime.h"

#define RENDER_WINDOW_CLASS_NAME "RenderWindowClass"
#define PROGRESS_WINDOW_CLASS_NAME "ProgressWindowClass"

float g_GameDeltaTime = 0.0f;
float g_ApplicationTime = 0.0f;
int64_t g_FrameCounter = 0L;

// Global Window Procedure callback function
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

typedef std::map<HWND, RenderWindow*> WindowHandleMap;
static WindowHandleMap gs_WindowHandles;

static Application* gs_pApplicationInstance = nullptr;

Application::Application()
	: m_bIsInitialized(false)
	, m_bIsRunning(false)
	, m_fTotalTime(0.0f)
{
	// Initialize common controls (for progress windows)
	//InitCommonControls();

	m_hInstance = ::GetModuleHandle(NULL);

	// Register a window class for creating our render windows with.
	WNDCLASSEX renderWindowClass;

	renderWindowClass.cbSize = sizeof(WNDCLASSEX);
	renderWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	renderWindowClass.lpfnWndProc = &WndProc;
	renderWindowClass.cbClsExtra = 0;
	renderWindowClass.cbWndExtra = 0;
	renderWindowClass.hInstance = m_hInstance;
	//renderWindowClass.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(APPLICATION_ICON));
	renderWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	renderWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	renderWindowClass.lpszMenuName = NULL;
	renderWindowClass.lpszClassName = RENDER_WINDOW_CLASS_NAME;
	//renderWindowClass.hIconSm = LoadIcon(m_hInstance, MAKEINTRESOURCE(APPLICATION_ICON));

	if (!RegisterClassEx(&renderWindowClass))
	{
		Log::Error("Failed to register the render window class.");
	}

	// Register a window class for creating progress windows with.
	// Register a window class for creating our render windows with.
	WNDCLASSEX progressWindowClass;

	progressWindowClass.cbSize = sizeof(WNDCLASSEX);
	progressWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	progressWindowClass.lpfnWndProc = &WndProc;
	progressWindowClass.cbClsExtra = 0;
	progressWindowClass.cbWndExtra = 0;
	progressWindowClass.hInstance = m_hInstance;
	//progressWindowClass.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCEA(APPLICATION_ICON));
	progressWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	progressWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	progressWindowClass.lpszMenuName = NULL;
	progressWindowClass.lpszClassName = PROGRESS_WINDOW_CLASS_NAME;
	//progressWindowClass.hIconSm = LoadIcon(m_hInstance, MAKEINTRESOURCEA(APPLICATION_ICON));

	if (!RegisterClassEx(&progressWindowClass))
	{
		Log::Error("Failed to register the progress window class.");
	}

	// Create Render device.
#if defined(_WIN32_WINNT_WIN10) && 0
	try
	{
		m_pRenderDevice = new RenderDeviceDX12(*this);
	}
	catch (const std::exception&)
#endif
	{
		m_pRenderDevice = new RenderDeviceDX11(*this);
	}

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
		Log::Error("Failed to unregister render window class");
	}

	if (!UnregisterClass(PROGRESS_WINDOW_CLASS_NAME, m_hInstance))
	{
		Log::Error("Failed to unregister progress window class");
	}

	delete m_pRenderDevice;

	gs_pApplicationInstance = nullptr;
}

HINSTANCE Application::GetModuleHandle() const
{
	return m_hInstance;
}

RenderWindow& Application::CreateRenderWindow(cstring windowName, int windowWidth, int windowHeight, bool vSync)
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
		windowX,
		windowY,
		windowWidth,
		windowHeight,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);

	if (!hWindow)
	{
		Log::Error("Failed to create render window.");
	}

	RenderWindow* pRenderWindow = nullptr;
#if defined(_WIN32_WINNT_WIN10) && 0
	try
	{
		// TODO: I really should abstract device and window creation to an ApplicationDX12 class...
		RenderDeviceDX12* pDevice = dynamic_cast<RenderDeviceDX12*>(m_pRenderDevice);
		assert(pDevice);

		// Try to create a DX12 window.
		pRenderWindow = new RenderWindowDX12(*this, hWindow, *pDevice, windowName, windowWidth, windowHeight, vSync);

	}
	catch (const std::exception&)
#endif
	{
		// If we couldn't create a DX12 window.. fall-back to DX11.

		// TODO: I really should abstract device and window creation to an ApplicationDX11 class...
		RenderDeviceDX11* pDevice = dynamic_cast<RenderDeviceDX11*>(m_pRenderDevice);
		assert(pDevice);

		pRenderWindow = new RenderWindowDX11(*this, hWindow, *pDevice, windowName, windowWidth, windowHeight, vSync);
	}

	m_Windows.insert(WindowMap::value_type(windowName, pRenderWindow));
	gs_WindowHandles.insert(WindowHandleMap::value_type(hWindow, pRenderWindow));

	if (m_bIsRunning)
	{
		// If the application is already running, call initialize on the window
		EventArgs eventArgs(*this);
		pRenderWindow->OnInitialize(eventArgs);
	}

	//    ShowWindow( hWindow, SW_SHOWDEFAULT );
	UpdateWindow(hWindow);

	return *pRenderWindow;
}

ProgressWindow& Application::CreateProgressWindow(cstring title, int windowWidth, int windowHeight)
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
		PROGRESS_WINDOW_CLASS_NAME,
		title.c_str(),
		WS_CAPTION | WS_SYSMENU | WS_THICKFRAME, // WS_OVERLAPPEDWINDOW,
		windowX,
		windowY,
		windowWidth,
		windowHeight,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);

	if (!hWindow)
	{
		Log::Error("Failed to create render window.");
	}

	ProgressWindow* pProgressWindow = new ProgressWindow(*this, hWindow, windowWidth, windowWidth);

	m_Windows.insert(WindowMap::value_type(title, pProgressWindow));
	gs_WindowHandles.insert(WindowHandleMap::value_type(hWindow, pProgressWindow));

	UpdateWindow(hWindow);

	return *pProgressWindow;
}

RenderDevice& Application::GetRenderDevice()
{
	assert(m_pRenderDevice);
	return *m_pRenderDevice;
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
	RenderWindow* pWindow = NULL;

	// This is scoped here otherwise the iterator gets invalidated 
	// if a window is destructed while we are in this message loop.
	{
		WindowHandleMap::iterator iter = gs_WindowHandles.find(hwnd);
		if (iter != gs_WindowHandles.end())
		{
			pWindow = iter->second;
		}
	}

	if (pWindow != NULL)
	{
		switch (message)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT paintStruct;
			HDC hDC;

			hDC = BeginPaint(hwnd, &paintStruct);
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
			KeyEventArgs keyEventArgs(*pWindow, key, c, KeyEventArgs::Pressed, control, shift, alt);
			pWindow->OnKeyPressed(keyEventArgs);
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

			KeyEventArgs keyEventArgs(*pWindow, key, c, KeyEventArgs::Released, control, shift, alt);
			pWindow->OnKeyReleased(keyEventArgs);
		}
		break;
		case WM_KILLFOCUS:
		{
			// Window lost keyboard focus.
			EventArgs eventArgs(*pWindow);
			pWindow->OnKeyboardBlur(eventArgs);
		}
		break;
		case WM_SETFOCUS:
		{
			EventArgs eventArgs(*pWindow);
			pWindow->OnKeyboardFocus(eventArgs);
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

			MouseMotionEventArgs mouseMotionEventArgs(*pWindow, lButton, mButton, rButton, control, shift, x, y);
			pWindow->OnMouseMoved(mouseMotionEventArgs);
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

			MouseButtonEventArgs mouseButtonEventArgs(*pWindow, DecodeMouseButton(message), MouseButtonEventArgs::Pressed, lButton, mButton, rButton, control, shift, x, y);
			pWindow->OnMouseButtonPressed(mouseButtonEventArgs);
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

			MouseButtonEventArgs mouseButtonEventArgs(*pWindow, DecodeMouseButton(message), MouseButtonEventArgs::Released, lButton, mButton, rButton, control, shift, x, y);
			pWindow->OnMouseButtonReleased(mouseButtonEventArgs);
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

			MouseWheelEventArgs mouseWheelEventArgs(*pWindow, zDelta, lButton, mButton, rButton, control, shift, (int)clientToScreenPoint.x, (int)clientToScreenPoint.y);
			pWindow->OnMouseWheel(mouseWheelEventArgs);
		}
		break;
		// NOTE: Not really sure if these next set of messages are working correctly.
		// Not really sure HOW to get them to work correctly.
		// TODO: Try to fix these if I need them ;)
		case WM_CAPTURECHANGED:
		{
			EventArgs mouseBlurEventArgs(*pWindow);
			pWindow->OnMouseBlur(mouseBlurEventArgs);
		}
		break;
		case WM_MOUSEACTIVATE:
		{
			EventArgs mouseFocusEventArgs(*pWindow);
			pWindow->OnMouseFocus(mouseFocusEventArgs);
		}
		break;
		case WM_MOUSELEAVE:
		{
			EventArgs mouseLeaveEventArgs(*pWindow);
			pWindow->OnMouseLeave(mouseLeaveEventArgs);
		}
		break;
		case WM_SIZE:
		{
			int width = ((int)(short)LOWORD(lParam));
			int height = ((int)(short)HIWORD(lParam));

			ResizeEventArgs resizeEventArgs(*pWindow, width, height);
			pWindow->OnResize(resizeEventArgs);
		}
		break;
		case WM_CLOSE:
		{
			WindowCloseEventArgs windowCloseEventArgs(*pWindow);
			pWindow->OnClose(windowCloseEventArgs);

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
			WindowHandleMap::iterator iter = gs_WindowHandles.find(hwnd);
			if (iter != gs_WindowHandles.end())
			{
				gs_WindowHandles.erase(iter);
			}
		}
		break;
		default:
		{
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		break;
		}
	}
	else
	{
		switch (message)
		{
		case WM_CREATE:
			break;
		default:
		{
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		break;
		}

	}

	return 0;
}

int Application::Run()
{
	static Timer elapsedTime;

	OnInitialize(EventArgs(*this));

	m_bIsRunning = true;

	MSG          msg;
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
		//else
		{
			g_GameDeltaTime = elapsedTime.GetElapsedTime();
			g_ApplicationTime += g_GameDeltaTime;
			++g_FrameCounter;

			UpdateEventArgs updateArgs(*this, g_GameDeltaTime, g_ApplicationTime);
			OnUpdate(updateArgs);

			RenderEventArgs renderArgs(*this, g_GameDeltaTime, g_ApplicationTime, g_FrameCounter);
			OnRender(renderArgs);
		}
	}

	OnTerminate(EventArgs(*this));
	OnTerminated(EventArgs(*this));

	return static_cast<int>(msg.wParam);
}

void Application::Stop()
{
	// Request to stop the application.
	// Push a quit event on to the event queue to ensure
	// events are processed in the correct order.
	PostQuitMessage(0);
}

void Application::OnInitialize(EventArgs& e)
{
	if (m_bIsInitialized) return;

	Initialize(e);

	m_bIsInitialized = true;
}

void Application::OnTerminate(EventArgs& e)
{
	Terminate(e);

	// I had to move this block of code from the destructor to  here 
	// due to a bug in the Visual Studio 2013 runtime (msvc12).
	// This bug is resolved in Visual Studio 2015 runtime (msvc14).
	// A std::thread::join would hang indefinently if the join occurs
	// after main exits. Since the application can be statically declared
	// in global scope, it's destruction would occur during destruction of 
	// statically declared variables (after main exits) and causing the join
	// method to hang indefinently.
	// See: https://connect.microsoft.com/VisualStudio/feedback/details/747145

	for (auto window : m_Windows)
	{
		delete (window.second);
	}
	m_Windows.clear();
}

void Application::OnTerminated(EventArgs& e)
{

	m_bIsInitialized = false;

	Terminated(e);
}

void Application::OnUpdate(UpdateEventArgs& e)
{
	Update(e);
}

void Application::OnRender(RenderEventArgs& e)
{
	for (auto window : m_Windows)
	{
		RenderWindow* pWindow = window.second;
		pWindow->OnPreRender(e);
		pWindow->OnRender(e);
		pWindow->OnPostRender(e);
	}
}

// The user requested to exit the application
void Application::OnExit(EventArgs& e)
{
	Exit(e);

	// Destroy any windows that are still hanging around.
	while (!gs_WindowHandles.empty())
	{
		DestroyWindow((*gs_WindowHandles.begin()).first);
	}

	// Setting this to false will cause the main application's
	// message pump to stop.
	m_bIsRunning = false;
}

// A user event was generated
void Application::OnUserEvent(UserEventArgs& e)
{
	UserEvent(e);
}

float Application::GetTotalApplicationTime() const
{
	return m_fTotalTime;
}