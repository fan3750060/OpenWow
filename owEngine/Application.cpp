#include "stdafx.h"

// Genreal
#include "Application.h"

// Additional
#include "EngineTime.h"

// Additional renders
#include "..\\owRenderDX11\\owRenderDX11.h"
//#include "..\\owRenderOpenGL\\owRenderOpenGL.h"

float g_GameDeltaTime = 0.0f;
float g_ApplicationTime = 0.0f;
int64_t g_FrameCounter = 0L;

std::shared_ptr<RenderWindow> gs_WindowHandle = nullptr;
IApplication* _ApplicationInstance = nullptr;

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
	renderWindowClass.lpfnWndProc = &Application::WndProc;
	renderWindowClass.cbClsExtra = 0;
	renderWindowClass.cbWndExtra = 0;
	renderWindowClass.hInstance = m_hInstance;
	renderWindowClass.hIcon = LoadIcon(hDll, MAKEINTRESOURCE(2));
	renderWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	renderWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	renderWindowClass.lpszMenuName = NULL;
	renderWindowClass.lpszClassName = c_RenderWindow_ClassName;
	renderWindowClass.hIconSm = LoadIcon(hDll, MAKEINTRESOURCE(2));

	if (!RegisterClassEx(&renderWindowClass))
	{
		fail2("Failed to register the render window class.");
	}

	/*HANDLE hProcess = ::GetCurrentProcess();
	if (::SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS))
		Log::Info("Process priority class set to HIGH");
	else
		Log::Error("Can't set process priority class.");*/

#ifdef  IS_DX11
	m_pRenderDevice = CreateRenderDeviceDX11(_BaseManager);
#else
	m_pRenderDevice = CreateRenderDeviceOGL(_BaseManager);
#endif

	_ApplicationInstance = this;
}

Application::Application(HINSTANCE hInstance)
	: m_bIsInitialized(false)
	, m_bIsRunning(false)
{
	m_hInstance = hInstance;

	_ApplicationInstance = this;
}


Application::~Application()
{
	if (!UnregisterClass(c_RenderWindow_ClassName, m_hInstance))
	{
		//fail1("Failed to unregister render window class");
	}

	_ApplicationInstance = nullptr;
}

IApplication& Application::Get()
{
	assert(_ApplicationInstance != nullptr);
	return *_ApplicationInstance;
}

bool Application::Load()
{
	Random::SetSeed(static_cast<unsigned long>(time(0)));

	// Create window
	CreateRenderWindow("Name", 1280, 1024);
	Initialize += std::bind(&RenderWindow::OnInitialize, m_pWindow, std::placeholders::_1);
	Terminate += std::bind(&RenderWindow::OnTerminate, m_pWindow, std::placeholders::_1);
	Update += std::bind(&RenderWindow::OnUpdate, m_pWindow, std::placeholders::_1);
	m_pWindow->ShowWindow();

	return true;
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

			m_pRenderDevice->Lock();

			Render3DEventArgs renderArgs(*this, g_GameDeltaTime * 166.0f, g_ApplicationTime * 166.0f, g_FrameCounter);
			OnRender(renderArgs);

			RenderUIEventArgs renderUIArgs(*this, g_GameDeltaTime, g_ApplicationTime, g_FrameCounter);
			OnRenderUI(renderUIArgs);

			m_pWindow->Present();

			m_pRenderDevice->Unlock();
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


std::shared_ptr<RenderWindow> Application::CreateRenderWindow(cstring windowName, int windowWidth, int windowHeight, bool vSync)
{
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	RECT windowRect = { 0, 0, windowWidth, windowHeight };
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	windowWidth = windowRect.right - windowRect.left;
	windowHeight = windowRect.bottom - windowRect.top;

	int windowX = (screenWidth - windowWidth) / 2;
	int windowY = (screenHeight - windowHeight) / 2;

	HWND hWindow = CreateWindowEx
	(
		NULL,
		c_RenderWindow_ClassName,
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
		fail2("Failed to create render window.");
	}

#ifdef  IS_DX11
	m_pWindow = CreateRenderWindowDX11(hWindow, GetRenderDevice(), windowName, windowWidth, windowHeight, vSync);
#else
	m_pWindow = CreateRenderWindowOGL(hWindow, GetRenderDevice(), windowName, windowWidth, windowHeight, vSync);
#endif

	m_hWindow = hWindow;
	gs_WindowHandle = m_pWindow;

	if (m_bIsRunning)
	{
		EventArgs eventArgs(*this);
		m_pWindow->OnInitialize(eventArgs);
	}

	::UpdateWindow(hWindow);

	return m_pWindow;
}

CLoader* Application::GetLoader()
{
	return &m_Loader;
}

HINSTANCE Application::Get_HINSTANCE() const
{
	return m_hInstance;
}

HWND Application::Get_HWND() const
{
	return m_hWindow;
}


//
// IApplication
//
std::shared_ptr<IRenderDevice> Application::GetRenderDevice() const
{
	assert1(m_pRenderDevice);
	return m_pRenderDevice;
}

void Application::SetRenderDevice(std::shared_ptr<IRenderDevice> _renderDevice)
{
	m_pRenderDevice = _renderDevice;
}

std::shared_ptr<RenderWindow> Application::GetRenderWindow() const
{
	assert1(m_pWindow);
	return m_pWindow;
}

void Application::SetRenderWindow(std::shared_ptr<RenderWindow> _renderWindow)
{
	m_pWindow = _renderWindow;
	gs_WindowHandle = _renderWindow;
}




//
// IGameStateManager
//

void Application::AddGameState(GameStatesNames::List _name, std::shared_ptr<IGameState> _gameState)
{
	assert1(_gameState != nullptr);
	assert1(m_GameStatesCollection.find(_name) == m_GameStatesCollection.end());

	m_GameStatesCollection.insert(std::make_pair(_name, _gameState));
}

bool Application::SetGameState(GameStatesNames::List _name)
{
	assert1(m_GameStatesCollection.find(_name) != m_GameStatesCollection.end());

	std::shared_ptr<IGameState> gameState = m_GameStatesCollection[_name];
	return SetGameState(gameState);
}

bool Application::SetGameState(std::shared_ptr<IGameState> _newGameState)
{
	assert1(_newGameState);

	Log::Print("GameStateManager[]: Setting new CGameState.");

	// 1. Unset current GameState
	if (m_CurrentGameState != nullptr)
	{
		m_CurrentGameState->Unset();
		m_CurrentGameState->SetCurrent(false);
	}

	// 2. If new GameState not inited, init them
	if (!_newGameState->IsInited())
	{
		Log::Warn("GameStateManager[]: New CGameState in not inited. Initializating.");
		if (_newGameState->Init())
		{
			Log::Green("GameStateManager[]: New CGameState is inited successfully.");
			_newGameState->SetInited(true);
		}
		else
		{
			Log::Error("GameStateManager[]: Error initing new CGameState.");
		}
	}

	// 3. Set new GameState
	m_CurrentGameState = std::dynamic_pointer_cast<CGameState, IGameState>(_newGameState);
	if (m_CurrentGameState->Set())
	{
		Log::Green("GameStateManager[]: New CGameState is current now.");
		m_CurrentGameState->SetCurrent(true);
	}
	else
	{
		Log::Error("GameStateManager[]: Error setting current new CGameState.");
	}

	return true;
}

std::shared_ptr<IGameState> Application::GetGameState()
{
	return m_CurrentGameState;
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

LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			bool shift = (::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
			bool control = (::GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
			bool alt = (::GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

			KeyCode key = (KeyCode)wParam;
			unsigned int c = 0;
			unsigned int scanCode = (lParam & 0x00FF0000) >> 16;

			// Determine which key was released by converting the key code and the scan code
			// to a printable character (if possible).
			// Inspired by the SDL 1.2 implementation.
			unsigned char keyboardState[256];
			::GetKeyboardState(keyboardState);
			wchar_t translatedCharacters[4];
			if (int result = ::ToUnicodeEx((UINT)wParam, scanCode, keyboardState, translatedCharacters, 4, 0, NULL) > 0)
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
			::ScreenToClient(hwnd, &clientToScreenPoint);

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

			::UpdateWindow(hwnd);
		}
		break;
		case WM_CLOSE:
		{
			WindowCloseEventArgs windowCloseEventArgs(*gs_WindowHandle);
			gs_WindowHandle->OnClose(windowCloseEventArgs);

			if (windowCloseEventArgs.ConfirmClose)
			{
				// Just hide the window.
				// Windows will be destroyed when the application quits.
				::ShowWindow(hwnd, SW_HIDE);
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

//---------------------------------------------------------------
//-- EVENTS
//---------------------------------------------------------------

void Application::OnInitialize(EventArgs& e)
{
	if (m_bIsInitialized)
		return;

	Initialize(e);

	m_bIsInitialized = true;
}

void Application::OnUpdate(UpdateEventArgs& e)
{
	Update(e);
}

void Application::OnRender(Render3DEventArgs& e)
{
	m_pWindow->OnPreRender(e);
	m_pWindow->OnRender(e);
	m_pWindow->OnPostRender(e);
}

void Application::OnRenderUI(RenderUIEventArgs & e)
{
	m_pWindow->OnRenderUI(e);
}

void Application::OnTerminate(EventArgs& e)
{
	Terminate(e);

	m_pWindow.reset();
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
	DestroyWindow(m_hWindow);

	// Setting this to false will cause the main application's message pump to stop.
	m_bIsRunning = false;
}

void Application::OnUserEvent(UserEventArgs& e)
{
	UserEvent(e);
}